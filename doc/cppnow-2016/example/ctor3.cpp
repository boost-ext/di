//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>

template<class TParent>
struct any_type {
  template<class T, class = std::enable_if_t<!std::is_convertible<TParent, T>{}>>
  operator T();
};


int main() {
  struct example {
    example(int, double);
  };

  static_assert(!std::is_constructible<example, any_type<example>>::value);
}
