//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>
#include "boost/di/config.hpp"

struct policy1 {
  template <class T>
  void operator()(T) const;
};

struct policy2 {
  template <class T>
  void operator()(T) const;
};

test make_policies_types = [] {
  expect(std::is_same<core::pool<aux::type_list<>>, decltype(make_policies())>{});
  expect(std::is_same<core::pool<aux::type_list<policy1, policy2>>, decltype(make_policies(policy1{}, policy2{}))>{});
};

test default_config = [] {
  expect(std::is_same<providers::stack_over_heap, decltype(config::provider(0))>{});
  expect(std::is_same<core::pool<aux::type_list<>>, decltype(config::policies(0))>{});
};
