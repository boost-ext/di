//
// Copyright (c) 2012-2017 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_CONFIG_HPP
#define BOOST_DI_FAKE_CONFIG_HPP

#include "boost/di/core/pool.hpp"
#include "boost/di/providers/heap.hpp"

template <class = void>
struct fake_config {
  static auto policies(...) noexcept { return core::pool<>{}; }
  static auto provider(...) noexcept { return providers::heap{}; }
};

#endif
