//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <boost/di.hpp>
#include <cassert>
#include <type_traits>

namespace di = boost::di;
//->

int main() {
  // clang-format off
  auto injector = di::make_injector(
    di::bind<int>().to<std::integral_constant<int, 42>>()
  );
  // clang-format on

  assert(42 == injector.create<int>());
}
