//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>

namespace di = boost::di;

auto module = [] {
  // clang-format off
  return di::make_injector(
    di::bind<>.to(42)
  );
  // clang-format on
};

auto test() {
  auto injector = di::make_injector(module());
  return injector.create<int>();
}

//<-
int main() {}
/**
 * ASM x86-64 (same as `return 42`)
 *
 * xor 0x2a,%eax
 * retq
 */
//->
