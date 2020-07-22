//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <boost/di.hpp>
#include <cassert>
#include <memory>

namespace di = boost::di;

struct i1 {
  virtual ~i1() noexcept = default;
  virtual void dummy1() = 0;
};
struct i2 {
  virtual ~i2() noexcept = default;
  virtual void dummy2() = 0;
};
struct impl1 : i1 {
  void dummy1() override {}
};
struct impl2 : i2 {
  void dummy2() override {}
};
//->

struct T {
  T(std::unique_ptr<i1> i1, std::unique_ptr<i2> i2, int i) : i1_(std::move(i1)), i2_(std::move(i2)), i(i) {}

  std::unique_ptr<i1> i1_;
  std::unique_ptr<i2> i2_;
  int i = 0;
};

// clang-format off
auto module1 = [] {
  return di::make_injector(
    di::bind<i1>().to<impl1>()
  , di::bind<int>().to(42)
  );
};

auto module2 = [] {
  return di::make_injector(
    di::bind<i2>().to<impl2>()
  );
};
// clang-format on

int main() {
  auto injector = di::make_injector(module1(), module2());
  auto object = injector.create<std::unique_ptr<T>>();
  assert(dynamic_cast<impl1*>(object->i1_.get()));
  assert(dynamic_cast<impl2*>(object->i2_.get()));
  assert(42 == object->i);
  auto up1 = injector.create<std::unique_ptr<i1>>();
  assert(dynamic_cast<impl1*>(up1.get()));
  auto up2 = injector.create<std::unique_ptr<i2>>();
  assert(dynamic_cast<impl2*>(up2.get()));
}
