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

int main() {
  // clang-format off
  auto injector = di::make_injector(
    di::bind<i1>().in(di::singleton).to<impl1>()
  );
  // clang-format on

  assert(injector.create<std::shared_ptr<i1>>() == injector.create<std::shared_ptr<i1>>());
}
