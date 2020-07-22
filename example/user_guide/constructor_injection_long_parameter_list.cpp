//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <boost/di.hpp>

namespace di = boost::di;
//->

struct T {
  // clang-format off
  using boost_di_inject__ = di::inject<
    int, int, int, int, int, int, int, int, int, int
  , int, int, int, int, int, int, int, int, int, int
  , int, int, int, int, int, int, int, int, int, int>;

  T(int, int, int, int, int, int, int, int, int, int
  , int, int, int, int, int, int, int, int, int, int
  , int, int, int, int, int, int, int, int, int, int) { }
  // clang-format on
};

int main() {
  auto injector = di::make_injector();
  injector.create<T>();  // compile clean
}
