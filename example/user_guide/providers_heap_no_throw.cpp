//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <boost/di.hpp>
#include <cassert>
#include <new>
#include <utility>

namespace di = boost::di;
//->

class heap_no_throw {
 public:
  template <class...>
  struct is_creatable {
    static constexpr auto value = true;
  };

  // clang-format off
  template <class T         // implementation
          , class TInit     // direct()/uniform{}
          , class TMemory   // heap/stack
          , class... TArgs>
  auto get(const TInit&, const TMemory&, TArgs&&... args) const noexcept {
    return new (std::nothrow) T{std::forward<TArgs>(args)...};
  }
  // clang-format on
};

class my_provider : public di::config {
 public:
  static auto provider(...) noexcept { return heap_no_throw{}; }
};

int main() {
  auto injector = di::make_injector<my_provider>();
  assert(0 == injector.create<int>());
}
