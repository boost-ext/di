//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <di/registry.hpp>
#include <di/injector.hpp>

void module(di::registry& r) {
  // clang-format off
  r.add(r.type<int>().instance(new int{42}));
  // clang-format on
}

int main() {
  di::injector injector;
  injector.install(module);
  return injector.construct<int>() != 42;
}
