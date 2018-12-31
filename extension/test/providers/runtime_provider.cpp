//
// Copyright (c) 2012-2018 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/extension/providers/runtime_provider.hpp"

#include <cassert>

namespace di = boost::di;

struct i1 {
  virtual ~i1() noexcept = default;
  virtual int foo() const = 0;
};

struct i2 {
  virtual ~i2() noexcept = default;
  virtual int bar() const = 0;
};

struct impl2 : i2 {
  int bar() const override { return 99; }
};

struct impl1 : i1 {
  impl1(std::unique_ptr<i2> sp2) { assert(sp2->bar() == 99); }

  int foo() const override { return 42; }
};

struct example {
  example(std::shared_ptr<i1> sp, int i) {
    assert(dynamic_cast<impl1*>(sp.get()));
    assert(sp->foo() == 42);
    assert(i == 87);
  }
};

int main() {
  // clang-format off
  namespace di = boost::di;

  /*<<create runtime injector>>*/
  di::extension::injector injector{};

  /*<<install bindings>>*/
  injector.install(
    di::bind<i1>().to<impl1>(),
    di::bind<i2>().to<impl2>()
  );

  /*<<more bindings>>*/
  injector.install(di::bind<int>().to(87));

  /*<<create example>>*/
  injector.create<example>();

  // clang-format on
}
