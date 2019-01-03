//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/extension/scopes/scoped.hpp"

#include <cassert>

namespace di = boost::di;

//<-
struct interface1 {
  virtual ~interface1() noexcept = default;
  virtual void dummy() = 0;
};

struct implementation1 : interface1 {
  static auto &ctor_calls() {
    static auto calls = 0;
    return calls;
  }
  static auto &dtor_calls() {
    static auto calls = 0;
    return calls;
  }
  implementation1() { ctor_calls()++; }
  ~implementation1() { dtor_calls()++; }
  void dummy() override {}
};

struct interface2 {
  virtual ~interface2() noexcept = default;
};

struct implementation2 : interface2 {
  static auto &ctor_calls() {
    static auto calls = 0;
    return calls;
  }
  static int &dtor_calls() {
    static auto calls = 0;
    return calls;
  }
  implementation2() { ctor_calls()++; }
  ~implementation2() { dtor_calls()++; }
};
//->

auto module = [] {
  // clang-format off
  return di::make_injector(
    di::bind<interface1>().to<implementation1>()
  );
  // clang-format on
};

int main() {
  assert(!implementation1::ctor_calls());
  assert(!implementation1::dtor_calls());
  assert(!implementation2::ctor_calls());
  assert(!implementation2::dtor_calls());

  auto parent_injector = module();
  /*<<create `interface1` using deduced singleton scope>>*/
  parent_injector.create<interface1 &>();
  assert(1 == implementation1::ctor_calls());
  {
    /*<<create `child_injector` with `module` and binding using scoped singleton scope>>*/
    // clang-format off
    auto child_injector = di::make_injector(
        module()
      , di::bind<interface2>().to<implementation2>().in(di::extension::scoped)
    );
    // clang-format on
    child_injector.create<interface2 &>();
    assert(1 == implementation2::ctor_calls());
  }  // end of scoped
  assert(1 == implementation2::dtor_calls());

  /*<<create `interface1` again in singleton scope>>*/
  parent_injector.create<interface1 &>();
  /*<<already created>>*/
  assert(1 == implementation1::ctor_calls());
  /*<<`implementation1` will die with the app (static storage)>>*/
  assert(0 == implementation1::dtor_calls());
}
