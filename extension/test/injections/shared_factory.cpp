//
// Copyright (c) 2012-2017 Kanstantsin Chernik
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/extension/injections/shared_factory.hpp"

#include <cassert>

namespace di = boost::di;

struct interface {
  virtual ~interface() noexcept = default;
  virtual void dummy() = 0;
};

struct implementation : interface {
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

struct implementation_exception : interface {
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
	  di::bind<implementation_exception>().to(di::extension::shared_factory<implementation_exception>([&](const auto& inner_injector)
      {
        //<<throws an exception here>>
        return inner_injector.template create<std::shared_ptr<implementation_exception>>();
      })),
      //<<bind factory interface to implementation>>
      di::bind<implementation>().to(di::extension::shared_factory<implementation>([&](const auto& inner_injector)
      {
        static int calls = 0;
        assert(1 == ++calls);
        //<<shouldn't be recursive call here>>
        return inner_injector.template create<std::shared_ptr<implementation>>();
      })),
      di::bind<interface>().to(di::extension::conditional_shared_factory<implementation>([&]()
      {
        static int calls = 0;
        assert(1 == ++calls);
        return true;
      }))
    );
    // clang-format on

    //<<create `implementation` via `shared_factory`>>
    auto i1 = injector.create<std::shared_ptr<implementation>>();
    auto i2 = injector.create<std::shared_ptr<implementation>>();
    assert(i1);
    assert(i1 == i2);
    assert(1 == implementation::ctor_calls());
    assert(!implementation::dtor_calls());

    //<<create `interface` via `conditional_shared_factory`>>
    auto i3 = injector.create<std::shared_ptr<interface>>();

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

    auto i4 = injector.create<std::shared_ptr<interface>>();
    assert(i3);
    assert(i3 == i4);
    assert(1 == implementation::ctor_calls());
    assert(!implementation::dtor_calls());
  }
  //<<Destroyed with injector>>
  assert(1 == implementation::dtor_calls());
}
