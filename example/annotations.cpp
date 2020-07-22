//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
//->
#include <boost/di.hpp>

namespace di = boost::di;

auto int_1 = [] {};
struct int_2_t {
} int_2;

class annotations1 {
 public:
  /*<<Constructor with named parameters of the same `int` type>>*/
  BOOST_DI_INJECT(annotations1, (named = int_1) int i1, (named = int_2) int i2, int i3) : i1(i1), i2(i2), i3(i3) {
    assert(i1 == 42);
    assert(i2 == 87);
    assert(i3 == 123);
  }

 private:
  int i1 = 0;
  int i2 = 0;
  int i3 = 0;
};

class annotations2 {
 public:
  /*<<Constructor with named parameters of the same `int` type>>*/
  BOOST_DI_INJECT(annotations2, (named = int_1) int i1, (named = int_2) int i2, int i3);

 private:
  int i1 = 0;
  int i2 = 0;
  int i3 = 0;
};

annotations2::annotations2(int i1, int i2, int i3) {
  assert(i1 == 42);
  assert(i2 == 87);
  assert(i3 == 123);
}

//<-
#if !defined(__MSVC__)
#if defined(__CLANG__)
#pragma clang diagnostic ignored "-Wgnu-string-literal-operator-template"
#endif

template <char...>
struct string {};

#if (__GNUC__ >= 9)
#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-Wpedantic"
#endif
template <class T, T... Chars>
constexpr auto operator""_s() {
  return string<Chars...>{};
}
#if (__GNUC__ >= 9)
#pragma GCC diagnostic pop
#endif
#endif
//->

//<-
#if !defined(__MSVC__)
//->
class annotations3 {
 public:
  /*<<Constructor with named parameters of the same `int` type>>*/
  BOOST_DI_INJECT(annotations3, (named = "int1"_s) int i1, (named = "int2"_s) int i2, int i3) : i1(i1), i2(i2), i3(i3) {
    assert(i1 == 42);
    assert(i2 == 87);
    assert(i3 == 123);
  }

 private:
  int i1 = 0;
  int i2 = 0;
  int i3 = 0;
};
//<-
#endif
//->

int main() {
  {
    /*<<make injector and bind named parameters>>*/
    // clang-format off
    auto injector = di::make_injector(
      di::bind<int>().named(int_1).to(42)
    , di::bind<int>().named(int_2).to(87)
    , di::bind<int>().to(123)
    );
    // clang-format on

    /*<<create `annotations`>>*/
    injector.create<annotations1>();
    injector.create<annotations2>();
  }

//<-
#if !defined(__MSVC__)
  //->
  {
    /*<<make injector and bind named parameters>>*/
    // clang-format off
    auto injector = di::make_injector(
      di::bind<int>().named("int1"_s).to(42)
    , di::bind<int>().named("int2"_s).to(87)
    , di::bind<int>().to(123)
    );
    // clang-format on

    /*<<create `annotations`>>*/
    injector.create<annotations3>();
  }
//<-
#endif
  //->
}
