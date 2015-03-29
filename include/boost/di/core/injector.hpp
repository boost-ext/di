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
        std::is_constructible<TConfig, injector>{}
      , config_t
      , _
    >;

public:
    using deps = transform_t<TDeps...>;

    template<class... TArgs>
    explicit injector(const init&, const TArgs&... args) noexcept
        : pool_t{init{}, pool<aux::type_list<
              std::remove_reference_t<decltype(arg(args, has_configure<decltype(args)>{}))>...>>{
                  arg(args, has_configure<decltype(args)>{})...}}
        , config{*this}
    { }

    template<class TConfig_, class... TDeps_>
    explicit injector(const injector<TConfig_, TDeps_...>& injector) noexcept
        : pool_t{init{}, create_from_injector(injector, deps{})}
        , config{*this}
    { }

    template<class T, REQUIRES<concepts::creatable_<injector, T, no_name, is_root_t>()> = 0>
    T create() const {
        return create_impl<T, no_name, is_root_t>();
    }

    template<class T, REQUIRES<!concepts::creatable_<injector, T, no_name, is_root_t>()> = 0>
    BOOST_DI_ATTR_ERROR("creatable constraint not satisfied")
    T create() const {
        return create_impl<T, no_name, is_root_t>();
    }

    template<class TAction>
    void call(const TAction& action) {
        call_impl(action, deps{});
    }

    template<
        class T
      , class TName = no_name
      , class TIsRoot = std::false_type
      , class TDependency = std::remove_reference_t<decltype(binder::resolve<T, TName>((injector*)0))>
      , class TCtor = typename type_traits::ctor_traits<typename TDependency::given>::type
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
       ), T>{} && decltype(policy<pool_t>::template
           call<T, TName, TIsRoot>(((TConfig&)*this).policies(), std::declval<TDependency>(), TCtor{})){}
    >;

private:
    template<class T, class TName = no_name, class TIsRoot = std::false_type>
    auto create_impl() const {
        auto&& dependency = binder::resolve<T, TName>((injector*)this);
        using dependency_t = std::remove_reference_t<decltype(dependency)>;
        using expected_t = typename dependency_t::expected;
        using given_t = typename dependency_t::given;
        using ctor_t = typename type_traits::ctor_traits<given_t>::type;
        using provider_t = provider<expected_t, given_t, TName, T, ctor_t, injector>;
        policy<pool_t>::template call<T, TName, TIsRoot>(((TConfig&)*this).policies(), dependency, ctor_t{});
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
        int _[]{0, (call_impl<Ts>(action, has_call<Ts, const TAction&>{}), 0)...}; (void)_;
    }

    template<class T, class TAction>
    void call_impl(const TAction& action, const std::true_type&) {
        static_cast<T&>(*this).call(action);
    }

    template<class, class TAction>
    void call_impl(const TAction&, const std::false_type&) { }

    template<class TInjector, class... Ts>
    auto create_from_injector(const TInjector& injector
                            , const aux::type_list<Ts...>&) const noexcept {
        return pool_t{Ts{injector}...};
    }

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

