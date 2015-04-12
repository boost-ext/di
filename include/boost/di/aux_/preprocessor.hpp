//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_PREPROCESSOR_HPP
#define BOOST_DI_AUX_PREPROCESSOR_HPP

#define BOOST_PP_EMPTY()
#define BOOST_PP_DETAIL_VD_IBP_CAT(a, b) BOOST_PP_DETAIL_VD_IBP_CAT_I(a, b)
#define BOOST_PP_DETAIL_VD_IBP_CAT_I(a, b) BOOST_PP_DETAIL_VD_IBP_CAT_II(a ## b)
#define BOOST_PP_DETAIL_VD_IBP_CAT_II(res) res
#define BOOST_PP_DETAIL_IBP_SPLIT(i, ...) BOOST_PP_DETAIL_VD_IBP_CAT(BOOST_PP_DETAIL_IBP_PRIMITIVE_CAT(BOOST_PP_DETAIL_IBP_SPLIT_,i)(__VA_ARGS__),BOOST_PP_EMPTY())
#define BOOST_PP_DETAIL_IBP_IS_VARIADIC_C(...) 1 1
#define BOOST_PP_DETAIL_IBP_SPLIT_0(a, ...) a
#define BOOST_PP_DETAIL_IBP_SPLIT_1(a, ...) __VA_ARGS__
#define BOOST_PP_DETAIL_IBP_CAT(a, ...) BOOST_PP_DETAIL_IBP_PRIMITIVE_CAT(a,__VA_ARGS__)
#define BOOST_PP_DETAIL_IBP_PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__
#define BOOST_PP_DETAIL_IBP_IS_VARIADIC_R_1 1,
#define BOOST_PP_DETAIL_IBP_IS_VARIADIC_R_BOOST_PP_DETAIL_IBP_IS_VARIADIC_C 0,
#define BOOST_DI_IBP(...) BOOST_PP_DETAIL_IBP_SPLIT(0, BOOST_PP_DETAIL_IBP_CAT(BOOST_PP_DETAIL_IBP_IS_VARIADIC_R_, BOOST_PP_DETAIL_IBP_IS_VARIADIC_C __VA_ARGS__))
#define BOOST_DI_CAT(a, ...) BOOST_DI_PRIMITIVE_CAT(a, __VA_ARGS__)
#define BOOST_DI_CALL(m, ...) m(__VA_ARGS__)
#define BOOST_DI_EMPTY()
#define BOOST_DI_COMMA() ,
#define BOOST_DI_EAT(...)
#define BOOST_DI_EXPAND(...) __VA_ARGS__
#define BOOST_DI_NARG_(...) BOOST_DI_ARG_N(__VA_ARGS__)
#define BOOST_DI_NARG(...) BOOST_DI_CAT(BOOST_PP_VARIADIC_SIZE_I(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,),)
#define BOOST_PP_VARIADIC_SIZE_I(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63, size, ...) size
#define BOOST_DI_PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__
#define BOOST_DI_TRIGGER_PARENTHESIS(...) ,
#define BOOST_DI_IS_EMPTY(...)  \
    BOOST_DI_IS_EMPTY_IMPL(BOOST_DI_HAS_COMMA(__VA_ARGS__) \
           , BOOST_DI_HAS_COMMA(BOOST_DI_TRIGGER_PARENTHESIS __VA_ARGS__) \
           , BOOST_DI_HAS_COMMA(__VA_ARGS__ ()) \
           , BOOST_DI_HAS_COMMA(BOOST_DI_TRIGGER_PARENTHESIS __VA_ARGS__ ()))
#define BOOST_DI_IS_EMPTY_IMPL(_0, _1, _2, _3) \
    BOOST_DI_HAS_COMMA(BOOST_DI_JOIN_5(BOOST_DI_IS_EMPTY_IMPL_, _0, _1, _2, _3))
#define BOOST_DI_IF(cond, t, f) BOOST_PP_IF_I(cond, t, f)
# define BOOST_PP_IF_I(cond, t, f) BOOST_PP_IIF(cond, t, f)
#define BOOST_PP_IIF(bit, t, f) BOOST_PP_IIF_OO((bit, t, f))
#define BOOST_PP_IIF_OO(par) BOOST_PP_IIF_I ## par
#define BOOST_PP_IIF_I(bit, t, f) BOOST_PP_IIF_ ## bit(t, f)
#define BOOST_PP_IIF_0(t, f) f
#define BOOST_PP_IIF_1(t, f) t
#define BOOST_PP_IIF_2(t, f) t
#define BOOST_PP_IIF_3(t, f) t
#define BOOST_PP_IIF_4(t, f) t
#define BOOST_PP_IIF_5(t, f) t
#define BOOST_PP_IIF_6(t, f) t
#define BOOST_PP_IIF_7(t, f) t
#define BOOST_PP_IIF_8(t, f) t
#define BOOST_PP_IIF_9(t, f) t
#define BOOST_DI_JOIN_5(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4
#define BOOST_DI_IS_EMPTY_IMPL_0001 ,
#define BOOST_DI_ARG_N( _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, N, ...) N
#define BOOST_DI_HAS_COMMA(...) BOOST_DI_ARG_N(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0)
#define BOOST_DI_ARG(n, ...) BOOST_DI_ARG_I(n,__VA_ARGS__)
#define BOOST_DI_ARG_I(n, ...) BOOST_DI_CAT(BOOST_DI_CAT(BOOST_DI_ARG, n)(__VA_ARGS__,),)
#define BOOST_DI_ARG0(p1, ...) p1
#define BOOST_DI_ARG1(p1, p2, ...) p2
#define BOOST_DI_ARG2(p1, p2, p3, ...) p3
#define BOOST_DI_ARG3(p1, p2, p3, p4, ...) p4
#define BOOST_DI_ARG4(p1, p2, p3, p4, p5, ...) p5
#define BOOST_DI_ARG5(p1, p2, p3, p4, p5, p6, ...) p6
#define BOOST_DI_ARG6(p1, p2, p3, p4, p5, p6, p7, ...) p7
#define BOOST_DI_ARG7(p1, p2, p3, p4, p5, p6, p7, p8, ...) p8
#define BOOST_DI_ARG8(p1, p2, p3, p4, p5, p6, p7, p8, p9, ...) p9
#define BOOST_DI_ARG9(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, ...) p10
#define BOOST_DI_REPEAT(i, m, ...) BOOST_DI_REPEAT_N(i, m, __VA_ARGS__)
#define BOOST_DI_REPEAT_N(i, m, ...) BOOST_DI_REPEAT_##i(m, __VA_ARGS__)
#define BOOST_DI_REPEAT_1(m, ...) m(0, __VA_ARGS__)
#define BOOST_DI_REPEAT_2(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__)
#define BOOST_DI_REPEAT_3(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__)
#define BOOST_DI_REPEAT_4(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__)
#define BOOST_DI_REPEAT_5(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__) m(4, __VA_ARGS__)
#define BOOST_DI_REPEAT_6(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__) m(4, __VA_ARGS__) m(5, __VA_ARGS__)
#define BOOST_DI_REPEAT_7(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__) m(4, __VA_ARGS__) m(5, __VA_ARGS__) m(6, __VA_ARGS__)
#define BOOST_DI_REPEAT_8(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__) m(4, __VA_ARGS__) m(5, __VA_ARGS__) m(6, __VA_ARGS__) m(7, __VA_ARGS__)
#define BOOST_DI_REPEAT_9(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__) m(4, __VA_ARGS__) m(5, __VA_ARGS__) m(6, __VA_ARGS__) m(7, __VA_ARGS__) m(8, __VA_ARGS__)
#define BOOST_DI_REPEAT_10(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__) m(4, __VA_ARGS__) m(5, __VA_ARGS__) m(6, __VA_ARGS__) m(7, __VA_ARGS__) m(8, __VA_ARGS__) m(9, __VA_ARGS__)

#endif

