//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_CONFIG_HPP
#define BOOST_DI_FAKE_CONFIG_HPP

#include "boost/di/config.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/providers/stack_over_heap.hpp"

template <class T = void>
struct fake_config : BOOST_DI_NAMESPACE::config {
  auto policies(...) noexcept { return core::pool<>{}; }
  auto provider(...) noexcept { return providers::stack_over_heap{}; }
};

#endif
