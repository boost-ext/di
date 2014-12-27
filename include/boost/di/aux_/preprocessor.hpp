//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_PREPROCESSOR_HPP
#define BOOST_DI_AUX_PREPROCESSOR_HPP

// mostly from the web!

#define PP_ARG_N( \
          _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, _10, \
         _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
         _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
         _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
         _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
         _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
         _61, _62, _63, N, ...) N

#define PP_RSEQ_N()                                        \
         63, 62, 61, 60,                                   \
         59, 58, 57, 56, 55, 54, 53, 52, 51, 50,           \
         49, 48, 47, 46, 45, 44, 43, 42, 41, 40,           \
         39, 38, 37, 36, 35, 34, 33, 32, 31, 30,           \
         29, 28, 27, 26, 25, 24, 23, 22, 21, 20,           \
         19, 18, 17, 16, 15, 14, 13, 12, 11, 10,           \
          9,  8,  7,  6,  5,  4,  3,  2,  1,  0

#define PP_NARG_(...)    PP_ARG_N(__VA_ARGS__)
#define PP_NARG(...)     PP_NARG_(__VA_ARGS__, PP_RSEQ_N())

#define HAS_COMMA(...)                                    \
        PP_ARG_N(__VA_ARGS__, \
          1,  1,  1,  1,                                   \
          1,  1,  1,  1,  1,  1,  1,  1,  1,  1,           \
          1,  1,  1,  1,  1,  1,  1,  1,  1,  1,           \
          1,  1,  1,  1,  1,  1,  1,  1,  1,  1,           \
          1,  1,  1,  1,  1,  1,  1,  1,  1,  1,           \
          1,  1,  1,  1,  1,  1,  1,  1,  1,  1,           \
          1,  1,  1,  1,  1,  1,  1,  1,  0,  0)

#define DEC(x) BOOST_DI_PRIMITIVE_CAT(DEC_, x)
#define DEC_0 0
#define DEC_1 0
#define DEC_2 1
#define DEC_3 2
#define DEC_4 3
#define DEC_5 4
#define DEC_6 5
#define DEC_7 6
#define DEC_8 7
#define DEC_9 8
#define DEC_10 9

#define ARG0(p1, ...) p1
#define ARG1(p1, p2, ...) p2
#define ARG2(p1, p2, p3, ...) p3
#define ARG3(p1, p2, p3, p4, ...) p4
#define ARG4(p1, p2, p3, p4, p5, ...) p5
#define ARG5(p1, p2, p3, p4, p5, p6, ...) p6
#define ARG6(p1, p2, p3, p4, p5, p6, p7, ...) p7
#define ARG7(p1, p2, p3, p4, p5, p6, p7, p8, ...) p8
#define ARG8(p1, p2, p3, p4, p5, p6, p7, p8, p9, ...) p9
#define ARG9(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, ...) p10

#define BOOST_DI_COMMA() ,
#define BOOST_DI_PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__
#define BOOST_DI_CAT(a, ...) BOOST_DI_PRIMITIVE_CAT(a, __VA_ARGS__)
#define BOOST_DI_CALL(m, ...) m(__VA_ARGS__)
#define BOOST_DI_IBP_SPLIT(i, ...) BOOST_DI_PRIMITIVE_CAT(BOOST_DI_IBP_SPLIT_, i)(__VA_ARGS__)
#define BOOST_DI_IBP_SPLIT_0(a, ...) a
#define BOOST_DI_IBP_SPLIT_1(a, ...) __VA_ARGS__
#define BOOST_DI_IBP_IS_VARIADIC_C(...) 1
#define BOOST_DI_IBP_IS_VARIADIC_R_1 1,
#define BOOST_DI_IBP_IS_VARIADIC_R_BOOST_DI_IBP_IS_VARIADIC_C 0,
#define BOOST_DI_IBP(...) BOOST_DI_IBP_SPLIT(0, BOOST_DI_CAT(BOOST_DI_IBP_IS_VARIADIC_R_, BOOST_DI_IBP_IS_VARIADIC_C __VA_ARGS__))
#define EMPTY()
#define _TRIGGER_PARENTHESIS_(...) ,
#define ISEMPTY(...) _ISEMPTY(HAS_COMMA(__VA_ARGS__), HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__), HAS_COMMA(__VA_ARGS__ ()), HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__ ()))
#define DEFER(id) id EMPTY()
#define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
#define EVAL(...)  EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL1(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL2(...) __VA_ARGS__
#define CHECK_N(x, n, ...) n
#define CHECK(...) CHECK_N(__VA_ARGS__, 0,)
#define NOT(x) CHECK(BOOST_DI_PRIMITIVE_CAT(NOT_, x))
#define NOT_0 ~, 1,
#define COMPL(b) BOOST_DI_PRIMITIVE_CAT(COMPL_, b)
#define COMPL_0 1
#define COMPL_1 0
#define BOOL(x) COMPL(NOT(x))
#define IIF(c) BOOST_DI_PRIMITIVE_CAT(IIF_, c)
#define IIF_0(t, ...) __VA_ARGS__
#define IIF_1(t, ...) t
#define BOOST_DI_IF(c) IIF(BOOL(c))
#define PASTE5(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4
#define _ISEMPTY(_0, _1, _2, _3) HAS_COMMA(PASTE5(_IS_EMPTY_CASE_, _0, _1, _2, _3))
#define _IS_EMPTY_CASE_0001 ,
#define EAT(...)
#define EXPAND(...) __VA_ARGS__
#define WHEN(c) BOOST_DI_IF(c)(EXPAND, EAT)
#define REPEAT(count, macro, ...) WHEN(count)(OBSTRUCT(REPEAT_INDIRECT)()(DEC(count), macro, __VA_ARGS__) OBSTRUCT(macro) (DEC(count), __VA_ARGS__ ))
#define REPEAT_INDIRECT() REPEAT
#define BOOST_DI_VARARG_IMPL2(m, count, ...) m##count(__VA_ARGS__)
#define BOOST_DI_VARARG_IMPL(m, count, ...) BOOST_DI_VARARG_IMPL2(m, count, __VA_ARGS__)

#endif

