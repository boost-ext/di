//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/core/any_type.hpp"
#include "common/fakes/fake_injector.hpp"

namespace core {

test is_referable = [] {
  using injector = fake_injector<>;
  static_expect(!is_referable__<int, injector, aux::false_type>::value);
  static_expect(!is_referable__<int&&, injector, aux::false_type>::value);
  static_expect(is_referable__<int&, injector, aux::false_type>::value);
  static_expect(is_referable__<const int&, injector, aux::false_type>::value);

  static_expect(is_referable__<int, injector, aux::true_type>::value);
  static_expect(is_referable__<int&, injector, aux::true_type>::value);
  static_expect(is_referable__<const int&, injector, aux::true_type>::value);
  static_expect(is_referable__<int&&, injector, aux::true_type>::value);
};

test is_creatable = [] {
  using injector = fake_injector<>;
  static_expect(is_creatable__<int, injector, aux::false_type>::value);
  static_expect(is_creatable__<int, injector, aux::true_type>::value);
};

test is_copy_ctor = [] {
  struct c {};
  static_expect(is_copy_ctor__<int, int>::value);
  static_expect(is_copy_ctor__<c, c>::value);
#if defined(__GCC__) || defined(__MSVC__)
  static_expect(is_copy_ctor__<int, const int>::value);
  static_expect(is_copy_ctor__<c, const c>::value);
  static_expect(is_copy_ctor__<c, c const>::value);
#endif
};

test any_type_create = [] {
  fake_injector<> injector;
  expect(0 == static_cast<int>(any_type<void, fake_injector<>>{injector}));
};

test any_type_ref_create = [] {
  fake_injector<> injector;
  expect(0 == static_cast<int>(any_type_ref<void, fake_injector<>>{injector}));
};

}  // namespace core
