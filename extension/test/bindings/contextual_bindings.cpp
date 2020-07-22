//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/extension/bindings/contextual_bindings.hpp"

#include <cassert>

namespace di = boost::di;

struct more_data {
  int i;
};

struct data {
  int i;
  more_data md;
};

class example {
 public:
  example(data& d, int i, float f) {
    assert(87 == d.i);
    assert(99 == d.md.i);
    assert(42 == i);
    assert(123.f == f);
  }
};

int main() {
  // clang-format off
  auto injector = di::make_injector<di::extension::contextual_bindings>(
      di::bind<>().to(123.f)
    , di::bind<int>().to([](const auto& injector) -> int{
        if (di::extension::context(injector) == "example->data") return 87;
        if (di::extension::context(injector) == "example->data->more_data") return 99;
        return 42;
      })
  );
  // clang-format on

  /*<<create `example`>>*/
  injector.create<example>();
}
