//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONFIG_HPP
#define BOOST_DI_CONFIG_HPP

#include "boost/di/providers/nothrow_reduce_heap_usage.hpp"

namespace boost { namespace di {

template<class T, class TDependency, class TDeps>
struct data {
    using type = T;

    TDependency& dep;
    TDeps& deps;

    template<class U, class TDefault>
    decltype(auto) resolve() const noexcept {
        return core::binder::template resolve<U, TDefault>(&deps);
    }
};

template<class T, class TDependency, class TDeps, class TPolicy>
inline void call_policies_impl(TDependency& dependency
                             , TDeps& deps
                             , const TPolicy& policy) noexcept {
    policy(data<T, TDependency, TDeps>{dependency, deps});
}

template<class T, class TDependency, class TDeps, class... TPolicies>
inline void call_policies(TDependency& dependency
                        , TDeps& deps
                        , const TPolicies&... policies) noexcept {
    void(call_policies_impl<T>(dependency, deps, policies)...);
}

#if defined(BOOST_DI_CFG_CUSTOM_POLICIES)
    template<class T, class TDependency, class TDeps>
    void custom_policies(TDependency&, TDeps&) noexcept;
#endif

#if defined(BOOST_DI_CFG_CUSTOM_PROVIDER)
    BOOST_DI_CFG_CUSTOM_PROVIDER custom_provider() noexcept;
#else
    auto custom_provider() noexcept {
        return providers::nothrow_reduce_heap_usage{};
    }
#endif

}} // boost::di

#endif

