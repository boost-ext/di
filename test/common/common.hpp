//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_COMMON_HPP
#define BOOST_DI_COMMON_HPP

#include <functional>
#include <utility>

template <class T>
using owner = T;

template <class T>
struct function : std::function<T> {
  template <class U, class = decltype(std::declval<U>()())>
  function(const U& f)  // non explicit
      : std::function<T>(f) {}
};

template <class T>
struct deleter {
  void operator()(owner<T*> ptr) const noexcept { delete ptr; }
};

#endif
