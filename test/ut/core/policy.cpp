//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/core/policy.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "common/fakes/fake_dependency.hpp"

namespace core {

struct fake_policy {
  template <class T>
  void operator()(const T&) const noexcept {
    ++calls();
  }

  static int& calls() {
    static auto calls = 0;
    return calls;
  }
};

struct fake_policy_other {
  template <class T>
  void operator()(const T&) const noexcept {
    ++calls();
  }

  static int& calls() {
    static auto calls = 0;
    return calls;
  }
};

test call = [] {
  fake_policy::calls() = 0;
  using dep = fake_dependency<int>;
  pool<aux::type_list<fake_policy>> policies;

  policy::call<arg_wrapper<int, no_name, std::false_type, aux::type_list<>, dep, aux::type_list<>>>(policies);

  expect(1 == fake_policy::calls());
};

test call_policies = [] {
  fake_policy::calls() = 0;
  fake_policy_other::calls() = 0;
  using dep = fake_dependency<int>;
  pool<aux::type_list<fake_policy, fake_policy_other>> policies;

  policy::call<arg_wrapper<int, no_name, std::false_type, aux::type_list<>, dep, aux::type_list<>>>(policies);

  expect(1 == fake_policy::calls());
  expect(1 == fake_policy_other::calls());
};

test ignore_internal_types = [] {
  fake_policy::calls() = 0;
  fake_policy_other::calls() = 0;
  using dep = fake_dependency<int>;
  pool<aux::type_list<fake_policy, fake_policy_other>> policies;
  struct internal : _ {};

  policy::call<arg_wrapper<int, no_name, std::false_type, aux::type_list<>, dep, aux::type_list<>>>(policies);
  policy::call<arg_wrapper<internal, no_name, std::false_type, aux::type_list<>, dep, aux::type_list<>>>(policies);
  policy::call<arg_wrapper<int, no_name, std::false_type, aux::type_list<>, dep, aux::type_list<>>>(policies);

  expect(2 == fake_policy::calls());
  expect(2 == fake_policy_other::calls());
};

}  // namespace core
