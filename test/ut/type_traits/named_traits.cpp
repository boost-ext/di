//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/named_traits.hpp"
#include <memory>
#include <type_traits>

namespace type_traits {

test remove_named_traits = [] {
  static_expect(std::is_same<int, remove_named_t<int>>::value);
  static_expect(std::is_same<const int*, remove_named_t<const int*>>::value);
  static_expect(std::is_same<int, remove_named_t<named<void, int>>>::value);
  static_expect(std::is_same<int, remove_named_t<named<no_name, int>>>::value);
  static_expect(std::is_same<const int&, remove_named_t<named<no_name, const int&>>>::value);
};

test add_named_traits = [] {
  static_expect(std::is_same<named<no_name, int>, add_named_t<int>>::value);
  static_expect(std::is_same<named<void, int>, add_named_t<named<void, int>>>::value);
  static_expect(std::is_same<named<no_name, int&>, add_named_t<named<no_name, int&>>>::value);
  static_expect(std::is_same<named<no_name, const int&>, add_named_t<const int&>>::value);
};

test named_decay_traits = [] {
  static_expect(std::is_same<int, named_decay_t<int>>::value);
  static_expect(std::is_same<int, named_decay_t<const int&>>::value);
  static_expect(std::is_same<named<void, int>, named_decay_t<named<void, int>>>::value);
  static_expect(std::is_same<named<void, int>, named_decay_t<named<void, const int&>>>::value);
  static_expect(std::is_same<named<no_name, int>, named_decay_t<named<no_name, int*>>>::value);
};

}  // namespace type_traits
