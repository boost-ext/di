//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_INJECTOR_HPP
#define BOOST_DI_CORE_INJECTOR_HPP

#include <memory>
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/any_type.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/core/dependency.hpp"
#include "boost/di/core/policy.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/core/provider.hpp"
#include "boost/di/core/requires.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"

#include <boost/units/detail/utility.hpp>
namespace boost { namespace di { namespace core {

BOOST_DI_HAS_METHOD(call, call);

template<class T, class TWrapper>
struct wrapper {
    inline operator T() noexcept {
        return wrapper_;
    }

    TWrapper wrapper_;
};

template<class>
struct any;

template<class, class>
struct is;

template<class T, class TInitialization, class... TArgs>
struct is<T, aux::pair<TInitialization, aux::type_list<TArgs...>>> {
    using type = aux::is_braces_constructible<T, TArgs...>;
};

template<class P>
struct any {
    template<class T
          , class U = aux::decay_t<T>
          , class = std::enable_if_t<!std::is_same<U, P>{}>
          , class = std::enable_if_t<typename is<U, typename type_traits::ctor_traits<
                typename std::remove_reference_t<decltype(binder::resolve<U>((pool<>*)nullptr))>::given
            >::type>::type{}>
    > operator T();
};

template<class, class, class = void>
struct is_constructible : std::false_type { };

template<class T, class TParent>
struct is_constructible<T, TParent, aux::void_t<decltype(static_cast<T>(any<TParent>{}))>>
    : std::true_type
{ };

template<class TDeps, class TConfig>
class injector : requires_unique_bindings<TDeps>, public pool<TDeps> {
    template<class, class> friend struct any_type;
    template<class...> friend struct provider;

    using pool_t = pool<TDeps>;

public:
    using deps = TDeps;

    // bind<...>, etc.  -> ignore
    // injector<...>    -> get all dependencies from the injector
    // dependency<...>  -> pass

    template<class... TArgs>
    explicit injector(const TArgs&... args) noexcept
        : injector(init{}, pass_arg(args)...)
    { }

    template<class... TArgs>
    injector(const injector<TArgs...>& injector) noexcept // non explicit
        : pool_t{init{}, create_from_injector(injector, TDeps{})}
    { }

    template<class T>
    T create() const {
        constexpr auto is_constructible_v = typename is_constructible<T, void>::type{};
        static_assert(is_constructible_v, "");
        using IsRoot = std::true_type;
        return create_impl<T, no_name, IsRoot>(is_constructible_v);
    }

    template<class TAction>
    void call(const TAction& action) noexcept {
        call_impl(action, deps{});
    }

private:
    template<class... TArgs>
    injector(const init&, const TArgs&... args) noexcept
        : pool_t{init{}, pool<aux::type_list<TArgs...>>{args...}}
    { }

    template<class, class T>
    auto create_t(const aux::type<T>&) const {
        return create_impl<T>();
    }

    template<class TParent, class... Ts>
    auto create_t(const aux::type<any_type<Ts...>>&) const {
        return any_type<TParent, injector>{*this};
    }

    template<class, class T, class TName>
    auto create_t(const aux::type<type_traits::named<TName, T>>&) const {
        return create_impl<T, TName>();
    }

    template<class T, class, class>
    T create_impl(const std::false_type&) const;

    template<class T, class TName = no_name, class TIsRoot = std::false_type>
    auto create_impl(const std::true_type& = {}) const {
        auto&& dependency = binder::resolve<T, TName>((injector*)this);
        using dependency_t = std::remove_reference_t<decltype(dependency)>;
        using expected_t = typename dependency_t::expected;
        using given_t = typename dependency_t::given;
        using ctor_t = typename type_traits::ctor_traits<given_t>::type;
        using provider_t = provider<expected_t, given_t, T, ctor_t, injector>;
        policy<pool_t>::template call<T, TName, TIsRoot>(
            ((TConfig&)config_).policies(), dependency, ctor_t{}
        );
        using wrapper_t = decltype(dependency.template create<T>(provider_t{*this}));
        using type = std::conditional_t<
            std::is_reference<T>{} && has_is_ref<dependency_t>{}
          , T
          , std::remove_reference_t<T>
        >;
        return wrapper<type, wrapper_t>{dependency.template create<T>(provider_t{*this})};
    }

    template<class TAction, class... Ts>
    void call_impl(const TAction& action, const aux::type_list<Ts...>&) noexcept {
        int _[]{0, (call_impl<Ts>(action), 0)...}; (void)_;
    }

    template<class T, class TAction>
    std::enable_if_t<has_call<T, const TAction&>{}>
    call_impl(const TAction& action) noexcept {
        static_cast<T&>(*this).call(action);
    }

    template<class T, class TAction>
    std::enable_if_t<!has_call<T, const TAction&>{}>
    call_impl(const TAction&) noexcept { }

    template<class T>
    decltype(auto) pass_arg(const T& arg, std::enable_if_t<!has_configure<T>{}>* = 0) const noexcept {
        return arg;
    }

    template<class T>
    decltype(auto) pass_arg(const T& arg, std::enable_if_t<has_configure<T>{}>* = 0) const noexcept {
        return arg.configure();
    }

    template<class... TArgs, class... Ts>
    auto create_from_injector(const injector<TArgs...>& injector
                            , const aux::type_list<Ts...>&) const noexcept {
        return pool<TDeps>(create_dep<Ts>(injector)...);
    }

    template<class TDependency, class TInjector>
    auto create_dep(const TInjector& injector) const noexcept {
        return TDependency{injector};
    }

    TConfig config_;
};

}}} // boost::di::core

#endif

