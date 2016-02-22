//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <fruit/fruit.h>

auto i = 42;  // bindInstance doesn't support l-value's

fruit::Component<int> module() {
  // clang-format off
  return fruit::createComponent().bindInstance(i);
  // clang-format on
}

auto test() {
  fruit::Injector<int> injector{module()};
  return injector.get<int>();
}

//<-
int main() {}
/**
 * ASM x86-64
 *
 */
//->
