//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_INJECTOR_HPP
#define BOOST_DI_CORE_INJECTOR_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/core/any_type.hpp"
#include "boost/di/core/policy.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/core/provider.hpp"
#include "boost/di/core/transform.hpp"
#include "boost/di/core/wrapper.hpp"
#include "boost/di/scopes/exposed.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/config_traits.hpp"
#include "boost/di/concepts/creatable.hpp"

namespace boost { namespace di { namespace core {

BOOST_DI_HAS_METHOD(call, call);

template<class TConfig, class... TDeps>
class injector : public pool<transform_t<TDeps...>>
               , public type_traits::config_traits<TConfig, injector<TConfig, TDeps...>>::type
               , _ {
    template<class...> friend struct provider;
    template<class, class, class> friend struct any_type;
    template<class> friend class scopes::exposed;

    using pool_t = pool<transform_t<TDeps...>>;
    using is_root_t = std::true_type;
    using config_t = typename type_traits::config_traits<TConfig, injector>::type;
    using config = std::conditional_t<
        std::is_default_constructible<TConfig>::value
      , _
      , config_t
    >;

    struct from_injector { };
    struct from_deps { };

public:
    using deps = transform_t<TDeps...>;

    template<class... TArgs>
    explicit injector(const init&, const TArgs&... args) noexcept
        : injector{from_deps{}, arg(args, has_configure<decltype(args)>{})...}
    { }

    template<class TConfig_, class... TDeps_>
    explicit injector(const injector<TConfig_, TDeps_...>& other) noexcept
        : injector{from_injector{}, other, deps{}}
    { }

    template<class T, class TName = no_name, class TIsRoot = std::false_type>
    static constexpr auto is_creatable() {
        return decltype(is_creatable_impl(
            std::declval<T>(), std::declval<TName>(), std::declval<TIsRoot>())
        )::value;
    }

    template<class T
#if !defined(_MSC_VER)
        , BOOST_DI_REQUIRES(is_creatable<T, no_name, is_root_t>())
#endif
        >
    T create() const {
        return create_impl<T, no_name, is_root_t>();
    }

#if !defined(_MSC_VER)
    template<class T, BOOST_DI_REQUIRES(!is_creatable<T, no_name, is_root_t>())>
    BOOST_DI_CONCEPTS_CREATABLE_ATTR
    T create() const {
        return create_impl<T, no_name, is_root_t>();
    }
#endif

    template<class TAction>
    void call(const TAction& action) {
        call_impl(action, deps{});
    }

private:
    template<class... TArgs>
    explicit injector(const from_deps&, const TArgs&... args) noexcept
        : pool_t{init{}, pool<aux::type_list<TArgs...>>{args...}}
        , config{*this}
    { }

    template<class TInjector, class... TArgs>
    explicit injector(const from_injector&, const TInjector& injector, const aux::type_list<TArgs...>&) noexcept
        : pool_t{init{}, pool_t{build<TArgs>(injector)...}}
        , config{*this}
    { }

    template<class T, class TInjector>
    inline auto build(const TInjector& injector) const noexcept {
        return T{injector};
    }

    template<
        class T
      , class TName = no_name
      , class TIsRoot = std::false_type
      , class TDependency = std::remove_reference_t<decltype(binder::resolve<T, TName>((injector*)0))>
      , class TCtor = typename type_traits::ctor_traits<typename TDependency::given>::type
    > static auto try_create_impl() -> std::enable_if_t<std::is_convertible<
       decltype(
           std::declval<TDependency>().template try_create<T>(
               provider<
                   typename TDependency::expected
                 , typename TDependency::given
                 , TName
                 , T
                 , TCtor
                 , injector
               >{std::declval<injector>()}
           )
       ), T>::value
#if !defined(_MSC_VER)
       &&
       decltype(policy<pool_t>::template call<T, TName, TIsRoot>(
          ((TConfig*)0)->policies(), std::declval<TDependency>(), TCtor{})
       )::value
#endif
    >;

    template<class T, class TName = no_name, class TIsRoot = std::false_type>
    auto create_impl() const {
        auto&& dependency = binder::resolve<T, TName>((injector*)this);
        using dependency_t = std::remove_reference_t<decltype(dependency)>;
        using expected_t = typename dependency_t::expected;
        using given_t = typename dependency_t::given;
        using ctor_t = typename type_traits::ctor_traits<given_t>::type;
        using provider_t = core::provider<expected_t, given_t, TName, T, ctor_t, injector>;
        policy<pool_t>::template call<T, TName, TIsRoot>(((TConfig&)*this).policies(), dependency, ctor_t{});
        using wrapper_t = decltype(dependency.template create<T>(provider_t{*this}));
        using create_t = std::conditional_t<
            std::is_reference<T>::value && has_is_ref<dependency_t>::value
          , T
          , std::remove_reference_t<T>
        >;
        return wrapper<create_t, wrapper_t>{dependency.template create<T>(provider_t{*this})};
    }

    static auto is_creatable_impl(...) -> std::false_type;

    template<class T, class TName, class TIsRoot>
    static auto is_creatable_impl(T&&, TName&&, TIsRoot&&)
        -> aux::is_valid_expr<decltype(try_create_impl<T, TName, TIsRoot>())>;

    template<class TAction, class... Ts>
    void call_impl(const TAction& action, const aux::type_list<Ts...>&) {
        int _[]{0, (call_impl<Ts>(action, has_call<Ts, const TAction&>{}), 0)...}; (void)_;
    }

    template<class T, class TAction>
    void call_impl(const TAction& action, const std::true_type&) {
        static_cast<T&>(*this).call(action);
    }

    template<class, class TAction>
    void call_impl(const TAction&, const std::false_type&) { }

    template<class T>
    decltype(auto) arg(const T& arg, const std::false_type&) noexcept {
        return arg;
    }

    template<class T>
    decltype(auto) arg(const T& arg, const std::true_type&) noexcept {
        return arg.configure();
    }
};

}}} // boost::di::core

#endif

