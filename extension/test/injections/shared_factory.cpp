//
// Copyright (c) 2012-2019 Kanstantsin Chernik
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/extension/injections/shared_factory.hpp"

#include <cassert>

namespace di = boost::di;
namespace ext = di::extension;

struct interface1 {
  virtual ~interface1() noexcept = default;
  virtual void dummy() = 0;
};

struct interface2 {
  virtual ~interface2() noexcept = default;
  virtual void dummy() = 0;
};

struct implementation : interface1, interface2 {
  static auto& ctor_calls() {
    static auto calls = 0;
    return calls;
  }
  static auto& dtor_calls() {
    static auto calls = 0;
    return calls;
  }
  implementation() { ctor_calls()++; }
  ~implementation() { dtor_calls()++; }
  void dummy() override {}
};

struct implementation_exception : interface1 {
#if defined(__EXCEPTIONS)
  implementation_exception() { throw 0; }
#endif
  void dummy() override {}
};

int main() {
  assert(!implementation::ctor_calls());
  assert(!implementation::dtor_calls());

  {
    //<<define injector>>
    // clang-format off
    auto injector = di::make_injector(
      di::bind<implementation_exception>().to(ext::shared_factory<implementation_exception>([&](const auto& inner_injector)
      {
        //<<throws an exception here>>
        return inner_injector.template create<std::shared_ptr<implementation_exception>>();
      })),
      //<<bind factory interface to implementation>>
      di::bind<interface1, implementation>().to(ext::shared_factory<implementation, ext::no_recursion>([&](const auto& inner_injector)
      {
        static int calls = 0;
        assert(1 == ++calls);
        //<<shouldn't be recursive call here>>
        return inner_injector.template create<std::shared_ptr<implementation>>();
      })),
      di::bind<interface2>().to(ext::conditional_shared_factory<implementation>([&]()
      {
        static int calls = 0;
        assert(1 == ++calls);
        return true;
      }))
    );
    // clang-format on

    //<<create `implementation` via `shared_factory`>>
    auto i1 = injector.create<std::shared_ptr<interface1>>();
    auto i2 = injector.create<std::shared_ptr<implementation>>();
    assert(i1);
    assert(i1 == i2);
    assert(1 == implementation::ctor_calls());
    assert(!implementation::dtor_calls());

    //<<create `interface` via `conditional_shared_factory`>>
    auto i3 = injector.create<std::shared_ptr<interface2>>();

//<<make sure that exception doesn't affect injector>>
#if defined(__EXCEPTIONS)
    auto exception_thrown = false;
    try {
      injector.create<std::shared_ptr<implementation_exception>>();
    } catch (...) {
      exception_thrown = true;
    }
    assert(exception_thrown);
#endif

    auto i4 = injector.create<std::shared_ptr<interface2>>();
    assert(i3);
    assert(i3 == i4);
    assert(1 == implementation::ctor_calls());
    assert(!implementation::dtor_calls());
  }
  //<<Destroyed with injector>>
  assert(1 == implementation::dtor_calls());
}
