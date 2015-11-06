//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include <type_traits>
#include "boost/di/type_traits/scope_traits.hpp"
#include "common/common.hpp"

namespace type_traits {

struct c {};

test traits = [] {
  static_expect(std::is_same<scopes::unique, scope_traits<int>::type>{});
  static_expect(std::is_same<scopes::unique, scope_traits<c>::type>{});
  static_expect(std::is_same<scopes::singleton, scope_traits<const int&>::type>{});
  static_expect(std::is_same<scopes::unique, scope_traits<int*>::type>{});
  static_expect(std::is_same<scopes::unique, scope_traits<const int*>::type>{});
  static_expect(std::is_same<scopes::singleton, scope_traits<std::shared_ptr<int>>::type>{});
  static_expect(std::is_same<scopes::singleton, scope_traits<const std::shared_ptr<int>&>::type>{});
  static_expect(std::is_same<scopes::singleton, scope_traits<boost::shared_ptr<int>>::type>{});
  static_expect(std::is_same<scopes::singleton, scope_traits<const boost::shared_ptr<int>&>::type>{});
  static_expect(std::is_same<scopes::singleton, scope_traits<std::weak_ptr<int>>::type>{});
  static_expect(std::is_same<scopes::singleton, scope_traits<const std::weak_ptr<int>&>::type>{});
  static_expect(std::is_same<scopes::singleton, scope_traits<int&>::type>{});
  static_expect(std::is_same<scopes::unique, scope_traits<std::unique_ptr<int>>::type>{});
  static_expect(std::is_same<scopes::unique, scope_traits<std::unique_ptr<int, deleter<int>>>::type>{});
  static_expect(std::is_same<scopes::singleton, scope_traits<std::shared_ptr<int>>::type>{});
  static_expect(std::is_same<scopes::singleton, scope_traits<std::weak_ptr<int>>::type>{});
  static_expect(std::is_same<scopes::unique, scope_traits<int&&>::type>{});
  static_expect(std::is_same<scopes::unique, scope_traits<const int&&>::type>{});
};

}  // type_traits
