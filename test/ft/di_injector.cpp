//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include "boost/di.hpp"

namespace di = boost::di;

struct empty {};
struct i1 {
  virtual ~i1() noexcept = default;
  virtual void dummy1() = 0;
};
struct impl1 : i1 {
  void dummy1() override {}
};
struct complex1 {
  explicit complex1(const std::shared_ptr<i1>& i1) : i1_(i1) {}

  std::shared_ptr<i1> i1_;
};

test empty_injector = [] {
  auto injector = di::make_injector();
  expect(0 == injector.create<int>());
};

test create_using_copy = [] {
  auto injector = di::make_injector();
  injector.create<empty>();
};

test create_using_ptr = [] {
  auto injector = di::make_injector();
  std::unique_ptr<empty> object{injector.create<empty*>()};
  expect(object.get());
};

test create_using_const_ptr = [] {
  auto injector = di::make_injector();
  std::unique_ptr<const empty> object{injector.create<const empty*>()};
  expect(object.get());
};

test create_using_unique_ptr = [] {
  auto injector = di::make_injector();
  auto object = injector.create<std::unique_ptr<empty>>();
  expect(object.get());
};

test create_using_shared_ptr = [] {
  auto injector = di::make_injector();
  auto object = injector.create<std::shared_ptr<empty>>();
  expect(object.get());
};

test create_ptr = [] {
  struct c {
    explicit c(di::aux::owner<i1*> ptr) { delete ptr; }
  };

  auto injector = di::make_injector(di::bind<i1>().to<impl1>());

  injector.create<c>();
};

test create_interface_when_impl_with_one_arg_ctor = [] {
  struct impl : i1 {
    explicit impl(int) {}
    void dummy1() override {}
  };

  auto injector = di::make_injector(di::bind<i1>().to<impl>());

  auto object = injector.create<std::unique_ptr<i1>>();
  expect(object.get());
};

test injectors_mix = [] {
  auto injector = di::make_injector(di::make_injector(di::bind<i1>().to<impl1>()));

  auto object = injector.create<std::shared_ptr<complex1>>();
  expect(object->i1_.get());
};
