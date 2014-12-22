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
template<class>
struct named_ { };
}} // boost::di

#if !defined(BOOST_DI_INJECTOR)
    #define BOOST_DI_INJECTOR boost_di_injector__
#endif

#if !defined(BOOST_DI_CFG_CTOR_LIMIT_SIZE)
    #define BOOST_DI_CFG_CTOR_LIMIT_SIZE 10
#endif

#define BOOST_DI_TYPE(x)
#define BOOST_DI_TYPE_END()
#define BOOST_DI_TYPE_X(x) x
#define BOOST_DI_TYPE_END_X()
#define BOOST_DI_FUN(x) static auto name__() { auto x{}; return named; } static void arg__(
#define BOOST_DI_FUN_END() );
#define BOOST_DI_F(x) static void arg__(x);
#define BOOST_DI_F_END()
#define BOOST_DI_X(x)
#define BOOST_DI_ARG(n, expr1, expr2, x, y, p)  BOOST_DI_IF(BOOST_DI_IBP(p))(struct arg##n { expr1 p x() };, )
#define BOOST_DI_ARG2(n, expr1, expr2, x, y, p) BOOST_DI_IF(BOOST_DI_IBP(p))(expr1 p x(), expr2(p) y())
#define BOOST_DI_ARGX(n, expr1, expr2, x, y, p) BOOST_DI_IF(BOOST_DI_IBP(p))(::boost::di::named_<arg##n>, p)
#define BOOST_DI_INJECT_IMPL7(c, m, expr1, x, expr2, y, p1) m(1, expr1, expr2, x, y, p1)
#define BOOST_DI_INJECT_IMPL8(c, m, expr1, x, expr2, y, p1, p2) m(1, expr1, expr2, x, y, p1) c() m(2, expr1, expr2, x, y, p2)
#define BOOST_DI_INJECT_IMPL9(c, m, expr1, x, expr2, y, p1, p2, p3) m(1, expr1, expr2, x, y, p1) c() m(2, expr1, expr2, x, y, p2) c() m(3, expr1, expr2, x, y, p3)
#define BOOST_DI_INJECT_IMPL10(c, m, expr1, x, expr2, y, p1, p2, p3, p4) m(1, expr1, expr2, x, y, p1) c() m(2, expr1, expr2, x, y, p2) c() m(3, expr1, expr2, x, y, p3) c() m(4, expr1, expr2, x, y, p4)
#define BOOST_DI_INJECT_IMPL11(c, m, expr1, x, expr2, y, p1, p2, p3, p4, p5) m(1, expr1, expr2, x, y, p1) c() m(2, expr1, expr2, x, y, p2) c() m(3, expr1, expr2, x, y, p3) c() m(4, expr1, expr2, x, y, p4) c() m(5, expr1, expr2, x, y, p5)
#define BOOST_DI_INJECT_IMPL12(c, m, expr1, x, expr2, y, p1, p2, p3, p4, p5, p6) m(1, expr1, expr2, x, y, p1) c() m(2, expr1, expr2, x, y, p2) c() m(3, expr1, expr2, x, y, p3) c() m(4, expr1, expr2, x, y, p4) c() m(5, expr1, expr2, x, y, p5) c() m(6, expr1, expr2, x, y, p6)
#define BOOST_DI_INJECT_IMPL13(c, m, expr1, x, expr2, y, p1, p2, p3, p4, p5, p6, p7) m(1, expr1, expr2, x, y, p1) c() m(2, expr1, expr2, x, y, p2) c() m(3, expr1, expr2, x, y, p3) c() m(4, expr1, expr2, x, y, p4) c() m(5, expr1, expr2, x, y, p5) c() m(6, expr1, expr2, x, y, p6) c() m(7, expr1, expr2, x, y, p7)
#define BOOST_DI_INJECT_IMPL14(c, m, expr1, x, expr2, y, p1, p2, p3, p4, p5, p6, p7, p8) m(1, expr1, expr2, x, y, p1) c() m(2, expr1, expr2, x, y, p2) c() m(3, expr1, expr2, x, y, p3) c() m(4, expr1, expr2, x, y, p4) c() m(5, expr1, expr2, x, y, p5) c() m(6, expr1, expr2, x, y, p6) c() m(7, expr1, expr2, x, y, p7) c() m(8, expr1, expr2, x, y, p8)
#define BOOST_DI_INJECT_IMPL15(c, m, expr1, x, expr2, y, p1, p2, p3, p4, p5, p6, p7, p8, p9) m(1, expr1, expr2, x, y, p1) c() m(2, expr1, expr2, x, y, p2) c() m(3, expr1, expr2, x, y, p3) c() m(4, expr1, expr2, x, y, p4) c() m(5, expr1, expr2, x, y, p5) c() m(6, expr1, expr2, x, y, p6) c() m(7, expr1, expr2, x, y, p7) c() m(8, expr1, expr2, x, y, p8) c() m(9, expr1, expr2, x, y, p9)
#define BOOST_DI_INJECT_IMPL16(c, m, expr1, x, expr2, y, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) m(1, expr1, expr2, x, y, p1) c() m(2, expr1, expr2, x, y, p2) c() m(3, expr1, expr2, x, y, p3) c() m(4, expr1, expr2, x, y, p4) c() m(5, expr1, expr2, x, y, p5) c() m(6, expr1, expr2, x, y, p6) c() m(7, expr1, expr2, x, y, p7) c() m(8, expr1, expr2, x, y, p8) c() m(9, expr1, expr2, x, y, p9) c() m(10, expr1, expr2, x, y, p10)

#if !defined(BOOST_DI_INJECT_TRAITS)
    #define BOOST_DI_INJECT_TRAITS(...) \
		BOOST_DI_VAR_ARG(BOOST_DI_INJECT_IMPL, BOOST_DI_NONE, BOOST_DI_ARG, BOOST_DI_FUN, BOOST_DI_FUN_END, BOOST_DI_F, BOOST_DI_F_END, __VA_ARGS__) \
		static void BOOST_DI_INJECTOR(BOOST_DI_VAR_ARG(BOOST_DI_INJECT_IMPL, BOOST_DI_COMMA, BOOST_DI_ARGX, BOOST_DI_X, BOOST_DI_FUN_END, BOOST_DI_F, BOOST_DI_F_END, __VA_ARGS__))
#endif

#if !defined(BOOST_DI_INJECT)
    #define BOOST_DI_INJECT(type, ...) \
        BOOST_DI_INJECT_TRAITS(__VA_ARGS__); \
        type(BOOST_DI_VAR_ARG(BOOST_DI_INJECT_IMPL, BOOST_DI_COMMA, BOOST_DI_ARG2, BOOST_DI_TYPE, BOOST_DI_TYPE_END, BOOST_DI_TYPE_X, BOOST_DI_TYPE_END_X, __VA_ARGS__))
#endif

#endif

