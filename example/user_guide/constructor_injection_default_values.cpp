//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
#include <boost/di.hpp>

namespace di = boost::di;
//->

class example {
 public:
  BOOST_DI_INJECT_TRAITS(int);
  explicit example(int i, double d = 42.0) : i(i), d(d) {}

  int i = 0;
  double d = 0.0;
};

int main() {
  auto injector = di::make_injector();
  auto object = injector.create<example>();
  assert(0 == object.i);
  assert(42.0 == object.d);
}
