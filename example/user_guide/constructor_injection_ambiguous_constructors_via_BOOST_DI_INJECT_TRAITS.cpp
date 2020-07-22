//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <boost/di.hpp>
#include <cassert>

namespace di = boost::di;
//->

struct T {
  BOOST_DI_INJECT_TRAITS(int, double);
  T(double d, int a) : a(a), d(d) {}
  T(int a, double d) : a(a), d(d) {}

  int a = 0;
  double d = 0.0;
};

int main() {
  // clang-format off
  auto injector = di::make_injector(
    di::bind<int>().to(42)
  , di::bind<double>().to(87.0)
  );
  // clang-format on
  auto object = injector.create<T>();
  assert(42 == object.a);
  assert(87.0 == object.d);
}
