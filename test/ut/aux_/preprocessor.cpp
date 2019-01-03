//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/aux_/preprocessor.hpp"
#include <string>

test cat = [] {
#define CAT __BOOST_DI_CAT(in, t)
  CAT i = 0;
#undef CAT
  expect(!i);
};

test conditional = [] {
  expect(false == __BOOST_DI_IF(0, true, false));
  expect(true == __BOOST_DI_IF(1, true, false));
};

test args_size = [] {
  expect(1 == __BOOST_DI_SIZE());
  expect(1 == __BOOST_DI_SIZE(p1));
  expect(3 == __BOOST_DI_SIZE(p1, p2, p3));
  expect(10 == __BOOST_DI_SIZE(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10));
};

test is_bracket = [] {
  expect(!__BOOST_DI_IBP());
  expect(__BOOST_DI_IBP(()));
  expect(__BOOST_DI_IBP((named = name)));
  expect(__BOOST_DI_IBP((named = name) int i = 0));
};

test arg = [] {
  expect(1 == __BOOST_DI_ELEM(0, 1, 2, 3, ...));
  expect(2 == __BOOST_DI_ELEM(1, 1, 2, 3, ...));
  expect(3 == __BOOST_DI_ELEM(2, 1, 2, 3, ...));
};

test eval_repeat = [] {
#define Q_IMPL(...) \
  std::string { #__VA_ARGS__ }
#define Q(...) Q_IMPL(__VA_ARGS__)
#define M(i, ...) i
  expect(std::string{"0"} == Q(__BOOST_DI_REPEAT(1, M, ...)));
  expect(std::string{"0 1"} == Q(__BOOST_DI_REPEAT(2, M, ...)));
  expect(std::string{"0 1 2"} == Q(__BOOST_DI_REPEAT(3, M, ...)));
#undef M
#undef Q
#undef Q_IMPL
};
