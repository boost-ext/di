//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_INJECT_HPP
#define BOOST_DI_INJECT_HPP

#include "boost/di/aux_/utility.hpp"

template<class>
struct traits;

template<class T>
struct traits<void(T)> {
    using type = T;
};

#if !defined(BOOST_DI_INJECTOR)
    #define BOOST_DI_INJECTOR boost_di_injector__
#endif

#if !defined(BOOST_DI_CFG_CTOR_LIMIT_SIZE)
    #define BOOST_DI_CFG_CTOR_LIMIT_SIZE 10
#endif

#define BOOST_DI_TYPE(v1, v2) v2
#define BOOST_DI_NAME(v1, v2) di::named<v1, typename traits<void(v2)>::type>

#define BOOST_DI_ARG(expr, p) BOOST_DI_IF(BOOST_DI_IBP(p))(expr p, p)

#define BOOST_DI_INJECT_IMPL1(expr)

#define BOOST_DI_INJECT_IMPL2(expr, p1) \
    BOOST_DI_ARG(expr, p1)

#define BOOST_DI_INJECT_IMPL3(expr, p1, p2) \
    BOOST_DI_ARG(expr, p1) \
  , BOOST_DI_ARG(expr, p2)

#define BOOST_DI_INJECT_IMPL4(expr, p1, p2, p3) \
    BOOST_DI_ARG(expr, p1) \
  , BOOST_DI_ARG(expr, p2) \
  , BOOST_DI_ARG(expr, p3)

#define BOOST_DI_INJECT_IMPL5(expr, p1, p2, p3, p4) \
    BOOST_DI_ARG(expr, p1) \
  , BOOST_DI_ARG(expr, p2) \
  , BOOST_DI_ARG(expr, p3) \
  , BOOST_DI_ARG(expr, p4)

#define BOOST_DI_INJECT_IMPL6(expr, p1, p2, p3, p4, p5) \
    BOOST_DI_ARG(expr, p1) \
  , BOOST_DI_ARG(expr, p2) \
  , BOOST_DI_ARG(expr, p3) \
  , BOOST_DI_ARG(expr, p4) \
  , BOOST_DI_ARG(expr, p5)

#define BOOST_DI_INJECT_IMPL7(expr, p1, p2, p3, p4, p5, p6) \
    BOOST_DI_ARG(expr, p1) \
  , BOOST_DI_ARG(expr, p2) \
  , BOOST_DI_ARG(expr, p3) \
  , BOOST_DI_ARG(expr, p4) \
  , BOOST_DI_ARG(expr, p5) \
  , BOOST_DI_ARG(expr, p6)

#define BOOST_DI_INJECT_IMPL8(expr, p1, p2, p3, p4, p5, p6, p7) \
    BOOST_DI_ARG(expr, p1) \
  , BOOST_DI_ARG(expr, p2) \
  , BOOST_DI_ARG(expr, p3) \
  , BOOST_DI_ARG(expr, p4) \
  , BOOST_DI_ARG(expr, p5) \
  , BOOST_DI_ARG(expr, p6) \
  , BOOST_DI_ARG(expr, p7)

#define BOOST_DI_INJECT_IMPL9(expr, p1, p2, p3, p4, p5, p6, p7, p8) \
    BOOST_DI_ARG(expr, p1) \
  , BOOST_DI_ARG(expr, p2) \
  , BOOST_DI_ARG(expr, p3) \
  , BOOST_DI_ARG(expr, p4) \
  , BOOST_DI_ARG(expr, p5) \
  , BOOST_DI_ARG(expr, p6) \
  , BOOST_DI_ARG(expr, p7) \
  , BOOST_DI_ARG(expr, p8)

#define BOOST_DI_INJECT_IMPL10(expr, p1, p2, p3, p4, p5, p6, p7, p8, p9) \
    BOOST_DI_ARG(expr, p1) \
  , BOOST_DI_ARG(expr, p2) \
  , BOOST_DI_ARG(expr, p3) \
  , BOOST_DI_ARG(expr, p4) \
  , BOOST_DI_ARG(expr, p5) \
  , BOOST_DI_ARG(expr, p6) \
  , BOOST_DI_ARG(expr, p7) \
  , BOOST_DI_ARG(expr, p8) \
  , BOOST_DI_ARG(expr, p9)

#define BOOST_DI_INJECT_IMPL11(expr, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    BOOST_DI_ARG(expr, p1) \
  , BOOST_DI_ARG(expr, p2) \
  , BOOST_DI_ARG(expr, p3) \
  , BOOST_DI_ARG(expr, p4) \
  , BOOST_DI_ARG(expr, p5) \
  , BOOST_DI_ARG(expr, p6) \
  , BOOST_DI_ARG(expr, p7) \
  , BOOST_DI_ARG(expr, p8) \
  , BOOST_DI_ARG(expr, p9) \
  , BOOST_DI_ARG(expr, p10)

#if !defined(BOOST_DI_INJECT_TRAITS)
    #define BOOST_DI_INJECT_TRAITS(...) \
        static void BOOST_DI_INJECTOR( \
            BOOST_DI_VAR_ARG(BOOST_DI_INJECT_IMPL, BOOST_DI_NAME, __VA_ARGS__) \
        )
#endif

#if !defined(BOOST_DI_INJECT)
    #define BOOST_DI_INJECT(type, ...) \
        BOOST_DI_INJECT_TRAITS(__VA_ARGS__); \
        type(BOOST_DI_VAR_ARG(BOOST_DI_INJECT_IMPL, BOOST_DI_TYPE, __VA_ARGS__))
#endif

#endif

