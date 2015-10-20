//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_INJECTOR_HPP
#define BOOST_DI_CORE_INJECTOR_HPP

#if !defined(BOOST_DI_CORE_INJECTOR_POLICY)

#include "boost/di/aux_/compiler.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/any_type.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/core/policy.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/core/provider.hpp"
#include "boost/di/core/bindings.hpp"
#include "boost/di/core/wrapper.hpp"
#include "boost/di/scopes/exposed.hpp"
#include "boost/di/type_traits/array_traits.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/generic_traits.hpp"
#include "boost/di/concepts/creatable.hpp"
#include "boost/di/config.hpp"
#include "boost/di/fwd.hpp"

namespace boost { namespace di { inline namespace v1 { namespace core {

struct from_injector { };
struct from_deps { };
struct init { };
struct with_error { };

template<class>
struct copyable;

template<class T>
struct copyable_impl : aux::conditional<
    aux::is_default_constructible<
        typename T::scope::template scope<
            typename T::expected, typename T::given>
    >::value
  , aux::type_list<>
  , aux::type_list<T>
> { };

template<class... TDeps>
struct copyable<aux::type_list<TDeps...>>
    : aux::join<typename copyable_impl<TDeps>::type...>
{ };

template<class TDeps>
using copyable_t = typename copyable<TDeps>::type;

template<class T, class>
struct referable {
    using type = T;
};

template<class T, class TDependency>
struct referable<T&, TDependency> {
    using type = aux::conditional_t<TDependency::template is_referable<T&>::value, T&, T>;
};

template<class T, class TDependency>
struct referable<const T&, TDependency> {
    using type = aux::conditional_t<TDependency::template is_referable<const T&>::value, const T&, T>;
};

#if defined(_MSC_VER) // __pph__
    template<class T, class TDependency>
    struct referable<T&&, TDependency> {
        using type = aux::conditional_t<TDependency::template is_referable<T&&>::value, T&&, T>;
    };
#endif // __pph__

template<class T, class TDependency>
using referable_t = typename referable<T, TDependency>::type;

#if defined(_MSC_VER) // __pph__
    template<class T, class TInjector>
    inline auto build(const TInjector& injector) noexcept {
        return T{injector};
    }
#endif // __pph__

#define BOOST_DI_CORE_INJECTOR_POLICY(...) __VA_ARGS__ BOOST_DI_CORE_INJECTOR_POLICY_ELSE
#define BOOST_DI_CORE_INJECTOR_POLICY_ELSE(...)
#define BOOST_DI_INJECTOR_ITERATE

#endif

template<class TConfig BOOST_DI_CORE_INJECTOR_POLICY(, class TPolicies = pool<>)(), class... TDeps>
class injector BOOST_DI_CORE_INJECTOR_POLICY()(<TConfig, pool<>, TDeps...>) : pool<bindings_t<TDeps...>> {
    friend class binder; template<class> friend struct pool;
    using pool_t = pool<bindings_t<TDeps...>>;

protected:
    template<class T, class TName = no_name, class TIsRoot = aux::false_type>
    struct is_creatable {
        using dependency_t = binder::resolve_t<injector, T, TName>;
        using given_t = type_traits::array_traits_t<T, typename dependency_t::given>;
        using ctor_t = typename type_traits::ctor_traits__<given_t>::type;
        using type = aux::conditional_t<aux::is_same<_, given_t>::value, void, type_traits::generic_traits_t<T, given_t>>;

        static constexpr auto value = aux::is_convertible<
            decltype(
                dependency__<dependency_t>::template try_create<type>(
                    try_provider<
                        given_t
                      , ctor_t
                      , injector
                      , decltype(TConfig::provider(aux::declval<injector>()))
                    >{}
                )
            ), type>::value BOOST_DI_CORE_INJECTOR_POLICY(&&
            policy::template try_call<
                arg_wrapper<referable_t<type, dependency__<dependency_t>>, TName, TIsRoot, pool_t>
              , TPolicies
              , dependency_t
              , ctor_t
            >::value)();
    };

public:
    using deps = bindings_t<TDeps...>;
    using config = TConfig;

    template<class... TArgs>
    explicit injector(const init&, const TArgs&... args) noexcept
        : injector{from_deps{}, args...}
    { }

    template<class TConfig_, class TPolicies_, class... TDeps_>
    explicit injector(const injector<TConfig_, TPolicies_, TDeps_...>& other) noexcept
        : injector{from_injector{}, other, deps{}}
    { }

    template<class T, BOOST_DI_REQUIRES(is_creatable<T, no_name, aux::true_type>::value) = 0>
    T create() const {
        return BOOST_DI_TYPE_WKND(T)create_successful_impl<aux::true_type>(aux::type<T>{});
    }

    template<class T, BOOST_DI_REQUIRES(!is_creatable<T, no_name, aux::true_type>::value) = 0>
    BOOST_DI_CONCEPTS_CREATABLE_ERROR_MSG
    T create() const {
        return BOOST_DI_TYPE_WKND(T)create_impl<aux::true_type>(aux::type<T>{});
    }

    template<class T, BOOST_DI_REQUIRES(!is_injector<T>::value) = 0>
    operator T() const {
        return create<T>();
    }

protected:
    template<class T>
    struct try_create {
        using type = aux::conditional_t<is_creatable<T>::value, typename is_creatable<T>::type, void>;
    };

    template<class TParent>
    struct try_create<any_type_fwd<TParent>> {
        using type = any_type<TParent, injector, with_error>;
    };

    template<class TParent>
    struct try_create<any_type_ref_fwd<TParent>> {
        using type = any_type_ref<TParent, injector, with_error>;
    };

    template<class TParent>
    struct try_create<any_type_1st_fwd<TParent>> {
        using type = any_type_1st<TParent, injector, with_error>;
    };

    template<class TParent>
    struct try_create<any_type_1st_ref_fwd<TParent>> {
        using type = any_type_1st_ref<TParent, injector, with_error>;
    };

    template<class TName, class T>
    struct try_create<di::named<TName, T>> {
        using type = aux::conditional_t<is_creatable<T, TName>::value, typename is_creatable<T, TName>::type, void>;
    };

    template<class TIsRoot = aux::false_type, class T>
    auto create_impl(const aux::type<T>&) const {
        return create_impl__<TIsRoot, T>();
    }

    template<class TIsRoot = aux::false_type, class TParent>
    auto create_impl(const aux::type<any_type_fwd<TParent>>&) const {
        return any_type<TParent, injector>{*this};
    }

    template<class TIsRoot = aux::false_type, class TParent>
    auto create_impl(const aux::type<any_type_ref_fwd<TParent>>&) const {
        return any_type_ref<TParent, injector, aux::false_type, aux::true_type>{*this};
    }

    template<class TIsRoot = aux::false_type, class TParent>
    auto create_impl(const aux::type<any_type_1st_fwd<TParent>>&) const {
        return any_type_1st<TParent, injector>{*this};
    }

    template<class TIsRoot = aux::false_type, class TParent>
    auto create_impl(const aux::type<any_type_1st_ref_fwd<TParent>>&) const {
        return any_type_1st_ref<TParent, injector, aux::false_type, aux::true_type>{*this};
    }

    template<class TIsRoot = aux::false_type, class T, class TName>
    auto create_impl(const aux::type<di::named<TName, T>>&) const {
        return create_impl__<TIsRoot, T, TName>();
    }

    template<class TIsRoot = aux::false_type, class T>
    auto create_successful_impl(const aux::type<T>&) const {
        return create_successful_impl__<TIsRoot, T>();
    }

    template<class TIsRoot = aux::false_type, class TParent>
    auto create_successful_impl(const aux::type<any_type_fwd<TParent>>&) const {
        return successful::any_type<TParent, injector>{*this};
    }

    template<class TIsRoot = aux::false_type, class TParent>
    auto create_successful_impl(const aux::type<any_type_ref_fwd<TParent>>&) const {
        return successful::any_type_ref<TParent, injector>{*this};
    }

    template<class TIsRoot = aux::false_type, class TParent>
    auto create_successful_impl(const aux::type<any_type_1st_fwd<TParent>>&) const {
        return successful::any_type_1st<TParent, injector>{*this};
    }

    template<class TIsRoot = aux::false_type, class TParent>
    auto create_successful_impl(const aux::type<any_type_1st_ref_fwd<TParent>>&) const {
        return successful::any_type_1st_ref<TParent, injector>{*this};
    }

    template<class TIsRoot = aux::false_type, class T, class TName>
    auto create_successful_impl(const aux::type<di::named<TName, T>>&) const {
        return create_successful_impl__<TIsRoot, T, TName>();
    }

private:
    template<class... TArgs>
    explicit injector(const from_deps&, const TArgs&... args) noexcept
        : pool_t{copyable_t<deps>{}, core::pool_t<TArgs...>{args...}}
    { }

    template<class TInjector, class... TArgs>
    explicit injector(const from_injector&, const TInjector& injector, const aux::type_list<TArgs...>&) noexcept
    #if defined(_MSC_VER) // __pph__
        : pool_t{copyable_t<deps>{}, pool_t{build<TArgs>(injector)...}}
    #else // __pph__
        : pool_t{copyable_t<deps>{}, pool_t{TArgs{injector}...}}
    #endif // __pph__
    { }

    template<class TIsRoot = aux::false_type, class T, class TName = no_name>
    auto create_impl__() const {
        auto&& dependency = binder::resolve<T, TName>((injector*)this);
        using dependency_t = aux::remove_reference_t<decltype(dependency)>;
        using expected_t = typename dependency_t::expected;
        using given_t = type_traits::array_traits_t<T, typename dependency_t::given>;
        using type_t = type_traits::generic_traits_t<T, given_t>;
        using ctor_t = typename type_traits::ctor_traits__<given_t>::type;
        using provider_t = core::provider<expected_t, given_t, TName, ctor_t, injector>;
        using wrapper_t = decltype(static_cast<dependency__<dependency_t>&&>(dependency).template create<type_t>(provider_t{*this}));
        BOOST_DI_CORE_INJECTOR_POLICY(
            policy::template call<arg_wrapper<type_t, TName, TIsRoot, pool_t>>(
                TConfig::policies(*this), dependency, ctor_t{}
            );
        )()
        return wrapper<typename aux::is_same<type_t, T>::type, type_t, wrapper_t>{
            static_cast<dependency__<dependency_t>&&>(dependency).template create<type_t>(provider_t{*this})
        };
    }

    template<class TIsRoot = aux::false_type, class T, class TName = no_name>
    auto create_successful_impl__() const {
        auto&& dependency = binder::resolve<T, TName>((injector*)this);
        using dependency_t = aux::remove_reference_t<decltype(dependency)>;
        using expected_t = typename dependency_t::expected;
        using given_t = type_traits::array_traits_t<T, typename dependency_t::given>;
        using ctor_t = typename type_traits::ctor_traits__<given_t>::type;
        using type_t = type_traits::generic_traits_t<T, given_t>;
        using provider_t = successful::provider<expected_t, given_t, ctor_t, injector>;
        using wrapper_t = decltype(static_cast<dependency__<dependency_t>&&>(dependency).template create<type_t>(provider_t{*this}));
        using create_t = referable_t<type_t, dependency__<dependency_t>>;
        BOOST_DI_CORE_INJECTOR_POLICY(
            policy::template call<arg_wrapper<create_t, TName, TIsRoot, pool_t>>(
                TConfig::policies(*this), dependency, ctor_t{}
            );
        )()
        return successful::wrapper<typename aux::is_same<type_t, T>::type, create_t, wrapper_t>{
            static_cast<dependency__<dependency_t>&&>(dependency).template create<type_t>(provider_t{*this})
        };
    }
};

#if defined(BOOST_DI_INJECTOR_ITERATE)
    #undef BOOST_DI_CORE_INJECTOR_HPP
    #undef BOOST_DI_INJECTOR_ITERATE
    #undef BOOST_DI_CORE_INJECTOR_POLICY
    #undef BOOST_DI_CORE_INJECTOR_POLICY_ELSE
    #define BOOST_DI_CORE_INJECTOR_POLICY(...) BOOST_DI_CORE_INJECTOR_POLICY_ELSE
    #define BOOST_DI_CORE_INJECTOR_POLICY_ELSE(...) __VA_ARGS__
    #include "boost/di/core/injector.hpp"
#endif

}} // boost::di::v1::core

#endif

