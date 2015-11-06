//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <typeinfo>
#include <cassert>
#include <boost/di.hpp>

namespace di = boost::di;

class print_types_policy : public di::config {
 public:
  template <class T>
  static auto policies(const T&) noexcept {
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
