//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/bindings.hpp"
#include <type_traits>
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/concepts/boundable.hpp"

struct i {
  virtual ~i() noexcept = default;
  virtual void dummy() = 0;
};
struct i1 {
  virtual ~i1() noexcept = default;
  virtual void dummy1() = 0;
};
struct i2 {
  virtual ~i2() noexcept = default;
  virtual void dummy2() = 0;
};
struct impl : i, i1, i2 {
  void dummy() override {}
  void dummy1() override {}
  void dummy2() override {}
};

test bindings_types = [] {
#if defined(__cpp_variable_templates)
  static_expect(std::is_base_of<core::dependency<scopes::deduce>, decltype(bind<>)>{});
  static_expect(std::is_base_of<core::dependency<scopes::deduce, int>, decltype(bind<int>)>{});
  static_expect(std::is_base_of<core::dependency<scopes::deduce, i>, decltype(bind<i>)>{});
  static_expect(std::is_base_of<core::dependency<scopes::deduce, i, impl>, decltype(bind<i>.to<impl>())>{});
  static_expect(
      std::is_base_of<core::dependency<scopes::deduce, concepts::any_of<i1, i2>, impl>, decltype(bind<i1, i2>.to<impl>())>{});
#endif
  static_expect(std::is_base_of<core::dependency<scopes::deduce>, aux::remove_qualifiers_t<decltype(bind<>())>>{});
  static_expect(std::is_base_of<core::dependency<scopes::deduce, int>, aux::remove_qualifiers_t<decltype(bind<int>())>>{});
  static_expect(std::is_base_of<core::dependency<scopes::deduce, i>, aux::remove_qualifiers_t<decltype(bind<i>())>>{});
  static_expect(
      std::is_base_of<core::dependency<scopes::deduce, i, impl>, aux::remove_qualifiers_t<decltype(bind<i>().to<impl>())>>{});
  static_expect(std::is_base_of<core::dependency<scopes::deduce, concepts::any_of<i1, i2>, impl>,
                                aux::remove_qualifiers_t<decltype(bind<i1, i2>().to<impl>())>>{});
};
