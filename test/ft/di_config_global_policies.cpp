//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#define BOOST_DI_CFG_FWD class global_policies;
#define BOOST_DI_CFG global_policies
#include <boost/di.hpp>

namespace di = boost::di;

auto custom_called = false;
auto global_called = false;

struct custom_policy {
  template <class T>
  void operator()(const T&) const noexcept {
    custom_called = true;
  }
};

struct global_policy {
  template <class T>
  void operator()(const T&) const noexcept {
    global_called = true;
  }
};

class custom_policies : public di::config {
 public:
  static auto policies(...) noexcept { return di::make_policies(custom_policy{}); }
};

class global_policies : public di::config {
 public:
  static auto policies(...) noexcept { return di::make_policies(global_policy{}); }
};

test call_policies_via_global_config = [] {
  global_called = 0;
  auto injector = di::make_injector();
  injector.create<int>();
  expect(1 == global_called);
};

test call_custom_policies_although_global_config = [] {
  custom_called = 0;
  auto injector = di::make_injector<custom_policies>();
  injector.create<int>();
  expect(1 == custom_called);
};
