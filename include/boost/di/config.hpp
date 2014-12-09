//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONFIG_HPP
#define BOOST_DI_CONFIG_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/providers/nothrow_reduce_heap_usage.hpp"

namespace boost { namespace di {

struct project_scope { };

template<class... TArgs>
inline auto make_policies(const TArgs&... args) noexcept {
    return core::pool<aux::type_list<TArgs...>>(args...);
}

}} // boost::di

#if !defined(BOOST_DI_POLICIES)
    #define BOOST_DI_POLICIES boost_di_policies__
#endif

inline auto BOOST_DI_POLICIES(...) noexcept {
    return boost::di::make_policies();
}

#if !defined(BOOST_DI_PROVIDER)
    #define BOOST_DI_PROVIDER boost_di_provider__
#endif

inline auto BOOST_DI_PROVIDER(...) noexcept {
    return boost::di::providers::nothrow_reduce_heap_usage{};
}

#endif

