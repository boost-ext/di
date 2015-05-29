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

class policy {
    template<class TArg, class TPolicy, class TPolicies, class TDependency, class TCtor>
    static auto call_impl(const TPolicies& policies, TDependency dependency, const TCtor& ctor) noexcept {
        return call_impl_type<TArg>(static_cast<const TPolicy&>(policies), dependency, ctor);
    }

    template<class TArg, class TPolicy, class TDependency, class TCtor>
    static auto call_impl_type(const TPolicy& policy, TDependency dependency, const TCtor& ctor) noexcept {
        call_impl_args<TArg>(policy, dependency, ctor);
        using type = decltype(call_impl_args<TArg>(policy, dependency, ctor));
        return std::conditional_t<std::is_same<type, void>::value, std::true_type, type>{};
    }

    template<class TArg, class TDependency, class TPolicy, class TInitialization, class... TCtor
           , BOOST_DI_REQUIRES(!has_call_operator<TPolicy, TArg, TDependency, TCtor...>::value)
    > static auto call_impl_args(const TPolicy& policy, TDependency, const aux::pair<TInitialization, aux::type_list<TCtor...>>&) noexcept {
        return (policy)(TArg{});
    }

    template<class TArg, class TDependency, class TPolicy, class TInitialization, class... TCtor
           , BOOST_DI_REQUIRES(has_call_operator<TPolicy, TArg, TDependency, TCtor...>::value)>
    static auto call_impl_args(const TPolicy& policy, TDependency dependency, const aux::pair<TInitialization, aux::type_list<TCtor...>>&) noexcept {
        return (policy)(TArg{}, dependency, aux::type<TCtor>{}...);
    }

public:
    template<class TArg, class TDependency, class TCtor, class... TPolicies>
    static auto call(BOOST_DI_UNUSED const pool<aux::type_list<TPolicies...>>& policies
                   , BOOST_DI_UNUSED TDependency dependency
                   , const TCtor& ctor) noexcept
   -> std::is_same<
            aux::bool_list<aux::always<TPolicies>::value...>
          , aux::bool_list<decltype(call_impl<TArg, TPolicies>(policies, dependency, ctor))::value...>
        >
    {
        int _[]{0, (call_impl<TArg, TPolicies>(policies, dependency, ctor), 0)...}; (void)_;
        return {};
    }
};

}}}} // boost::di::v1::core

#endif

