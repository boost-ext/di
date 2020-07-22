//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/instance.hpp"
#include <functional>
#include <memory>
#include <string>
#include "common/common.hpp"
#include "common/fakes/fake_injector.hpp"
#include "common/fakes/fake_provider.hpp"

namespace scopes {

struct interface {
  virtual ~interface() noexcept = default;
  virtual void dummy() = 0;
};
struct implementation : public interface {
  void dummy() override{};
};

test from_arithmetic = [] {
  const int i = 42;
  expect(i == static_cast<int>(instance::scope<int, int>{i}.create<void, no_name>(fake_provider<>{})));
};

test from_string = [] {
  const std::string s = "string";
  std::string object = instance::scope<std::string, std::string>{s}.create<void, no_name>(fake_provider<>{});
  expect(s == object);
};

test from_ref = [] {
  struct c {
  } c_;
  c& c_ref_ = instance::scope<c, c&>{c_}.create<void, no_name>(fake_provider<>{});
  expect(&c_ == &c_ref_);
};

test from_const_ref = [] {
  struct c {
  } c_;
  const c& c_ref_ = instance::scope<c, c&>{c_}.create<void, no_name>(fake_provider<>{});
  expect(&c_ == &c_ref_);
};

test from_shared_ptr = [] {
  struct c {};
  auto c_ = std::make_shared<c>();
  std::shared_ptr<c> sp_c = instance::scope<c, std::shared_ptr<c>>{c_}.create<void, no_name>(fake_provider<>{});
  expect(c_ == sp_c);
};

test from_context = [] {
  expect((static_cast<int>(instance::scope<int, int>{87}.create<void, no_name>(fake_provider<>{})) !=
          static_cast<int>(instance::scope<int, int>{42}.create<void, no_name>(fake_provider<>{}))));

  struct c {};
  auto c1 = std::make_shared<c>();
  auto c2 = std::make_shared<c>();

  {
    std::shared_ptr<c> c1_ = instance::scope<c, std::shared_ptr<c>>{c1}.create<void, no_name>(fake_provider<>{});
    std::shared_ptr<c> c2_ = instance::scope<c, std::shared_ptr<c>>{c2}.create<void, no_name>(fake_provider<>{});
    expect(c1_ != c2_);
  }

  {
    std::shared_ptr<c> c1_ = instance::scope<c, std::shared_ptr<c>>{c1}.create<void, no_name>(fake_provider<>{});
    std::shared_ptr<c> c2_ = instance::scope<c, std::shared_ptr<c>>{c1}.create<void, no_name>(fake_provider<>{});
    expect(c1_ == c2_);
  }
};

test from_if_shared_ptr = [] {
  auto i = std::make_shared<implementation>();
  std::shared_ptr<interface> c =
      instance::scope<interface, std::shared_ptr<interface>>{i}.create<void, no_name>(fake_provider<>{});
  expect(i == c);
};

test from_function_expr = [] {
  auto flag = false;
  auto expr = [&flag]() -> std::shared_ptr<interface> {
    if (!flag) {
      return std::make_shared<implementation>();
    }
    return nullptr;
  };

  instance::scope<interface, decltype(expr)> instance{expr};

  {
    std::shared_ptr<interface> sp = instance.create<void, no_name>(fake_provider<>{});
    expect(dynamic_cast<implementation*>(sp.get()));
  }

  {
    flag = true;
    std::shared_ptr<interface> sp = instance.create<void, no_name>(fake_provider<>{});
    expect(nullptr == sp);
  }
};

test from_function_expr_with_expected_function_expr = [] {
  constexpr auto i = 42;
  instance::scope<function<int()>, function<int()>> instance{[&] { return i; }};
  function<int()> f = instance.create<void, no_name>(fake_provider<>{});
  expect(i == f());
};

test from_function_expr_with_injector = [] {
  auto expr = [](const auto& injector) { return injector.template create<int>(); };
  instance::scope<int, decltype(expr)> instance{expr};
  expect(0 == static_cast<int>(instance.create<void, no_name>(fake_provider<int>{})));
};

test exposed = [] {
  instance::scope<void, aux::type_list<named<no_name, int>>> instance{fake_injector<>{}};
  expect(0 == static_cast<int>(instance.create<int, no_name>(fake_provider<int>{})));
};

test exposed_many = [] {
  struct name {};
  instance::scope<void, aux::type_list<named<name, double>, named<no_name, int>>> instance{fake_injector<>{}};
  expect(0 == static_cast<int>(instance.create<int, no_name>(fake_provider<int>{})));
  expect(0.0 == static_cast<double>(instance.create<double, name>(fake_provider<int>{})));
};

}  // namespace scopes
