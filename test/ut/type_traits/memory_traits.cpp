//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include <type_traits>
#include "boost/di/type_traits/memory_traits.hpp"
#if __has_include(<boost / shared_ptr.hpp>)
#include <boost/shared_ptr.hpp>
#endif
#include "common/common.hpp"

namespace type_traits {

struct c {};
struct i {
  virtual ~i() noexcept = default;
};

test traits = [] {
  static_expect(std::is_same<stack, memory_traits<int>::type>{});
  static_expect(std::is_same<stack, memory_traits<c>::type>{});
  static_expect(std::is_same<stack, memory_traits<const int&>::type>{});
  static_expect(std::is_same<heap, memory_traits<int*>::type>{});
  static_expect(std::is_same<heap, memory_traits<const int*>::type>{});
  static_expect(std::is_same<heap, memory_traits<std::shared_ptr<int>>::type>{});
  static_expect(std::is_same<heap, memory_traits<const std::shared_ptr<int>&>::type>{});
  static_expect(std::is_same<heap, memory_traits<std::weak_ptr<int>>::type>{});
  static_expect(std::is_same<heap, memory_traits<const std::weak_ptr<int>&>::type>{});
  static_expect(std::is_same<stack, memory_traits<int&>::type>{});
  static_expect(std::is_same<heap, memory_traits<std::unique_ptr<int>>::type>{});
  static_expect(std::is_same<heap, memory_traits<std::unique_ptr<int, deleter<int>>>::type>{});
  static_expect(std::is_same<heap, memory_traits<std::shared_ptr<int>>::type>{});
  static_expect(std::is_same<heap, memory_traits<std::weak_ptr<int>>::type>{});
  static_expect(std::is_same<stack, memory_traits<int&&>::type>{});
  static_expect(std::is_same<stack, memory_traits<const int&&>::type>{});
  static_expect(std::is_same<heap, memory_traits<i>::type>{});

#if __has_include(<boost / shared_ptr.hpp>)
  static_expect(std::is_same<heap, memory_traits<boost::shared_ptr<int>>::type>{});
  static_expect(std::is_same<heap, memory_traits<const boost::shared_ptr<int>&>::type>{});
#endif
};

}  // type_traits
