//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_INJECT_HPP
#define BOOST_DI_INJECT_HPP

template<class x>
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

#define IBP_SPLIT(i, ...) PRIMITIVE_CAT(IBP_SPLIT_,i)(__VA_ARGS__)
#define IBP_IS_VARIADIC_C(...) 1
#define IBP_SPLIT_0(a, ...) a
#define IBP_SPLIT_1(a, ...) __VA_ARGS__
#define IBP_IS_VARIADIC_R_1 1,
#define IBP_IS_VARIADIC_R_IBP_IS_VARIADIC_C 0,
#define IS_BEGIN_PARENS(...) IBP_SPLIT(0, CAT(IBP_IS_VARIADIC_R_, IBP_IS_VARIADIC_C __VA_ARGS__))
#define CAT(a, ...) PRIMITIVE_CAT(a, __VA_ARGS__)
#define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__
#define IIF(c) PRIMITIVE_CAT(IIF_, c)
#define IIF_0(t, ...) __VA_ARGS__
#define IIF_1(t, ...) t
#define FIRST(v1, v2) v2
#define SECOND(v1, v2) di::named<v1, typename traits<void(v2)>::type>
#define VA_NARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, N, ...) N
#define VA_NARGS(...) VA_NARGS_IMPL(__VA_ARGS__, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define VARARG_IMPL2(base, count, ...) base##count(__VA_ARGS__)
#define VARARG_IMPL(base, count, ...) VARARG_IMPL2(base, count, __VA_ARGS__)
#define VARARG(base, ...) VARARG_IMPL(base, VA_NARGS(__VA_ARGS__), __VA_ARGS__)
#define ARG(expr, p) IIF(IS_BEGIN_PARENS(p))(expr p, p)
#define BOOST_DI_INJECT_IMPL1(expr)
#define BOOST_DI_INJECT_IMPL2(expr, p1) ARG(expr, p1)
#define BOOST_DI_INJECT_IMPL3(expr, p1, p2) ARG(expr, p1), ARG(expr, p2)
#define BOOST_DI_INJECT_IMPL4(expr, p1, p2, p3) ARG(expr, p1), ARG(expr, p2), ARG(expr, p3)
#define BOOST_DI_INJECT_IMPL5(expr, p1, p2, p3, p4) ARG(expr, p1), ARG(expr, p2), ARG(expr, p3), ARG(expr, p4)
#define BOOST_DI_INJECT_IMPL6(expr, p1, p2, p3, p4, p5) ARG(expr, p1), ARG(expr, p2), ARG(expr, p3), ARG(expr, p4), ARG(expr, p5)
#define BOOST_DI_INJECT_IMPL7(expr, p1, p2, p3, p4, p5, p6) ARG(expr, p1), ARG(expr, p2), ARG(expr, p3), ARG(expr, p4), ARG(expr, p5), ARG(expr, p6)
#define BOOST_DI_INJECT_IMPL8(expr, p1, p2, p3, p4, p5, p6, p7) ARG(expr, p1), ARG(expr, p2), ARG(expr, p3), ARG(expr, p4), ARG(expr, p5), ARG(expr, p6), ARG(expr, p7)
#define BOOST_DI_INJECT_IMPL9(expr, p1, p2, p3, p4, p5, p6, p7, p8) ARG(expr, p1), ARG(expr, p2), ARG(expr, p3), ARG(expr, p4), ARG(expr, p5), ARG(expr, p6), ARG(expr, p7), ARG(expr, p8)
#define BOOST_DI_INJECT_IMPL10(expr, p1, p2, p3, p4, p5, p6, p7, p8, p9) ARG(expr, p1), ARG(expr, p2), ARG(expr, p3), ARG(expr, p4), ARG(expr, p5), ARG(expr, p6), ARG(expr, p7), ARG(expr, p8), ARG(expr, p9)
#define BOOST_DI_INJECT_IMPL11(expr, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) ARG(expr, p1), ARG(expr, p2), ARG(expr, p3), ARG(expr, p4), ARG(expr, p5), ARG(expr, p6), ARG(expr, p7), ARG(expr, p8), ARG(expr, p9), ARG(expr, p10)

#if !defined(BOOST_DI_INJECT_TRAITS)
    #define BOOST_DI_INJECT_TRAITS(...) \
        static void BOOST_DI_INJECTOR(VARARG(BOOST_DI_INJECT_IMPL, SECOND, __VA_ARGS__))
#endif

#if !defined(BOOST_DI_INJECT)
    #define BOOST_DI_INJECT(type, ...) \
        BOOST_DI_INJECT_TRAITS(__VA_ARGS__); \
        type(VARARG(BOOST_DI_INJECT_IMPL, FIRST, __VA_ARGS__))
#endif

#endif

