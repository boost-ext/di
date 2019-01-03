//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/callable.hpp"
#include <type_traits>

namespace concepts {

struct non_callable_type {};

struct non_match_callable_type {
  void operator()();
};

struct non_match_callable_type_specific {
  void operator()(int);
};

struct callable_type {
  template <class T>
  void operator()(const T&) const;
};

struct callable_type_return {
  template <class T>
  int operator()(T) const;
};

struct callable_type_extended {
  template <class T, class U>
  void operator()(T, U) const noexcept;
};

test is_concept_callable = [] {
  static_expect(std::is_same<policy<int>::requires_<call_operator_with_one_argument>, callable<int>>::value);
  static_expect(
      std::is_same<policy<non_callable_type>::requires_<call_operator_with_one_argument>, callable<non_callable_type>>::value);
  static_expect(std::is_same<policy<non_match_callable_type>::requires_<call_operator_with_one_argument>,
                             callable<non_match_callable_type>>::value);
  static_expect(std::is_same<policy<non_match_callable_type_specific>::requires_<call_operator_with_one_argument>,
                             callable<non_match_callable_type_specific>>::value);

  static_expect(callable<callable_type>::value);
  static_expect(callable<callable_type_return>::value);
  static_expect(!callable<callable_type_extended>::value);
};

}  // namespace concepts
