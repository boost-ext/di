//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/scope_traits.hpp"
#include <memory>
#include <type_traits>
#include "common/common.hpp"

namespace type_traits {

struct c {};

template <class T>
using scope_traits_t = typename scope_traits<T>::type;

test traits = [] {
  static_expect(std::is_same<scopes::unique, scope_traits_t<int>>{});
  static_expect(std::is_same<scopes::unique, scope_traits_t<c>>{});
  static_expect(std::is_same<scopes::singleton, scope_traits_t<const int&>>{});
  static_expect(std::is_same<scopes::unique, scope_traits_t<int*>>{});
  static_expect(std::is_same<scopes::unique, scope_traits_t<const int*>>{});
  static_expect(std::is_same<scopes::singleton, scope_traits_t<std::shared_ptr<int>>>{});
  static_expect(std::is_same<scopes::singleton, scope_traits_t<const std::shared_ptr<int>&>>{});
  static_expect(std::is_same<scopes::singleton, scope_traits_t<boost::shared_ptr<int>>>{});
  static_expect(std::is_same<scopes::singleton, scope_traits_t<const boost::shared_ptr<int>&>>{});
  static_expect(std::is_same<scopes::singleton, scope_traits_t<std::weak_ptr<int>>>{});
  static_expect(std::is_same<scopes::singleton, scope_traits_t<const std::weak_ptr<int>&>>{});
  static_expect(std::is_same<scopes::singleton, scope_traits_t<int&>>{});
  static_expect(std::is_same<scopes::unique, scope_traits_t<std::unique_ptr<int>>>{});
  static_expect(std::is_same<scopes::unique, scope_traits_t<std::unique_ptr<int, deleter<int>>>>{});
  static_expect(std::is_same<scopes::singleton, scope_traits_t<std::shared_ptr<int>>>{});
  static_expect(std::is_same<scopes::singleton, scope_traits_t<std::weak_ptr<int>>>{});
  static_expect(std::is_same<scopes::unique, scope_traits_t<int&&>>{});
  static_expect(std::is_same<scopes::unique, scope_traits_t<const int&&>>{});
};

}  // namespace type_traits
