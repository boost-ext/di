//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/extension/injections/assisted_injection.hpp"

#include <cassert>
#include <functional>
#include <memory>
#include <string>

namespace di = boost::di;

//<-
struct interface {
  virtual ~interface() noexcept = default;
  virtual void dummy() = 0;
};
struct implementation : interface {
  void dummy() override {}
};

struct example {
  /*<<define `example` constructor using `assisted` as assisted_injection creation parameters>>*/
  BOOST_DI_INJECT(example, (named = di::extension::assisted) int date /*from assisted_injection*/
                  ,
                  std::unique_ptr<interface> up /*from injector*/
                  ,
                  (named = di::extension::assisted) double factor /*from assisted_injection*/
                  ,
                  (named = di::extension::assisted) std::string str /*from assisted_injection*/
                  ,
                  int i /*from injector*/) {
    assert(i == 87);
    assert(dynamic_cast<implementation*>(up.get()));
    assert(date == 42);
    assert(factor == 123.0);
    assert(str == "str");
  }
};
//->

int main() {
  /*<<assisted_injection declaration using std function>>*/
  using example_assisted_injection = std::function<std::unique_ptr<example>(int, double, std::string)>;

  // clang-format off
  auto injector = di::make_injector(
      di::bind<int>().to(87)
    , di::bind<interface>().to<implementation>()
      /*<<bind `example_assisted_injection` to assisted_injection creating `example`>>*/
    , di::bind<example_assisted_injection>().to(di::extension::assisted_injection<example>())
  );
  // clang-format on

  auto f = injector.create<example_assisted_injection>();
  f(42, 123.0, "str");
}
