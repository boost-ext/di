//
// Copyright (c) 2012-2019 Kanstantsin Chernik
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/extension/injections/extensible_injector.hpp"

#include <cassert>
#include <memory>

namespace di = boost::di;

struct interface {
  virtual ~interface() noexcept = default;
  virtual int num() = 0;
};

struct any {
  virtual ~any() noexcept = default;
};

struct object : any {};

struct implementation1 : interface {
  int num() override { return 1; }
};

struct implementation2 : interface {
  int num() override { return 2; }
};

struct implementation3 : interface {
  int num() override { return 3; }
};

struct example {
  example(std::unique_ptr<interface> i, std::shared_ptr<implementation3> im3) : i_(std::move(i)), im3_(im3) {}

  std::shared_ptr<interface> i_;
  std::shared_ptr<interface> im3_;
};

int main() {
  //<<create instance object>>
  auto im3_orig = std::make_shared<implementation3>();

  //<<define injector>>
  // clang-format off
  auto orig_injector = di::make_injector(
      //<<bind interface to implementation1>>
      di::bind<interface>().to<implementation1>().in(di::unique)

      //<<bind any to object>>
    , di::bind<any>().to<object>()

      //<<overridden binding for any, object to object>>
    , di::bind<any, object>().to<object>()[di::override]

      //<<bind implementation3 to shared instance>>
    , di::bind<implementation3>().to(im3_orig)
  );

  {
    //<<define extended injector>>
    auto extended_injector = di::make_injector(
        //<<make extensible injector from original injector>>
        di::extension::make_extensible(orig_injector)

        //<<override bound interface to implementation2>>
      , di::bind<interface>().to<implementation2>().in(di::unique)[di::override]
    );
    // clang-format on

    //<<both injectors live together>>
    auto orig_example = orig_injector.create<example>();
    assert(1 == orig_example.i_->num());
    assert(3 == orig_example.im3_->num());

    auto extended_example = extended_injector.create<example>();
    assert(2 == extended_example.i_->num());
    assert(3 == extended_example.im3_->num());

    //<<both injectors share the same dependency>>
    assert(extended_example.im3_ == orig_example.im3_);
    assert(im3_orig == orig_example.im3_);

    //<<injector supports multiple bindings>>
    assert(orig_injector.create<std::shared_ptr<any>>() == orig_injector.create<std::shared_ptr<object>>());
  }
  //<<after death of extended dependency original dependency is still alive>>
  assert(im3_orig == orig_injector.create<std::shared_ptr<implementation3>>());

  {
    //<<make_extensible can be called with empty injector>>
    auto empty_injector = di::make_injector();
    auto empty_extended_injector = di::make_injector(di::extension::make_extensible(empty_injector));
    auto instance = empty_extended_injector.create<implementation1>();
    assert(1 == instance.num());
  }
}
