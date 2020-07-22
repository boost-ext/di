//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/ctor_traits.hpp"
#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <type_traits>
#include "boost/di/aux_/compiler.hpp"
#include "boost/di/core/any_type.hpp"
#include "boost/di/inject.hpp"

struct traits {};

template <>
struct ctor_traits<traits> {
  BOOST_DI_INJECT_TRAITS();
};

namespace type_traits {

struct ctor_conv {
  template <class T>
  ctor_conv(T&&);  // non explicit
};

struct ctor_conv_explicit {
  template <class T>
  explicit ctor_conv_explicit(T&&);
};

template <class T, class TInitialization, class... Ts>
void test_ctor_traits() {
  static_expect(
      std::is_same<aux::pair<T, aux::pair<TInitialization, aux::type_list<Ts...>>>, typename ctor_traits__<T, T>::type>::value);
}

test ctors = [] {
  struct empty {
    BOOST_DI_INJECT_TRAITS();
  };

  struct int_double {
    BOOST_DI_INJECT_TRAITS(int, double);
  };

  struct extensions {
    BOOST_DI_INJECT_TRAITS(char*, const int&);
  };

  struct ctor1 {
    explicit ctor1(int);
  };

  struct ctor2 {
    ctor2(char*, const int&);
  };

  struct ctor_complex {
    ctor_complex(int, double&, std::shared_ptr<int>, float&, const char*, const std::string&, void*, std::unique_ptr<int>) {}
  };

  struct ctor_unique_ptr {
    explicit ctor_unique_ptr(std::unique_ptr<int>) {}
  };

  struct rvalue {
    explicit rvalue(int&&){};
  };

  enum e {};
  enum class ec {};

  struct conv {
    conv(int, ctor_conv) {}
  };

  struct conv_explicit {
    conv_explicit(int, ctor_conv_explicit) {}
  };

  struct func {
    explicit func(const std::function<int()>&) {}
  };

  struct vaarg {
    explicit vaarg(int, ...) {}
  };

  test_ctor_traits<empty, direct>();
  test_ctor_traits<empty&, direct>();
  test_ctor_traits<const empty*, direct>();
  test_ctor_traits<traits, direct>();
  test_ctor_traits<empty, direct>();
  test_ctor_traits<int_double, direct, int, double>();
  test_ctor_traits<extensions, direct, char*, const int&>();
  test_ctor_traits<ctor_complex, direct, core::any_type_ref_fwd<ctor_complex>, core::any_type_ref_fwd<ctor_complex>,
                   core::any_type_ref_fwd<ctor_complex>, core::any_type_ref_fwd<ctor_complex>,
                   core::any_type_ref_fwd<ctor_complex>, core::any_type_ref_fwd<ctor_complex>,
                   core::any_type_ref_fwd<ctor_complex>, core::any_type_ref_fwd<ctor_complex>>();
  test_ctor_traits<e, direct>();
  test_ctor_traits<ec, direct>();
  test_ctor_traits<conv, uniform>();
  test_ctor_traits<conv_explicit, direct, core::any_type_fwd<conv_explicit>, core::any_type_fwd<conv_explicit>>();
  test_ctor_traits<vaarg, direct, core::any_type_fwd<vaarg>, core::any_type_fwd<vaarg>, core::any_type_fwd<vaarg>,
                   core::any_type_fwd<vaarg>, core::any_type_fwd<vaarg>, core::any_type_fwd<vaarg>, core::any_type_fwd<vaarg>,
                   core::any_type_fwd<vaarg>, core::any_type_fwd<vaarg>, core::any_type_fwd<vaarg>>();
  test_ctor_traits<ctor1, direct, core::any_type_1st_fwd<ctor1>>();
  test_ctor_traits<ctor_unique_ptr, direct, core::any_type_1st_fwd<ctor_unique_ptr>>();
  test_ctor_traits<ctor_conv, direct, core::any_type_1st_fwd<ctor_conv>>();

  test_ctor_traits<func, direct, core::any_type_1st_ref_fwd<func>>();
  test_ctor_traits<ctor2, direct, core::any_type_ref_fwd<ctor2>, core::any_type_ref_fwd<ctor2>>();

#if defined(__GCC__)
  test_ctor_traits<rvalue, direct, core::any_type_1st_ref_fwd<rvalue>>();
#else
  test_ctor_traits<rvalue, direct, core::any_type_1st_fwd<rvalue>>();
#endif
};

test uniforms = [] {
  struct empty {};
  struct ctor1 {
    int i = 0;
  };
  struct ctor2_ref {
    std::unique_ptr<int> ptr;
    int& i;
  };
  test_ctor_traits<empty, uniform>();

#if !defined(__MSVC__)
  test_ctor_traits<ctor2_ref, uniform, core::any_type_ref_fwd<ctor2_ref>, core::any_type_ref_fwd<ctor2_ref>>();
#endif
};

test inheriting_ctors = [] {
  struct c0 {
    c0(int, double) {}
  };
  struct c1 : public c0 {
    using c0::c0;
  };

  test_ctor_traits<c0, direct, core::any_type_fwd<c0>, core::any_type_fwd<c0>>();
  test_ctor_traits<c1, direct, core::any_type_fwd<c1>, core::any_type_fwd<c1>>();
};

test inheriting_ctors_inject = [] {
  struct c0 {
    c0(int, double) {}
    BOOST_DI_INJECT_TRAITS(int, double);
  };
  struct c1 : public c0 {
    using c0::c0;
  };
  struct c2 : public c0 {};
  struct c3 : public c0 {
    BOOST_DI_INJECT_TRAITS();
  };

  test_ctor_traits<c0, direct, int, double>();
  test_ctor_traits<c1, direct, int, double>();
  test_ctor_traits<c2, direct, int, double>();
  test_ctor_traits<c3, direct>();
};

test special_std_types = [] {
  test_ctor_traits<std::string, direct>();
  test_ctor_traits<std::initializer_list<int>, direct>();
};

}  // namespace type_traits
