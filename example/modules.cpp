//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
#include <memory>
#include <utility>
//->

#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct interface {
  virtual ~interface() noexcept = default;
  virtual void dummy() = 0;
};
struct implementation1 : interface {
  void dummy() override {}
};
struct implementation2 : interface {
  void dummy() override {}
};
//->

struct data {
  std::shared_ptr<interface> sp;
};

struct app {
  app(std::unique_ptr<interface> up, int i, double d, const data& data) {
    assert(dynamic_cast<implementation1*>(up.get()));
    assert(i == 42);
    assert(d == 87.0);
    assert(dynamic_cast<implementation2*>(data.sp.get()));
  }
};

/*<<`module1` configuration>>*/
auto module1 = [] {
  // clang-format off
  return di::make_injector(
    di::bind<interface>().to<implementation1>()
  );
  // clang-format on
};

/*<<`module2` configuration>>*/
auto module2(const int& i) {
  // clang-format off
  return di::make_injector(
    di::bind<int>().to(i)
  );
}
// clang-format on

/*<<module configuration with exposed `data`>>*/
auto exposed_module = []() -> di::injector<const data&> {
  // clang-format off
  return di::make_injector(
    di::bind<interface>().to<implementation2>()
  );
  // clang-format on
};

int main() {
  constexpr auto i = 42;
  constexpr auto d = 87.0;

  /*<<module configuration with movable injector>>*/
  // clang-format off
  auto movable_injector = di::make_injector(
    di::bind<double>().to(d)
  );
  // clang-format on

  /*<<create injector and pass `module1`, `module2` and `exposed_module`>>*/
  // clang-format off
  auto injector = di::make_injector(
    module1()
  , module2(i)
  , exposed_module()
  , std::move(movable_injector)
  );
  // clang-format on

  /*<<create `app`>>*/
  injector.create<app>();
}
