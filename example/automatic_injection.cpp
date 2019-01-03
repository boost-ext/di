//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
//->
#include <boost/di.hpp>

namespace di = boost::di;

/*<<ambiguous constructor with 2 parameters>>*/
struct example1 {
  /*<<ambiguous `example1` constructor taking 2 parameters>>*/
  example1(double, int) { assert(false); }

  /*<<ambiguous `example1` constructor taking 2 parameters - marked to be injected by `...`>>*/
  example1(int i, double d, ...) {
    assert(i == 42);
    assert(d == 87.0);
  }
};

struct example2 {
  /*<<constructor with 1 parameter marked to be injected by `...`>>*/
  explicit example2(int i, ...) { assert(i == 42); }

  /*<<constructor with 3 parameters, but still constructor with 1 and `...` will be chosen>>*/
  example2(int, double, float) { assert(false); }
};

int main() {
  /*<<make injector and bind named parameters>>*/
  // clang-format off
  auto injector = di::make_injector(
    di::bind<int>().to(42)
  , di::bind<double>().to(87.0)
  );
  // clang-format on

  /*<<create `example1`>>*/
  injector.create<example1>();

  /*<<create `example2`>>*/
  injector.create<example2>();
}
