//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include "boost/di.hpp"

namespace di = boost::di;

struct i1 {
  virtual ~i1() noexcept = default;
  virtual void dummy1() = 0;
};
struct impl1 : i1 {
  void dummy1() override {}
};

static auto called = 0;

class config : public di::config {
 public:
  static auto policies(...) noexcept {
    return di::make_policies([](auto) { ++called; });
  }
};

test call_policy_lambda = [] {
  called = 0;
  auto injector = di::make_injector<config>();
  expect(0 == injector.create<int>());
  expect(1 == called);
};

class config_provider : public di::config {
 public:
  static auto provider(...) noexcept {
    ++called;
    return di::providers::heap{};
  }
};

test call_provider = [] {
  called = 0;
  auto injector = di::make_injector<config_provider>();
  injector.create<int>();
  expect(1 == called);
};

class must_be_bound : public di::config {
 public:
  static auto policies(...) noexcept {
    using namespace di::policies;
    using namespace di::policies::operators;
    return di::make_policies(constructible(std::is_same<di::_, double>{} || is_bound<di::_>{}));
  }
};

test constructible_policy_must_be_bound = [] {
  struct c {
    int i = 0;
    double d = 0.0;
  };

  auto injector = di::make_injector<must_be_bound>(di::bind<int>().to(42));

  injector.create<c>();
};

class disallow_raw_pointers : public di::config {
 public:
  static auto policies(...) noexcept {
    using namespace di::policies;
    using namespace di::policies::operators;
    return di::make_policies(constructible(!std::is_pointer<di::_>{}));
  }
};

test constructible_policy_disallow_raw_pointers = [] {
  struct c {
    c(int, double) {}
  };

  {
    auto injector = di::make_injector<disallow_raw_pointers>();
    injector.create<c>();
  }

  {
    di::injector<c> injector = di::make_injector<disallow_raw_pointers>();
    injector.create<c>();
  }
};

class must_be_injected : public di::config {
 public:
  static auto policies(...) noexcept {
    using namespace di::policies;
    using namespace di::policies::operators;
    return di::make_policies(constructible(is_injected<di::_>{}));
  }
};

test constructible_policy_must_be_injected = [] {
  struct d {
    BOOST_DI_INJECT(d, int, double) {}
  };
  struct c {
    BOOST_DI_INJECT(c, int, d) {}
  };

  {
    auto injector = di::make_injector<must_be_injected>();
    injector.create<c>();
  }

  {
    di::injector<c> injector = di::make_injector<must_be_injected>();
    injector.create<c>();
  }
};

struct policy {
  static auto& called() {
    static auto i = 0;
    return i;
  }

  template <class T>
  void operator()(const T&) const noexcept {
    ++called();
  }
};

class custom_policies : public di::config {
 public:
  static auto policies(...) noexcept {
    return di::make_policies(
        policy{}, [](auto) { ++policy::called(); },
        [](BOOST_DI_UNUSED auto type, BOOST_DI_UNUSED auto dependency, BOOST_DI_UNUSED auto... ctor) { ++policy::called(); });
  }
};

test call_custom_policies = [] {
  policy::called() = 0;
  auto injector = di::make_injector<custom_policies>();
  injector.create<int>();
  expect(3 == policy::called());
};

test call_custom_policies_with_exposed_injector = [] {
  policy::called() = 0;
  di::injector<std::unique_ptr<i1>> injector = di::make_injector<custom_policies>(di::bind<i1>().to<impl1>());
  auto object = injector.create<std::unique_ptr<i1>>();
  expect(dynamic_cast<i1*>(object.get()));
  expect(3 == policy::called());
};
