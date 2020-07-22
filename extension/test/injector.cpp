//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/extension/injector.hpp"

#include <cassert>
#include <memory>

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
  explicit impl1(std::unique_ptr<i2> sp2) { assert(sp2->bar() == 99); }
  int foo() const override { return 42; }
};

struct app {
  explicit app(std::shared_ptr<i1> sp1) { assert(sp1->foo() == 42); }
};

int main() {
  // clang-format off
  namespace di = boost::di;

  /*<<creates generic injector>>*/
  {
    auto injector = di::make_injector(
      di::bind<i1>().to<impl1>(), /// missing binding -> compile error
      di::bind<i2>().to<impl2>()
    );
    di::create<app>(injector);
    di::create<std::shared_ptr<i1>>(injector); /// okay
  }

  /*<<creates exposed injector>>*/
  {
    di::extension::injector<app> injector = di::make_injector(
      di::bind<i1>().to<impl1>(), /// missing binding -> compile error
      di::bind<i2>().to<impl2>()
    );
    di::create<app>(injector);
    //di::create<std::shared_ptr<i1>>(injector); /// compile error, i1 is not exposed
  }

  /*<<creates runtime injector>>*/
  {
    di::extension::injector<> injector{};
    injector.install(di::bind<i1>().to<impl1>()); /// missing binding -> runtime error (Error policy)
    injector.install(di::bind<i2>().to<impl2>());

    di::create<app>(injector);
    di::create<std::shared_ptr<i1>>(injector); /// okay
  }

  // clang-format on
}
