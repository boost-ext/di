//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/extension/policies/types_dumper.hpp"

#include <memory>

namespace di = boost::di;

auto int_1 = [] { return "first int"; };
auto int_2 = [] { return "second int"; };

struct i0 {
  virtual ~i0(){};
};
struct c0 : i0 {};
struct c1 {
  c1(std::shared_ptr<i0>, int) {}
};
struct c2 {
  BOOST_DI_INJECT(c2, (named = int_1) int, (named = int_2) int, char) {}
};
struct c3 {
  c3(std::shared_ptr<c1>, std::shared_ptr<c2>) {}
};

int main() {
  /*<<define injector>>*/
  // clang-format off
  auto injector = di::make_injector<di::extension::types_dumper>(
    di::bind<i0>().to<c0>()
  , di::bind<int>().named(int_1).to(42)
  , di::bind<int>().named(int_2).to(42)
  );
  // clang-format on

  /*<<iterate through created objects with `types_dumper`>>*/
  injector.create<c3>();

  /*<< output [pre
      (2c3 -> 2c3)
          (St10shared_ptrI2c1E -> 2c1)
              (St10shared_ptrI2i0E -> 2c0)
              (i -> i)
          (St10shared_ptrI2c2E -> 2c2)
              (i[first int] -> i)
              (i[second int] -> i)
              (c -> c)
  ]>>*/
}
