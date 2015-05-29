//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_POLICY_HPP
#define BOOST_DI_CORE_POLICY_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/core/pool.hpp"

namespace boost { namespace di { inline namespace v1 { namespace core {

BOOST_DI_HAS_METHOD(call_operator, operator());


template<class T>
struct allow_void : T { };

template<>
struct allow_void<void> : std::true_type { };

class policy {
    template<class TArg, class TPolicy, class TPolicies, class TDependency, class TCtor>
    static void call_impl(const TPolicies& policies, TDependency dependency, const TCtor& ctor) noexcept {
        call_impl__<TArg>(static_cast<const TPolicy&>(policies), dependency, ctor);
    }

    template<class TArg, class TPolicy, class TDependency, class TCtor>
    static void call_impl__(const TPolicy& policy, TDependency dependency, const TCtor& ctor) noexcept {
        call_impl_args<TArg>(policy, dependency, ctor);
    }

    template<class TArg, class TDependency, class TPolicy, class TInitialization, class... TCtor
           , BOOST_DI_REQUIRES(!has_call_operator<TPolicy, TArg, TDependency, TCtor...>::value)
    > static void call_impl_args(const TPolicy& policy
                               , TDependency
                               , const aux::pair<TInitialization, aux::type_list<TCtor...>>&) noexcept {
        (policy)(TArg{});
    }

    template<class TArg, class TDependency, class TPolicy, class TInitialization, class... TCtor
           , BOOST_DI_REQUIRES(has_call_operator<TPolicy, TArg, TDependency, TCtor...>::value)>
    static void call_impl_args(const TPolicy& policy
                             , TDependency dependency
                             , const aux::pair<TInitialization, aux::type_list<TCtor...>>&) noexcept {
        (policy)(TArg{}, dependency, aux::type<TCtor>{}...);
    }

    template<class, class, class, class, class = void>
    struct try_call_impl;

    template<class TArg, class TPolicy, class TDependency, class TInitialization, class... TCtor>
    struct try_call_impl<TArg, TPolicy, TDependency, aux::pair<TInitialization, aux::type_list<TCtor...>>
                       , BOOST_DI_REQUIRES_T(!has_call_operator<TPolicy, TArg, TDependency, TCtor...>::value)>
        : allow_void<decltype((std::declval<TPolicy>())(std::declval<TArg>()))>
    { };

    template<class TArg, class TPolicy, class TDependency, class TInitialization, class... TCtor>
    struct try_call_impl<TArg, TPolicy, TDependency, aux::pair<TInitialization, aux::type_list<TCtor...>>
                       , BOOST_DI_REQUIRES_T(has_call_operator<TPolicy, TArg, TDependency, TCtor...>::value)>
        : allow_void<decltype((std::declval<TPolicy>())(std::declval<TArg>(), std::declval<TDependency>(), aux::type<TCtor>{}...))>
    { };

public:
    template<class, class, class, class>
    struct try_call;

    template<class TArg, class TDependency, class TCtor, class... TPolicies>
    struct try_call<TArg, pool_t<TPolicies...>, TDependency, TCtor>
        : std::is_same<
            aux::bool_list<aux::always<TPolicies>::value...>
          , aux::bool_list<try_call_impl<TArg, TPolicies, TDependency, TCtor>::value...>
        >
    { };

    template<class TArg, class TDependency, class TCtor, class... TPolicies>
    static void call(BOOST_DI_UNUSED const pool_t<TPolicies...>& policies
                   , BOOST_DI_UNUSED TDependency dependency
                   , BOOST_DI_UNUSED const TCtor& ctor) noexcept {
        int _[]{0, (call_impl<TArg, TPolicies>(policies, dependency, ctor), 0)...}; (void)_;
    }
};

}}}} // boost::di::v1::core

#endif

