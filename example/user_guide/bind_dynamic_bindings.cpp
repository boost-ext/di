//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <boost/di.hpp>
#include <cassert>

namespace di = boost::di;

struct i1 {
  virtual ~i1() noexcept = default;
  virtual void dummy1() = 0;
};
struct impl : i1 {
  void dummy1() override {}
};
struct impl1 : i1 {
  void dummy1() override {}
};
//->

int main() {
  // clang-format off
  auto injector = di::make_injector(
    di::bind<i1>().to([&](const auto& injector) -> i1& {
      if (true)
        return injector.template create<impl1&>();
      else
        return injector.template create<impl&>();
    })
  );
  // clang-format on

  auto&& object = injector.create<i1&>();
  assert(dynamic_cast<impl1*>(&object));
}
