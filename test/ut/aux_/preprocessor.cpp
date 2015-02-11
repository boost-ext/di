//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <string>
#include "boost/di/aux_/preprocessor.hpp"

test cat = [] {
    #define CAT BOOST_DI_CAT(in, t)
        CAT i = 0;
    #undef CAT
    expect(!i);
};

test call = [] {
    #define CAT BOOST_DI_CALL(BOOST_DI_CAT, in, t)
        CAT i = 0;
    #undef CAT
    expect(!i);
};

test conditional = [] {
    expect_eq(false, BOOST_DI_IF(0)(true, false));
    expect_eq(true, BOOST_DI_IF(1)(true, false));
};

test args_size = [] {
    expect_eq(1, BOOST_DI_NARG());
    expect_eq(1, BOOST_DI_NARG(p1));
    expect_eq(3, BOOST_DI_NARG(p1, p2, p3));
    expect_eq(10, BOOST_DI_NARG(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10));
};

test is_parenthesis = [] {
    expect(!BOOST_DI_IBP());
    expect(BOOST_DI_IBP(()));
    expect(BOOST_DI_IBP((named = name)));
    expect(BOOST_DI_IBP((named = name) int i = 0));
};

test arg = [] {
    expect_eq(1, BOOST_DI_ARG0(1, 2, 3, ...));
    expect_eq(2, BOOST_DI_ARG1(1, 2, 3, ...));
    expect_eq(3, BOOST_DI_ARG2(1, 2, 3, ...));
};

test eval_repeat = [] {
    #define Q_IMPL(...) std::string{#__VA_ARGS__}
    #define Q(...) Q_IMPL(__VA_ARGS__)
    #define M(i, ...) i
    expect_eq(std::string{"0"}, Q(BOOST_DI_EVAL(BOOST_DI_REPEAT(1, M, ...))));
    expect_eq(std::string{"0 1"}, Q(BOOST_DI_EVAL(BOOST_DI_REPEAT(2, M, ...))));
    expect_eq(std::string{"0 1 2"}, Q(BOOST_DI_EVAL(BOOST_DI_REPEAT(3, M, ...))));
    #undef M
    #undef Q
    #undef Q_IMPL
};

