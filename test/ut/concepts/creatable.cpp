//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/creatable.hpp"
#include "boost/di/aux_/compiler.hpp"

namespace concepts {

struct d {
  d(int, double) {}
};

struct u {
  int i = {};
  double d = {};
};

test is_creatable = [] {
  static_expect(creatable<type_traits::direct, int>::value);
  static_expect(creatable<type_traits::uniform, int>::value);
  static_expect(creatable<type_traits::direct, d, int, double>::value);
  static_expect(!creatable<type_traits::direct, d, int>::value);
  static_expect(creatable<type_traits::uniform, d, int, double>::value);
  static_expect(!creatable<type_traits::uniform, d, int>::value);
  static_expect(!creatable<type_traits::direct, u, int, double>::value);
  static_expect(creatable<type_traits::uniform, u>::value);
  static_expect(!creatable<type_traits::uniform, u, float, short>::value);

#if !defined(__MSVC__)
  static_expect(creatable<type_traits::uniform, u, int, double>::value);
  static_expect(creatable<type_traits::uniform, u, int>::value);
#endif
};

}  // namespace concepts
