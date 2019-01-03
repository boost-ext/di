//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <boost/di.hpp>
#include <cassert>
#include <iostream>
#include <typeinfo>

namespace di = boost::di;
//->

class print_types_policy : public di::config {
 public:
  static auto policies(...) noexcept {
    return di::make_policies([](auto type) {
      using arg = typename decltype(type)::type;
      std::cout << typeid(arg).name() << std::endl;
    });
  }
};

int main() {
  auto injector = di::make_injector<print_types_policy>();
  injector.create<int>();  // output: int
}
