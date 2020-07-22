//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <boost/di.hpp>
#include <cassert>
#include <functional>
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
  T(const std::shared_ptr<i1>& sp, std::unique_ptr<i2> up, int& i, double d) : sp(sp), up(std::move(up)), i(i), d(d) {}

  std::shared_ptr<i1> sp; /*singleton*/
  std::unique_ptr<i2> up; /*unique*/
  int& i;                 /*instance*/
  double d;               /*unique*/
};

int main() {
  auto i = 42;
  // clang-format off
  auto injector = di::make_injector(
    di::bind<i1>().to<impl1>()
  , di::bind<i2>().to<impl2>()
  , di::bind<int>().to(i)
  , di::bind<double>().to(87.0)
  );
  // clang-format on
  auto object1 = injector.create<std::unique_ptr<T>>();
  auto object2 = injector.create<std::unique_ptr<T>>();
  assert(object1->sp == object2->sp);
  assert(object1->up != object2->up);
  assert(42 == object1->i);
  assert(&i == &object1->i);
  assert(42 == object2->i);
  assert(&i == &object2->i);
  assert(87.0 == object1->d);
  assert(87.0 == object2->d);
  i = 12;
  assert(i == object2->i);
  assert(&i == &object2->i);
}
