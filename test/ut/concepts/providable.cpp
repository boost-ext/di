//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/providable.hpp"
#include <type_traits>
#include "boost/di/aux_/compiler.hpp"
#include "boost/di/providers/stack_over_heap.hpp"

namespace concepts {

test none = [] {
  struct none_providable {};
  static_expect(std::is_same<provider<none_providable>::requires_<get, is_creatable>, providable<none_providable>>::value);
};

class provider_private_access {
  template <class...>
  struct is_creatable {
    static constexpr auto value = true;
  };

  template <class, class T, class TInit, class TMemory, class... TArgs>
  T get(const TInit&, const TMemory&, TArgs&&...) const {
    return {};
  }
};

#if !defined(__MSVC__)
test private_access = [] {
  static_expect(std::is_same<provider<provider_private_access>::requires_<get, is_creatable>,
                             providable<provider_private_access>>::value);
};
#endif

class provider_missing_is_creatable {
 public:
  template <class, class T, class TInit, class TMemory, class... TArgs>
  T get(const TInit&, const TMemory&, TArgs&&...) const {
    return {};
  }
};

test missing_is_creatable = [] {
  static_expect(std::is_same<provider<provider_missing_is_creatable>::requires_<get, is_creatable>,
                             providable<provider_missing_is_creatable>>::value);
};

class provider_wrong_get {
 public:
  template <class...>
  struct is_creatable {
    static constexpr auto value = true;
  };

  template <class T>
  T get() const {
    return {};
  }
};

test wrong_get = [] {
  static_expect(
      std::is_same<provider<provider_wrong_get>::requires_<get, is_creatable>, providable<provider_wrong_get>>::value);
};

test providable_providers = [] { static_expect(providable<providers::stack_over_heap>::value); };

}  // namespace concepts
