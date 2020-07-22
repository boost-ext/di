//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di.hpp"  // no stdinc, stdinc++

namespace di = boost::di;

struct i1 {
  virtual ~i1() noexcept = default;
  virtual void dummy1() = 0;
};
struct impl1 : i1 {
  void dummy1() override {}
};

int main() {
  struct c {
    c(int, i1&) {}
  };

  auto injector = di::make_injector(di::bind<int>().to(42), di::bind<i1>().to<impl1>());
  injector.create<c>();
}
