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
struct impl1 : i1 {
  void dummy1() override {}
};
//->

auto my_name = [] {};

struct T {
  BOOST_DI_INJECT(T, (named = my_name) std::unique_ptr<i1> up) : up(std::move(up)) {}
  std::unique_ptr<i1> up;
};

auto module = []() -> di::injector<BOOST_DI_EXPOSE((named = my_name) std::unique_ptr<i1>)> {
  // clang-format off
  return di::make_injector(
    di::bind<i1>().named(my_name).to<impl1>()
  );
  // clang-format on
};

int main() {
  auto injector = di::make_injector(module());
  auto object = injector.create<std::unique_ptr<T>>();
  assert(dynamic_cast<impl1*>(object->up.get()));
}
