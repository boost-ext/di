//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
// clang-format off
#include <initializer_list>  // has to be before, due to the bug in clang < 3.7
// clang-format on
#include <boost/di.hpp>
#include <cassert>
#include <memory>
#include <set>
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
  auto il = {1, 2, 3};
  auto injector = di::make_injector(
    di::bind<int[]>().to(il) // or di::bind<int*[]>.to(il)
  );
  // clang-format on

  auto v = injector.create<std::vector<int>>();
  assert(3 == v.size());
  assert(1 == v[0]);
  assert(2 == v[1]);
  assert(3 == v[2]);
}
