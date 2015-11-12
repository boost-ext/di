//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <new>
#include <cassert>
#include <utility>
#include <boost/di.hpp>

namespace di = boost::di;

class heap_no_throw {
 public:
  template <class...>
  struct is_creatable {
    static constexpr auto value = true;
  };

  template <class T  // implementation
            ,
            class TInit  // direct()/uniform{}
            ,
            class TMemory  // heap/stack
            ,
            class... TArgs>
  auto get(const TInit&, const TMemory&, TArgs&&... args) const noexcept {
    return new (std::nothrow) T{std::forward<TArgs>(args)...};
  }
};

class my_provider : public di::config {
 public:
  template <class T>
  static auto provider(const T&) noexcept {
    return heap_no_throw{};
  }
};

int main() {
  auto injector = di::make_injector<my_provider>();
  assert(0 == injector.create<int>());
}
