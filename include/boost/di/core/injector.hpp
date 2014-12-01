//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_INJECTOR_HPP
#define BOOST_DI_CORE_INJECTOR_HPP

#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/any_type.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/providers/reduce_allocs.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/wrappers/universal.hpp"

namespace boost { namespace di { namespace core {

BOOST_DI_HAS_METHOD(configure, configure);
BOOST_DI_HAS_METHOD(call, call);

template<
    class TDeps = aux::type_list<>
  , class TDefaultProvider = providers::reduce_allocs
>
class injector : public pool<TDeps> {
    template<class, class, class, class>
    friend struct any_type;

    using pool_t = pool<TDeps>;

    template<class T, class TDependency>
    struct data {
        using type = T;
        using dependency = TDependency;
        using binder = core::binder;
    };

    template<class...>
    struct provider_impl;

    template<
        class T
      , class TDependency
      , class TProvider
      , class TPolicies
      , class... TArgs
    >
    struct provider_impl<
        T
      , TDependency
      , TProvider
      , TPolicies
      , aux::type_list<TArgs...>
    > {
        const injector& injector_;
        const TProvider& provider_;
        const TPolicies& policies_;

        decltype(auto) get() const noexcept {
            return provider_.template get<TDependency>(
                injector_.create_impl<TArgs, T>(provider_, policies_)...
            );
        }

        decltype(auto) get_ptr() const noexcept {
            return provider_.template get_ptr<TDependency>(
                injector_.create_impl<TArgs, T>(provider_, policies_)...
            );
        }
    };

public:
    using deps = TDeps;

    // bind<...>, etc.  -> ignore
    // injector<...>    -> get all dependencies from the injector
    // dependency<...>  -> pass

    template<class... TArgs>
    explicit injector(const TArgs&... args) noexcept
        : injector(init{}, pass_arg(args)...)
    { }

    template<class T>
    injector(const injector<T>& injector) noexcept // non explicit
        : pool_t{init{}, create_from_injector(injector, TDeps{})}
    { }

    template<class T, class... TArgs>
    T create(const TArgs&... args) const noexcept {
        pool<aux::type_list<TArgs...>> policies(args...);
        return create_impl<T, aux::none_t>(TDefaultProvider{}, policies);
    }

    template<class T, class TProvider, class... TArgs>
    T provide(const TProvider& provider, const TArgs&... args) const noexcept {
        pool<aux::type_list<TArgs...>> policies(args...);
        return create_impl<T, aux::none_t>(provider, policies);
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

    template<
        class T
      , class TParent
      , class TProvider
      , class TPolicies
    >
    decltype(auto)
    create_impl(const TProvider& provider
              , const TPolicies& policies
              , std::enable_if_t<is_any_type<T>{}>* = 0) const noexcept {
        return any_type<TParent, injector, TProvider, TPolicies>{*this, provider, policies};
    }

    template<
        class T
      , class TParent
      , class TProvider
      , class TPolicies
    >
    decltype(auto)
    create_impl(const TProvider& provider
              , const TPolicies& policies
              , std::enable_if_t<!is_any_type<T>{}>* = 0) const noexcept {
        return create_from_dep_impl<T>(provider, policies);
    }

    template<
        class T
      , class TProvider
      , class TPolicies
    >
    decltype(auto)
    create_from_dep_impl(const TProvider& provider
                       , const TPolicies& policies) const noexcept {
        auto&& dependency = binder::resolve<T>((injector*)this);
        using type = typename std::remove_reference_t<decltype(dependency)>::given;
        using ctor = typename type_traits::ctor_traits<type>::type;
        call_policies<data<T, type>>(policies);
        using provider_impl_type = provider_impl<T, type, TProvider, TPolicies, ctor>;
        auto&& ctor_provider = provider_impl_type{*this, provider, policies};
        using wrapper = decltype(dependency.template create<T>(ctor_provider));
        return wrappers::universal<T, wrapper>{dependency.template create<T>(ctor_provider)};
    }

    template<class T, class... TPolicies>
    void call_policies(const pool<aux::type_list<TPolicies...>>& policies) const noexcept {
        void(call_policies_impl<TPolicies, T>(policies)...);
    }

    template<class TPolicy, class T, class TPolicies>
    void call_policies_impl(const TPolicies& policies) const noexcept {
        (static_cast<const TPolicy&>(policies))(T{});
    }

    template<class TAction, class... Ts>
    void call_impl(const TAction& action, const aux::type_list<Ts...>&) noexcept {
        void(call_impl<Ts>(action)...);
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
    decltype(auto)
    pass_arg(const T& arg
           , std::enable_if_t<!has_configure<T>{}>* = 0) const noexcept {
        return arg;
    }

    template<class T>
    decltype(auto)
    pass_arg(const T& arg
           , std::enable_if_t<has_configure<T>{}>* = 0) const noexcept {
        return arg.configure();
    }

    template<class T, class... Ts>
    decltype(auto)
    create_from_injector(const injector<T>& injector
                       , const aux::type_list<Ts...>&) const noexcept {
        return pool<TDeps>(create_dep<Ts>(injector)...);
    }

    template<class TDependency, class TInjector>
    decltype(auto)
    create_dep(const TInjector& injector) const noexcept {
        return TDependency{injector};
    }
};

}}} // namespace boost::di::core

#endif

