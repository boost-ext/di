//
// Copyright (c) 2012-2018 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
//->
#include <boost/di.hpp>

namespace di = boost::di;

struct example {
  example(int, double, float) {}            // don't pick this one
  example(int i, double d) : d{d}, i{i} {}  // but pick this one

  double d{};
  int i{};
};

struct app {
  example& ex;
};

int main() {
  // clang-format off
  const auto injector = di::make_injector(
    di::bind<example>().to<example>(42, 87.0) // or di::bind<example>(42, 87.0)
  );
  // clang-format on

  auto object = di::create<app>(injector);

  assert(42 == object.ex.i);
  assert(87.0 == object.ex.d);
}
