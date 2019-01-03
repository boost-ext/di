//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/extension/injections/xml_injection.hpp"

#include <cassert>
#include <iostream>

namespace di = boost::di;

//<-
struct interface {
  virtual ~interface() noexcept = default;
};

struct implementation1 : interface {};
struct implementation2 : interface {};

struct xml_parser_stub : di::extension::ixml_parser {
  std::string parse(const std::string&) override {
    static auto i = 0;

    switch (i++) {
      default:
        return {};
      case 0:
        return typeid(implementation1).name();
      case 1:
        return typeid(implementation2).name();
    };

    return {};
  }
};
//->

auto module = [] {
  // clang-format off
  return di::make_injector(
    di::bind<di::extension::ixml_parser>().to<xml_parser_stub>()
  );
  // clang-format on
};

auto xml_module = [] {
  // clang-format off
  return di::make_injector(
    di::bind<interface>().to(di::extension::xml<implementation1, implementation2>())
  );
  // clang-format on
};

int main() {
  /*<<make injector>>*/
  auto injector = di::make_injector(module(), xml_module());

  /*<<create `interface` from xml configuration>>*/
  {
    auto object = injector.create<std::shared_ptr<interface>>();
    assert(object.get());
    assert(dynamic_cast<implementation1*>(object.get()));
  }

  /*<<create `interface` from different xml configuration>>*/
  {
    auto object = injector.create<std::shared_ptr<interface>>();
    assert(object.get());
    assert(dynamic_cast<implementation2*>(object.get()));
  }

  /*<<create `interface` from different xml configuration>>*/
  {
    auto object = injector.create<std::shared_ptr<interface>>();
    assert(!object.get());
  }
}
