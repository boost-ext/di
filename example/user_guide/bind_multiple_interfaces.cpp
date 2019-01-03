//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
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
struct impl : i1, i2 {
  void dummy1() override {}
  void dummy2() override {}
};
//->

int main() {
  // clang-format off
  auto injector = di::make_injector(
    di::bind<i1, i2>().to<impl>()
  );
  // clang-format on

  auto object1 = injector.create<std::shared_ptr<i1>>();
  auto object2 = injector.create<std::shared_ptr<i2>>();
  assert(dynamic_cast<impl*>(object1.get()));
  assert(dynamic_cast<impl*>(object2.get()));
}
