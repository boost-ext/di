//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_DEFAULTS_HPP
#define BOOST_DI_DEFAULTS_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/providers/nothrow_reduce_heap_usage.hpp"

namespace boost { namespace di {

template<class... TArgs>
inline auto make_policies(const TArgs&... args) noexcept {
    return core::pool<aux::type_list<TArgs...>>(args...);
}

struct project_scope { };
struct global_scope { };

template<class = global_scope>
struct injector_defaults {
	static auto policies() noexcept {
        return make_policies();
	}

	static auto provider() noexcept {
        return providers::nothrow_reduce_heap_usage{};
	}
};

}} // namespace boost::di

#endif

