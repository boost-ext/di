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

int main() {
  // clang-format off
  auto injector = di::make_injector(
    di::bind<>().to(42)
  );
  // clang-format on

  assert(42 == injector.create<int>());
}
