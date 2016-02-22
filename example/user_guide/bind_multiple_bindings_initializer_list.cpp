//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <memory>
#include <set>
#include <vector>
#include <cassert>
#include <boost/di.hpp>

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
    di::bind<int[]>().to({1, 2, 3}) // or int*[]
  );
  // clang-format on

  auto v = injector.create<std::vector<int>>();
  assert(3 == v.size());
  assert(1 == v[0]);
  assert(2 == v[1]);
  assert(3 == v[2]);
}
