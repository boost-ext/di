//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
#include <functional>
#include <memory>
#include <string>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct interface1 {
  virtual ~interface1() noexcept = default;
};
struct interface2 {
  virtual ~interface2() noexcept = default;
};
struct implementation1 : interface1 {};
struct implementation1_2 : interface1 {};
struct implementation2 : interface2 {};
auto some_name = [] {};
auto int_name = [] {};
//->

struct service {
  BOOST_DI_INJECT(service, const std::shared_ptr<interface1>& sp, bool b, int i, std::function<int()> f,
                  (named = int_name) const int ni)
      : sp(sp) {
    assert(dynamic_cast<implementation1_2*>(sp.get()));  // overridden
    assert(!b);                                          // default initialization
    assert(i == 42);
    assert(f() == 87);
    assert(ni == 123);
  }

  std::shared_ptr<interface1> sp;
};

struct app {
  BOOST_DI_INJECT(app, service copy, std::shared_ptr<interface1> sp, std::unique_ptr<interface2> ap, int i,
                  (named = some_name) const std::string& s, float& f, const double& d)
      : str(s), f(f), d(d) {
    assert(dynamic_cast<implementation2*>(ap.get()));
    assert(dynamic_cast<implementation1_2*>(sp.get()));  // overridden
    assert(copy.sp.get() == sp.get());
    assert(i == 42);
    assert(str == "my_text");
    assert(f == 0.f);
    assert(d == 0.f);
  }

  app& operator=(const app&) = delete;

  std::string str;
  float& f;
  const double& d;
};

int main() {
  float f = 0.f;
  double d = 0.0;

  /*<<create injector with `interface` binding to `implementation1`>>*/
  auto config = [] { return di::make_injector(di::bind<interface1>().to<implementation1>()); };

  /*<<create injector with configuration>>*/
  // clang-format off
  auto injector = di::make_injector(
    di::bind<interface2>().to<implementation2>()
  , di::bind<int>().to(42)
  , di::bind<std::string>().named(some_name).to("my_text")
  , di::bind<>().to(f)
  , di::bind<>().to(d)
  , di::bind<std::function<int()>>().to([] { return 87; })
  , di::bind<>().named(int_name).to(123)
  , config()
  , di::bind<interface1>().to(std::make_shared<implementation1_2>())[di::override]
  );
  // clang-format on

  /*<<create `service_app`>>*/
  auto service_app = injector.create<app>();

  /*<<verify parameter `f` affection by `service_app`>>*/
  service_app.f = 42.f;
  assert(f == 42.f);

  d = 42.f;
  assert(service_app.d == 42.f);
}
