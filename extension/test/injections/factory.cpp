//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/extension/injections/factory.hpp"

#include <cassert>

namespace di = boost::di;

struct interface {
  virtual ~interface() noexcept = default;
  virtual void dummy1() = 0;
};

struct implementation : interface {
  void dummy1() override {}
};

struct implementation_exception : interface {
#if defined(__EXCEPTIONS)
  implementation_exception() { throw 0; }
#endif
  void dummy1() override {}
};

struct implementation_with_args : interface {
  implementation_with_args(int i, double d) {
    assert(42 == i);
    assert(87.0 == d);
  }
  void dummy1() override {}
};

struct implementation_with_injected_args : interface {
  implementation_with_injected_args(int i, double d) {
    assert(123 == i);
    assert(87.0 == d);
  }
  void dummy1() override {}
};

/*<<example `example` class>>*/
class example {
 public:
  example(const di::extension::ifactory<interface>& f1, const di::extension::ifactory<interface, int, double>& f2,
          const di::extension::ifactory<interface, double>& f3) {
    assert(dynamic_cast<implementation*>(f1.create().get()));
    assert(dynamic_cast<implementation_with_args*>(f2.create(42, 87.0).get()));
    assert(dynamic_cast<implementation_with_injected_args*>(f3.create(87.0).get()));
  }
};

int main() {
  //<<define injector>>
  auto module = [] { return di::make_injector(di::bind<>().to(123)); };

  // clang-format off
  auto injector = di::make_injector(module()
  // <<bind instance just to make sure that factory doesn't affect injector>>
  , di::bind<interface>().to(std::make_shared<implementation>())

  // <<bind instance which throws exception to make sure that exception doesn't affect injector>>
  , di::bind<di::extension::ifactory<implementation_exception>>().to(di::extension::factory<implementation_exception>{})

  //<<bind factory interface to implementation>>
  , di::bind<di::extension::ifactory<interface>>().to(di::extension::factory<implementation>{})

  //<<bind factory interface with parameters to implementation_with_args>>
  , di::bind<di::extension::ifactory<interface, int, double>>().to(di::extension::factory<implementation_with_args>{})

  //<<bind factory interface with some parameters injected to implementation_with_injected_args>>
  , di::bind<di::extension::ifactory<interface, double>>().to(di::extension::factory<implementation_with_injected_args>{})
  );
  // clang-format on

  /*<<create `example`>>*/
  injector.create<example>();

/*<<create `implementation_exception`>>*/
#if defined(__EXCEPTIONS)
  auto exception_thrown = false;
  try {
    injector.create<const implementation_exception&>();
  } catch (...) {
    exception_thrown = true;
  }
  assert(exception_thrown);
#endif

  // Check whether injector is affected by factory
  assert(injector.create<std::shared_ptr<interface>>());
}
