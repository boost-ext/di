//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#if !defined(_MSC_VER)
//->

#include "boost/di/extension/injections/named_parameters.hpp"

#include <cassert>
#include <memory>

namespace di = boost::di;

struct interface {
  virtual ~interface() = default;
  virtual void dummy() = 0;
};

struct implementation : interface {
  void dummy() override {}
};

struct example {
  /*<<inject constructor using automatic named parameters>>*/
  $inject(example, int i, std::unique_ptr<interface> up, [[named("my_value")]] int value) {
    assert(i == 42);
    assert(dynamic_cast<implementation*>(up.get()));
    assert(value == 87);
  }
};

int main() {
  using namespace di::extension;

  // clang-format off
  /*<<bind named parameters>>*/
  auto injector = di::make_injector(
      di::bind<int>.to(42)
    , di::bind<interface>.to<implementation>()
    , di::bind<int>.named("my_value"_s).to(87)
  );
  // clang-format on

  injector.create<example>();
}

//<-
#else
int main() {}
#endif
//->
