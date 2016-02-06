//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//<-
#include <memory>
#include <cassert>
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
auto module1 = [] { return di::make_injector(di::bind<interface>().to<implementation1>()); };

/*<<`module2` configuration>>*/
auto module2(const int& i) { return di::make_injector(di::bind<int>().to(i)); }

/*<<module configuration with exposed `data`>>*/
auto exposed_module = []() -> di::injector<const data&> {
  return di::make_injector(di::bind<interface>().to<implementation2>());
};

int main() {
  constexpr auto i = 42;
  constexpr auto d = 87.0;

  /*<<module configuration with movable injector>>*/
  auto movable_injector = di::make_injector(di::bind<double>().to(d));

  /*<<create injector and pass `module1`, `module2` and `exposed_module`>>*/
  auto injector = di::make_injector(module1(), module2(i), exposed_module(), std::move(movable_injector));

  /*<<create `app`>>*/
  injector.create<app>();
}

