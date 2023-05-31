//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/config.hpp"
#include <memory>
#include <type_traits>
#include <utility>
#include "common/common.hpp"
#include "common/fakes/fake_injector.hpp"

struct policy1 {
  template <class T>
  void operator()(T) const;
};

struct policy2 {
  template <class T>
  void operator()(T) const;
};

test make_policies_types = [] {
  expect(std::is_same<core::pool<aux::type_list<>>, decltype(make_policies())>{});
  expect(std::is_same<core::pool<aux::type_list<policy1, policy2>>, decltype(make_policies(policy1{}, policy2{}))>{});
};

test default_config = [] {
  expect(std::is_same<providers::stack_over_heap, decltype(std::declval<config>().provider(std::declval<fake_injector<>*>()))>{});
  expect(std::is_same<core::pool<aux::type_list<>>, decltype(std::declval<config>().policies(std::declval<fake_injector<>*>()))>{});
};

struct c {};

test traits = [] {
  static_expect(std::is_same<scopes::unique, typename config::scope_traits<int>::type>{});
  static_expect(std::is_same<scopes::unique, typename config::scope_traits<c>::type>{});
  static_expect(std::is_same<scopes::singleton, typename config::scope_traits<const int&>::type>{});
  static_expect(std::is_same<scopes::unique, typename config::scope_traits<int*>::type>{});
  static_expect(std::is_same<scopes::unique, typename config::scope_traits<const int*>::type>{});
  static_expect(std::is_same<scopes::singleton, typename config::scope_traits<std::shared_ptr<int>>::type>{});
  static_expect(std::is_same<scopes::singleton, typename config::scope_traits<const std::shared_ptr<int>&>::type>{});
  static_expect(std::is_same<scopes::singleton, typename config::scope_traits<boost::shared_ptr<int>>::type>{});
  static_expect(std::is_same<scopes::singleton, typename config::scope_traits<const boost::shared_ptr<int>&>::type>{});
  static_expect(std::is_same<scopes::singleton, typename config::scope_traits<std::weak_ptr<int>>::type>{});
  static_expect(std::is_same<scopes::singleton, typename config::scope_traits<const std::weak_ptr<int>&>::type>{});
  static_expect(std::is_same<scopes::singleton, typename config::scope_traits<int&>::type>{});
  static_expect(std::is_same<scopes::unique, typename config::scope_traits<std::unique_ptr<int>>::type>{});
  static_expect(std::is_same<scopes::unique, typename config::scope_traits<std::unique_ptr<int, deleter<int>>>::type>{});
  static_expect(std::is_same<scopes::singleton, typename config::scope_traits<std::shared_ptr<int>>::type>{});
  static_expect(std::is_same<scopes::singleton, typename config::scope_traits<std::weak_ptr<int>>::type>{});
  static_expect(std::is_same<scopes::unique, typename config::scope_traits<int&&>::type>{});
  static_expect(std::is_same<scopes::unique, typename config::scope_traits<const int&&>::type>{});
};
