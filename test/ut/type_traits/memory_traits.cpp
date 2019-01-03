//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/memory_traits.hpp"
#include <memory>
#include <type_traits>
#if __has_include(<boost / shared_ptr.hpp>)
#include <boost/shared_ptr.hpp>
#endif
#include "common/common.hpp"

namespace type_traits {

struct c {};
struct i {
  virtual ~i() noexcept = default;
};

template <class T>
using memory_traits_t = typename memory_traits<T>::type;

test traits = [] {
  static_expect(std::is_same<stack, memory_traits_t<int>>{});
  static_expect(std::is_same<stack, memory_traits_t<c>>{});
  static_expect(std::is_same<stack, memory_traits_t<const int&>>{});
  static_expect(std::is_same<heap, memory_traits_t<int*>>{});
  static_expect(std::is_same<heap, memory_traits_t<const int*>>{});
  static_expect(std::is_same<heap, memory_traits_t<std::shared_ptr<int>>>{});
  static_expect(std::is_same<heap, memory_traits_t<const std::shared_ptr<int>&>>{});
  static_expect(std::is_same<heap, memory_traits_t<std::weak_ptr<int>>>{});
  static_expect(std::is_same<heap, memory_traits_t<const std::weak_ptr<int>&>>{});
  static_expect(std::is_same<stack, memory_traits_t<int&>>{});
  static_expect(std::is_same<heap, memory_traits_t<std::unique_ptr<int>>>{});
  static_expect(std::is_same<heap, memory_traits_t<std::unique_ptr<int, deleter<int>>>>{});
  static_expect(std::is_same<heap, memory_traits_t<std::shared_ptr<int>>>{});
  static_expect(std::is_same<heap, memory_traits_t<std::weak_ptr<int>>>{});
  static_expect(std::is_same<stack, memory_traits_t<int&&>>{});
  static_expect(std::is_same<stack, memory_traits_t<const int&&>>{});
  static_expect(std::is_same<heap, memory_traits_t<i>>{});

#if __has_include(<boost / shared_ptr.hpp>)
  static_expect(std::is_same<heap, memory_traits_t<boost::shared_ptr<int>>>{});
  static_expect(std::is_same<heap, memory_traits_t<const boost::shared_ptr<int>&>>{});
#endif
};

}  // namespace type_traits
