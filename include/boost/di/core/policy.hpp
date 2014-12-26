//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_POLICY_HPP
#define BOOST_DI_CORE_POLICY_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/core/binder.hpp"

namespace boost { namespace di { namespace core {

BOOST_DI_HAS_METHOD(call_operator, operator());

template<class TDeps>
class policy {
    template<
        class TPolicy
      , class T
      , class TName
      , class TPolicies
      , class TDependency
      , class... TArgs
    > static void call_impl(const TPolicies& policies, TDependency&& dependency) noexcept {
        struct arg {
            using type = T;
            using name = TName;
            template<class T_, class TName_, class TDefault_>
            using resolve = decltype(binder::resolve<T_, TName_, TDefault_>((TDeps*)nullptr));
        };

        call_impl_args<arg, TDependency, TPolicy, TArgs...>(
            static_cast<const TPolicy&>(policies), dependency
        );
    }

    template<class TArg, class TDependency, class TPolicy, class... TArgs>
    static std::enable_if_t<has_call_operator<TPolicy, TArg>{}>
    call_impl_args(const TPolicy& policy, TDependency&&) noexcept {
        (policy)(TArg{});
    }

    template<class TArg, class TDependency, class TPolicy, class... TArgs>
    static std::enable_if_t<has_call_operator<TPolicy, TArg, TDependency, TArgs...>{}>
    call_impl_args(const TPolicy& policy, TDependency&& dependency) noexcept {
        (policy)(TArg{}, dependency, aux::type<TArgs>{}...);
    }

public:
    template<
        class T
      , class TName
      , class TInitialization
      , class TDependency
      , class... TArgs
      , class... TPolicies
    > static void call(const pool<aux::type_list<TPolicies...>>& policies
                     , TDependency&& dependency
                     , aux::pair<TInitialization, aux::type_list<TArgs...>>) noexcept {
        int _[]{0, (call_impl<TPolicies, T, TName, TPolicies, TDependency, TArgs...>(
            policies, dependency), 0)...}; (void)_;
    }
};

}}} // boost::di::core

#endif

