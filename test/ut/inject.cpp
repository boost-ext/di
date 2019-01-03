//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/inject.hpp"

test empty_ctor = [] {
  struct c {
    BOOST_DI_INJECT(c, ) {}
  };

  void(c{});
};

test empty_traits = [] {
  struct c {
    BOOST_DI_INJECT_TRAITS();
    c() {}
  };

  c c_;
};

struct c_def {
  static constexpr auto N = 42;
  BOOST_DI_INJECT(explicit c_def, int v = N) : i(v) {}
  int i = 0;
};

test explicit_with_default = [] { expect(c_def::N == c_def{}.i); };

test set_to_default = [] {
  struct c {
    BOOST_DI_INJECT(c, ) = default;
    int i = 0;
  };

  c c_;

  expect(0 == c_.i);
};

test params = [] {
  struct c {
    BOOST_DI_INJECT(c, int i, double d) : i(i), d(d) {}
    int i = 0;
    double d = 0.0;
  };

  constexpr auto i = 1;
  constexpr auto d = 2.0;

  c c_(i, d);

  expect(i == c_.i);
  expect(d == c_.d);
};

test traits = [] {
  constexpr auto i = 1;
  constexpr auto d = 2.0;

  struct c {
    BOOST_DI_INJECT_TRAITS(int i, double d);
    c(int i, double d) : i(i), d(d) {}

    int i = 0;
    double d = 0.0;
  };

  c c_(i, d);

  expect(i == c_.i);
  expect(d == c_.d);
};

test inheriting_ctors = [] {
  constexpr auto i = 1;
  constexpr auto d = 2.0;

  struct c0 {
    BOOST_DI_INJECT(c0, int i, double d) : i(i), d(d) {}
    int i = 0;
    double d = 0.0;
  };

  struct c1 : public c0 {
    using c0::c0;
  };

  c1 c1_(i, d);

  expect(i == c1_.i);
  expect(d == c1_.d);
};

test c_traits_no_limits = [] {
  struct c_no_limits {
    using boost_di_inject__ __BOOST_DI_UNUSED = inject<int, int, int, int, int, int, int, int, int, int, int>;
    c_no_limits(int, int, int, int, int, int, int, int, int, int, int) {}
  };

  c_no_limits object{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};

struct named_int_t {
} named_int;

test named_param = [] {
  constexpr auto i = 42;

  struct c {
    BOOST_DI_INJECT(explicit c, (named = named_int) int i) : i(i) {}
    int i = 0;
  };

  expect(i == c{i}.i);
};

struct c_def_named {
  static constexpr auto N = 42;
  BOOST_DI_INJECT(explicit c_def_named, (named = named_int) int i1 = N, int i2 = N) : i1(i1), i2(i2) {}
  int i1 = 0;
  int i2 = 0;
};

test named_def_param = [] {
  expect(c_def_named::N == c_def_named{}.i1);
  expect(c_def_named::N == c_def_named{}.i2);
};

struct c_def_named_without_def {
  static constexpr auto N = 42;
  BOOST_DI_INJECT(explicit c_def_named_without_def, int i1 = N, (named = named_int) int i2 = N) : i1(i1), i2(i2) {}
  int i1 = 0;
  int i2 = 0;
};

test named_def_param_without_def = [] {
  expect(c_def_named_without_def::N == c_def_named_without_def{c_def_named_without_def::N}.i1);
  expect(c_def_named_without_def::N == c_def_named_without_def{c_def_named_without_def::N}.i2);
};

test named_normal_ctor = [] {
  struct c {
    BOOST_DI_INJECT(c, (named = named_int) int i) : i(i) {}
    int i = 0;
  };

  expect(42 == c{42}.i);
};
