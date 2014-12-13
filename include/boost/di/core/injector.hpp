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
#include "boost/di/core/provider.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/wrappers/universal.hpp"

namespace boost { namespace di { namespace core {

BOOST_DI_HAS_METHOD(configure, configure);
BOOST_DI_HAS_METHOD(call, call);

template<class TDeps, class TConfig>
class injector : public pool<TDeps> {
    template<class, class> friend struct any_type;
    template<class...> friend struct provider;

    using pool_t = pool<TDeps>;

    template<class T, class TDependency>
    struct data {
        using type = T;

        TDependency&& dep;
        pool_t& deps;

        template<class U, class TDefault>
        decltype(auto) resolve() const noexcept {
            return binder::template resolve<U, TDefault>(&deps);
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

    template<class... TArgs>
    injector(const injector<TArgs...>& injector) noexcept // non explicit
        : pool_t{init{}, create_from_injector(injector, TDeps{})}
    { }

    template<class T>
    T create() const noexcept {
        return create<T, aux::none_t>();
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

    template<class T, class TParent>
    decltype(auto) create(std::enable_if_t<is_any_type<T>{}>* = 0) const noexcept {
        return any_type<TParent, injector>{*this};
    }

    template<class T, class TParent>
    decltype(auto) create(std::enable_if_t<!is_any_type<T>{}>* = 0) const noexcept {
        return create_impl<T>();
    }

    template<class T>
    decltype(auto) create_impl() const noexcept {
        auto&& dependency = binder::resolve<T>((injector*)this);
        using dependency_t = typename std::remove_reference_t<decltype(dependency)>;
        using given_t = typename dependency_t::given;
        using ctor_t = typename type_traits::ctor_traits<given_t>::type;
        using provider_type = provider<given_t, T, ctor_t, injector>;
        const auto& ctor_provider = provider_type{*this};
        using wrapper_t = decltype(dependency.template create<T>(ctor_provider));
        using type = std::conditional_t<
            std::is_reference<T>{} && has_is_ref<dependency_t>{}
          , T
          , std::remove_reference_t<T>
        >;
        call_policies<T>(config_.policies(), dependency);
        return wrappers::universal<type, wrapper_t>{
            dependency.template create<T>(ctor_provider)
        };
    }

    template<
        class T
      , class TDependency
      , class... TPolicies
    > void call_policies(const pool<aux::type_list<TPolicies...>>& policies
                       , TDependency&& dependency) const noexcept {
        void(call_policies_impl<TPolicies, T>(
            policies, std::forward<TDependency>(dependency))...
        );
    }

    template<
        class TPolicy
      , class T
      , class TPolicies
      , class TDependency
    > void call_policies_impl(const TPolicies& policies
                            , TDependency&& dependency) const noexcept {
        auto&& injections = data<T, TDependency>{
            std::forward<TDependency>(dependency)
          , (injector&)*this
        };
        (static_cast<const TPolicy&>(policies))(injections);
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
    decltype(auto) pass_arg(const T& arg
                          , std::enable_if_t<!has_configure<T>{}>* = 0) const noexcept {
        return arg;
    }

    template<class T>
    decltype(auto) pass_arg(const T& arg
                          , std::enable_if_t<has_configure<T>{}>* = 0) const noexcept {
        return arg.configure();
    }

    template<class... TArgs, class... Ts>
    decltype(auto)
    create_from_injector(const injector<TArgs...>& injector
                       , const aux::type_list<Ts...>&) const noexcept {
        return pool<TDeps>(create_dep<Ts>(injector)...);
    }

    template<class TDependency, class TInjector>
    decltype(auto) create_dep(const TInjector& injector) const noexcept {
        return TDependency{injector};
    }

    TConfig config_;
};

}}} // boost::di::core

#endif

