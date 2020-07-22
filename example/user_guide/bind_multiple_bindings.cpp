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
#include <vector>

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
    di::bind<i1*[]>().to<impl, impl1>()
  );
  // clang-format on

  auto v = injector.create<std::vector<std::unique_ptr<i1>>>();
  assert(2 == v.size());
  assert(dynamic_cast<impl*>(v[0].get()));
  assert(dynamic_cast<impl1*>(v[1].get()));
}
