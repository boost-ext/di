//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_INJECT_HPP
#define BOOST_DI_INJECT_HPP

#include "boost/di/aux_/utility.hpp"

namespace boost { namespace di {
template<class, class> struct named { };
}} // boost::di

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

#define BOOST_DI_TYPE(x)
#define BOOST_DI_TYPE_END()

#define BOOST_DI_NAME(name) di::named<name, typename traits<void(
#define BOOST_DI_NAME_END() )>::type>

#define BOOST_DI_ARG(expr, x, p) BOOST_DI_IF(BOOST_DI_IBP(p))(expr p x(), p)

#define BOOST_DI_INJECT_IMPL2(expr, x)

#define BOOST_DI_INJECT_IMPL3(expr, x, p1) \
    BOOST_DI_ARG(expr, x, p1)

#define BOOST_DI_INJECT_IMPL4(expr, x, p1, p2) \
    BOOST_DI_ARG(expr, x, p1) \
  , BOOST_DI_ARG(expr, x, p2)

#define BOOST_DI_INJECT_IMPL5(expr, x, p1, p2, p3) \
    BOOST_DI_ARG(expr, x, p1) \
  , BOOST_DI_ARG(expr, x, p2) \
  , BOOST_DI_ARG(expr, x, p3)

#define BOOST_DI_INJECT_IMPL6(expr, x, p1, p2, p3, p4) \
    BOOST_DI_ARG(expr, x, p1) \
  , BOOST_DI_ARG(expr, x, p2) \
  , BOOST_DI_ARG(expr, x, p3) \
  , BOOST_DI_ARG(expr, x, p4)

#define BOOST_DI_INJECT_IMPL7(expr, x, p1, p2, p3, p4, p5) \
    BOOST_DI_ARG(expr, x, p1) \
  , BOOST_DI_ARG(expr, x, p2) \
  , BOOST_DI_ARG(expr, x, p3) \
  , BOOST_DI_ARG(expr, x, p4) \
  , BOOST_DI_ARG(expr, x, p5)

#define BOOST_DI_INJECT_IMPL8(expr, x, p1, p2, p3, p4, p5, p6) \
    BOOST_DI_ARG(expr, x, p1) \
  , BOOST_DI_ARG(expr, x, p2) \
  , BOOST_DI_ARG(expr, x, p3) \
  , BOOST_DI_ARG(expr, x, p4) \
  , BOOST_DI_ARG(expr, x, p5) \
  , BOOST_DI_ARG(expr, x, p6)

#define BOOST_DI_INJECT_IMPL9(expr, x, p1, p2, p3, p4, p5, p6, p7) \
    BOOST_DI_ARG(expr, x, p1) \
  , BOOST_DI_ARG(expr, x, p2) \
  , BOOST_DI_ARG(expr, x, p3) \
  , BOOST_DI_ARG(expr, x, p4) \
  , BOOST_DI_ARG(expr, x, p5) \
  , BOOST_DI_ARG(expr, x, p6) \
  , BOOST_DI_ARG(expr, x, p7)

#define BOOST_DI_INJECT_IMPL10(expr, x, p1, p2, p3, p4, p5, p6, p7, p8) \
    BOOST_DI_ARG(expr, x, p1) \
  , BOOST_DI_ARG(expr, x, p2) \
  , BOOST_DI_ARG(expr, x, p3) \
  , BOOST_DI_ARG(expr, x, p4) \
  , BOOST_DI_ARG(expr, x, p5) \
  , BOOST_DI_ARG(expr, x, p6) \
  , BOOST_DI_ARG(expr, x, p7) \
  , BOOST_DI_ARG(expr, x, p8)

#define BOOST_DI_INJECT_IMPL11(expr, x, p1, p2, p3, p4, p5, p6, p7, p8, p9) \
    BOOST_DI_ARG(expr, x, p1) \
  , BOOST_DI_ARG(expr, x, p2) \
  , BOOST_DI_ARG(expr, x, p3) \
  , BOOST_DI_ARG(expr, x, p4) \
  , BOOST_DI_ARG(expr, x, p5) \
  , BOOST_DI_ARG(expr, x, p6) \
  , BOOST_DI_ARG(expr, x, p7) \
  , BOOST_DI_ARG(expr, x, p8) \
  , BOOST_DI_ARG(expr, x, p9)

#define BOOST_DI_INJECT_IMPL12(expr, x, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    BOOST_DI_ARG(expr, x, p1) \
  , BOOST_DI_ARG(expr, x, p2) \
  , BOOST_DI_ARG(expr, x, p3) \
  , BOOST_DI_ARG(expr, x, p4) \
  , BOOST_DI_ARG(expr, x, p5) \
  , BOOST_DI_ARG(expr, x, p6) \
  , BOOST_DI_ARG(expr, x, p7) \
  , BOOST_DI_ARG(expr, x, p8) \
  , BOOST_DI_ARG(expr, x, p9) \
  , BOOST_DI_ARG(expr, x, p10)

#if !defined(BOOST_DI_INJECT_TRAITS)
    #define BOOST_DI_INJECT_TRAITS(...) \
        static void BOOST_DI_INJECTOR( \
            BOOST_DI_VAR_ARG(BOOST_DI_INJECT_IMPL, BOOST_DI_NAME, BOOST_DI_NAME_END, __VA_ARGS__) \
        )
#endif

#if !defined(BOOST_DI_INJECT)
    #define BOOST_DI_INJECT(type, ...) \
        BOOST_DI_INJECT_TRAITS(__VA_ARGS__); \
        type(BOOST_DI_VAR_ARG(BOOST_DI_INJECT_IMPL, BOOST_DI_TYPE, BOOST_DI_TYPE_END, __VA_ARGS__))
#endif

#endif

