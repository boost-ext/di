//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/core/binder.hpp"
#include <type_traits>
#include "boost/di/scopes/unique.hpp"

namespace core {

test resolve_types_default = [] {
  struct deps {};
  auto result = binder::resolve<int>((deps*)nullptr);
  expect(std::is_same<decltype(result), dependency<scopes::deduce, int>>{});
};

test resolve_types_custom_default = [] {
  struct deps {};
  struct not_resolved {};
  auto result = binder::resolve<int, no_name, not_resolved>((deps*)nullptr);
  expect(std::is_same<decltype(result), not_resolved>{});
};

test resolve_types_found = [] {
  struct deps : dependency<scopes::unique, int> {};
  using result = std::remove_reference_t<decltype(binder::resolve<int>((deps*)nullptr))>;
  expect(std::is_same<result, dependency<scopes::unique, int>>{});
};

test resolve_types_not_found_by_name = [] {
  struct name {};
  struct deps : dependency<scopes::unique, int, int, name> {};
  using result = std::remove_reference_t<decltype(binder::resolve<int>((deps*)nullptr))>;
  expect(std::is_same<result, dependency<scopes::deduce, int>>{});
};

test resolve_types_not_found_inheritence = [] {
  struct deps : dependency<scopes::unique, _, int, no_name> {};

  struct any : _ {};

  using result = std::remove_reference_t<decltype(binder::resolve<any>((deps*)nullptr))>;
  expect(std::is_same<result, dependency<scopes::deduce, any>>{});
};

test resolve_types_found_by_name = [] {
  struct name {};
  struct deps : dependency<scopes::unique, int, int, name> {};
  using result = std::remove_reference_t<decltype(binder::resolve<int, name>((deps*)nullptr))>;
  expect(std::is_same<result, dependency<scopes::unique, int, int, name>>{});
};

test resolve_types_found_many = [] {
  struct not_resolved {};
  struct deps : dependency<scopes::unique, int>, dependency<scopes::unique, double>, dependency<scopes::unique, float> {};

  {
    using result = std::remove_reference_t<decltype(binder::resolve<float, no_name, not_resolved>((deps*)nullptr))>;
    expect(std::is_same<result, dependency<scopes::unique, float>>{});
  }

  {
    using result = std::remove_reference_t<decltype(binder::resolve<short, no_name, not_resolved>((deps*)nullptr))>;
    expect(std::is_same<result, not_resolved>{});
  }
};

test resolve_types_found_priority = [] {
  struct deps : dependency<scopes::unique, int, int, no_name, override>, dependency<scopes::unique, int, int, no_name> {};

  using result = std::remove_reference_t<decltype(binder::resolve<int>((deps*)nullptr))>;
  expect(std::is_same<result, dependency<scopes::unique, int, int, no_name, override>>{});
};

test resolve_types_found_priority_order = [] {
  struct deps : dependency<scopes::unique, int, int, no_name>, dependency<scopes::unique, int, int, no_name, override> {};

  using result = std::remove_reference_t<decltype(binder::resolve<int>((deps*)nullptr))>;
  expect(std::is_same<result, dependency<scopes::unique, int, int, no_name, override>>{});
};

}  // namespace core
