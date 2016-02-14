//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <boost/di.hpp>

namespace di = boost::di;
//->

class my_provider : public di::config {
 public:
  // this is default in di::config
  static auto provider(...) noexcept { return providers::stack_over_heap{}; }
};

int main() {
  auto injector = di::make_injector<my_provider>();
  injector.create<int>();                           // stack
  di::aux::owner<int> i = injector.create<int*>();  // heap
  delete i;
}
