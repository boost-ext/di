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
#include "boost/di/core/dependency.hpp"
#include "boost/di/core/policy.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/core/provider.hpp"
#include "boost/di/scopes/exposed.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/concepts/creatable.hpp"

namespace boost { namespace di { namespace core {

template<class T, class = void>
struct get_deps {
    using type = typename T::deps;
};

template<class T>
struct get_deps<T, std::enable_if_t<has_configure<T>{}>> {
    using type = typename aux::function_traits<
        decltype(&T::configure)
    >::result_type::deps;
};

template<
    class T
  , class = typename is_injector<T>::type
  , class = typename is_dependency<T>::type
> struct add_type_list;

template<class T, class TAny>
struct add_type_list<T, std::true_type, TAny> {
    using type = typename get_deps<T>::type;
};

template<class T>
struct add_type_list<T, std::false_type, std::true_type> {
    using type = aux::type_list<T>;
};

template<class T>
struct add_type_list<T, std::false_type, std::false_type> {
    using type = aux::type_list<dependency<scopes::exposed<>, T>>;
};

template<class... Ts>
using bindings_t = aux::join_t<typename add_type_list<Ts>::type...>;

template<class T>
decltype(auto) arg(const T& arg, std::enable_if_t<!has_configure<T>{}>* = 0) noexcept {
    return arg;
}

template<class T>
decltype(auto) arg(const T& arg, std::enable_if_t<has_configure<T>{}>* = 0) noexcept {
    return arg.configure();
}

template<class T, class TWrapper, class = void>
struct wrapper {
    using element_type = T;

    inline operator T() const noexcept {
        return wrapper_;
    }

    inline operator T() noexcept {
        return wrapper_;
    }

    TWrapper wrapper_;
};

template<class T, class TWrapper>
struct wrapper<T, TWrapper, REQUIRES<!std::is_convertible<TWrapper, T>{}, void, void>> {
    using element_type = T;

    inline operator T() const noexcept {
        return typename type<TWrapper>::template is_not_convertible_to<T>{};
    }

    inline operator T() noexcept {
        return typename type<TWrapper>::template is_not_convertible_to<T>{};
    }

    TWrapper wrapper_;
};

BOOST_DI_HAS_METHOD(call, call);

template<template<class> class TConfig, class... TDeps>
class injector : public pool<bindings_t<TDeps...>>
               , public TConfig<injector<TConfig, TDeps...>>
               , _ {
    template<class...> friend struct provider;
    template<class, class, class> friend struct any_type;
    template<class> friend class scopes::exposed;

    using pool_t = pool<bindings_t<TDeps...>>;
    using config = std::conditional_t<
        std::is_default_constructible<TConfig<injector>>{}
      , _
      , TConfig<injector>
    >;

public:
    using deps = bindings_t<TDeps...>;

    template<class... TArgs>
    explicit injector(const init&, const TArgs&... args) noexcept
        : pool_t{init{}, pool<aux::type_list<std::remove_reference_t<decltype(arg(args))>...>>{arg(args)...}}
        , config{*this}
    { }

    template<template<class> class TConfig_, class... TDeps_>
    explicit injector(const injector<TConfig_, TDeps_...>& injector) noexcept
        : pool_t{init{}, create_from_injector(injector, deps{})}
        , config{*this}
    { }

    template<
        class T
      , class TName = no_name
      , class TIsRoot = std::false_type
      , class TDependency = std::remove_reference_t<decltype(binder::resolve<T, TName>((injector*)0))>
      , class TCtor = typename type_traits::ctor_traits<typename TDependency::given>::type
     //, REQUIRES<
          //policy<pool_t>::template call<T, TName, TIsRoot>(core::pool<>{}, std::declval<TDependency>(), TCtor{})
       //> = 0
    > auto create_impl_() const -> std::enable_if_t<std::is_convertible<
       decltype(
           std::declval<TDependency>().template create_<T>(
               provider<
                   typename TDependency::expected
                 , typename TDependency::given
                 , TName
                 , T
                 , TCtor
                 , injector
               >{std::declval<injector>()}
           )
       ), T>{}
    >;

    template<class T, REQUIRES<creatable_<injector, T>()> = 0>
    T create() const {
        using TIsRoot = std::true_type;
        return create_impl<T, no_name, TIsRoot>();
    }

    template<class T, REQUIRES<!creatable_<injector, T>()> = 0>
    [[BOOST_DI_ATTR_ERROR("creatable constraint not satisfied")]]
    T create() {
        using TIsRoot = std::true_type;
        return create_impl<T, no_name, TIsRoot>();
    }

    template<class TAction>
    void call(const TAction& action) {
        call_impl(action, deps{});
    }

private:
    template<class T, class TName = no_name, class TIsRoot = std::false_type>
    auto create_impl() const {
        auto&& dependency = binder::resolve<T, TName>((injector*)this);
        using dependency_t = std::remove_reference_t<decltype(dependency)>;
        using expected_t = typename dependency_t::expected;
        using given_t = typename dependency_t::given;
        using ctor_t = typename type_traits::ctor_traits<given_t>::type;
        using provider_t = provider<expected_t, given_t, TName, T, ctor_t, injector>;
        policy<pool_t>::template call<T, TName, TIsRoot>(((TConfig<injector>&)*this).policies(), dependency, ctor_t{});
        using wrapper_t = decltype(dependency.template create<T>(provider_t{*this}));
        using type = std::conditional_t<
            std::is_reference<T>{} && has_is_ref<dependency_t>{}
          , T
          , std::remove_reference_t<T>
        >;
        return wrapper<type, wrapper_t>{dependency.template create<T>(provider_t{*this})};
    }

    template<class TAction, class... Ts>
    void call_impl(const TAction& action, const aux::type_list<Ts...>&) {
        int _[]{0, (call_impl<Ts>(action), 0)...}; (void)_;
    }

    template<class T, class TAction>
    std::enable_if_t<has_call<T, const TAction&>{}>
    call_impl(const TAction& action) {
        static_cast<T&>(*this).call(action);
    }

    template<class T, class TAction>
    std::enable_if_t<!has_call<T, const TAction&>{}>
    call_impl(const TAction&) { }

    template<class TInjector, class... Ts>
    auto create_from_injector(const TInjector& injector
                            , const aux::type_list<Ts...>&) const noexcept {
        return pool_t{Ts{injector}...};
    }
};

}}} // boost::di::core

#endif

