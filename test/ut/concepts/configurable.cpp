//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/configurable.hpp"
#include <type_traits>
#include "boost/di/aux_/compiler.hpp"
#include "boost/di/config.hpp"
#include "boost/di/providers/stack_over_heap.hpp"

namespace concepts {

test none = [] {
  class test_config {};
  static_expect(std::is_same<config<test_config>::requires_<provider<providable_type(...)>, policies<callable_type(...)>>,
                             configurable<test_config>>::value);
};

class config_just_policies {
 public:
  static auto policies(...) noexcept { return make_policies(); }
};

test just_policies = [] {
  static_expect(
      std::is_same<config<config_just_policies>::requires_<provider<providable_type(...)>, policies<callable_type(...)>>,
                   configurable<config_just_policies>>::value);
};

class config_just_provider {
 public:
  static auto provider(...) noexcept { return providers::stack_over_heap{}; }
};

test just_provider = [] {
  static_expect(
      std::is_same<config<config_just_provider>::requires_<provider<providable_type(...)>, policies<callable_type(...)>>,
                   configurable<config_just_provider>>::value);
};

class config_private_access {
 private:
  static auto policies(...) noexcept { return make_policies(); }
  static auto provider(...) noexcept { return providers::stack_over_heap{}; }
};

#if !defined(__MSVC__)
test private_access = [] {
  static_expect(
      std::is_same<config<config_private_access>::requires_<provider<providable_type(...)>, policies<callable_type(...)>>,
                   configurable<config_private_access>>::value);
};
#endif

class config_inheritance_impl {
 public:
  static auto policies(...) noexcept { return make_policies(); }
  static auto provider(...) noexcept { return providers::stack_over_heap{}; }
};

class config_inheritance : public config_inheritance_impl {};

test inheritance = [] { static_expect(configurable<config_inheritance>::value); };

class config_okay {
 public:
  static auto policies(...) noexcept { return make_policies(); }
  static auto provider(...) noexcept { return providers::stack_over_heap{}; }
};

test okay = [] { static_expect(configurable<config_okay>::value); };

class config_okay_type {
 public:
  template <class T>
  static auto policies(const T&) noexcept {
    return make_policies();
  }

  template <class T>
  static auto provider(const T&) noexcept {
    return providers::stack_over_heap{};
  }
};

test okay_type = [] { static_expect(configurable<config_okay_type>::value); };

}  // namespace concepts
