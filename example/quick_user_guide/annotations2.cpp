//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <string>
#include <boost/di.hpp>

namespace di = boost::di;

auto n1 = [] {};
auto n2 = [] {};

struct c {
  BOOST_DI_INJECT(c, (named = n1) int i1, (named = n1) int i2, (named = n2) int i3, int i4, (named = n1)std::string s)
      : i1(i1), i2(i2), i3(i3), i4(i4), s(s) {}

  int i1 = 0;
  int i2 = 0;
  int i3 = 0;
  int i4 = 0;
  std::string s;
};

int main() {
  auto injector = di::make_injector(di::bind<int>().named(n1).to(42), di::bind<int>().named(n2).to(87),
                                    di::bind<std::string>().named(n1).to("str"));

  auto object = injector.create<c>();
  assert(42 == object.i1);
  assert(42 == object.i2);
  assert(87 == object.i3);
  assert(0 == object.i4);
  assert("str" == object.s);
}
