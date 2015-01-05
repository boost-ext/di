//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_PREPROCESSOR_HPP
#define BOOST_DI_AUX_PREPROCESSOR_HPP

#define BOOST_DI_CAT(a, ...) BOOST_DI_PRIMITIVE_CAT(a, __VA_ARGS__)
#define BOOST_DI_CALL(m, ...) m(__VA_ARGS__)
#define BOOST_DI_EMPTY()
#define BOOST_DI_COMMA() ,
#define BOOST_DI_EAT(...)
#define BOOST_DI_EXPAND(...) __VA_ARGS__
#define BOOST_DI_NARG_(...) BOOST_DI_ARG_N(__VA_ARGS__)
#define BOOST_DI_NARG(...) BOOST_DI_NARG_(__VA_ARGS__, BOOST_DI_RSEQ_N())
#define BOOST_DI_PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__
#define BOOST_DI_IBP_SPLIT(i, ...) BOOST_DI_PRIMITIVE_CAT(BOOST_DI_IBP_SPLIT_, i)(__VA_ARGS__)
#define BOOST_DI_IBP_SPLIT_0(a, ...) a
#define BOOST_DI_IBP_SPLIT_1(a, ...) __VA_ARGS__
#define BOOST_DI_IBP_IS_VARIADIC_C(...) 1
#define BOOST_DI_IBP_IS_VARIADIC_R_1 1,
#define BOOST_DI_IBP_IS_VARIADIC_R_BOOST_DI_IBP_IS_VARIADIC_C 0,
#define BOOST_DI_IBP(...) \
    BOOST_DI_IBP_SPLIT(0, BOOST_DI_CAT( \
        BOOST_DI_IBP_IS_VARIADIC_R_, BOOST_DI_IBP_IS_VARIADIC_C __VA_ARGS__) \
    )
#define BOOST_DI_TRIGGER_PARENTHESIS(...) ,
#define BOOST_DI_IS_EMPTY(...)  \
    BOOST_DI_IS_EMPTY_IMPL(BOOST_DI_HAS_COMMA(__VA_ARGS__) \
           , BOOST_DI_HAS_COMMA(BOOST_DI_TRIGGER_PARENTHESIS __VA_ARGS__) \
           , BOOST_DI_HAS_COMMA(__VA_ARGS__ ()) \
           , BOOST_DI_HAS_COMMA(BOOST_DI_TRIGGER_PARENTHESIS __VA_ARGS__ ()))                     
#define BOOST_DI_IS_EMPTY_IMPL(_0, _1, _2, _3) \
    BOOST_DI_HAS_COMMA(BOOST_DI_JOIN_5(BOOST_DI_IS_EMPTY_IMPL_, _0, _1, _2, _3))
#define BOOST_DI_DEFER(id) id BOOST_DI_EMPTY()
#define BOOST_DI_OBSTRUCT(...) __VA_ARGS__ BOOST_DI_DEFER(BOOST_DI_EMPTY)()
#define BOOST_DI_EVAL(...) BOOST_DI_EVAL1(BOOST_DI_EVAL1(BOOST_DI_EVAL1(__VA_ARGS__)))
#define BOOST_DI_EVAL1(...) BOOST_DI_EVAL2(BOOST_DI_EVAL2(BOOST_DI_EVAL2(__VA_ARGS__)))
#define BOOST_DI_EVAL2(...) __VA_ARGS__
#define BOOST_DI_CHECK_N(x, n, ...) n
#define BOOST_DI_CHECK(...) BOOST_DI_CHECK_N(__VA_ARGS__, 0,)
#define BOOST_DI_NOT(x) BOOST_DI_CHECK(BOOST_DI_PRIMITIVE_CAT(BOOST_DI_NOT_, x))
#define BOOST_DI_NOT_0 ~, 1,
#define BOOST_DI_COMPL(b) BOOST_DI_PRIMITIVE_CAT(BOOST_DI_COMPL_, b)
#define BOOST_DI_COMPL_0 1
#define BOOST_DI_COMPL_1 0
#define BOOST_DI_BOOL(x) BOOST_DI_COMPL(BOOST_DI_NOT(x))
#define BOOST_DI_IF_IMPL(c) BOOST_DI_PRIMITIVE_CAT(BOOST_DI_IF_IMPL_, c)
#define BOOST_DI_IF_IMPL_0(t, ...) __VA_ARGS__
#define BOOST_DI_IF_IMPL_1(t, ...) t
#define BOOST_DI_IF(c) BOOST_DI_IF_IMPL(BOOST_DI_BOOL(c))
#define BOOST_DI_JOIN_5(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4
#define BOOST_DI_IS_EMPTY_IMPL_0001 ,
#define BOOST_DI_REPEAT(n, m, ...) \
    BOOST_DI_IF(n)(BOOST_DI_EXPAND, BOOST_DI_EAT)( \
        BOOST_DI_OBSTRUCT(BOOST_DI_REPEAT_INDIRECT)()( \
            BOOST_DI_DEC(n), m, __VA_ARGS__ \
        ) \
    BOOST_DI_OBSTRUCT(m)( \
        BOOST_DI_DEC(n), __VA_ARGS__ ) \
    )
#define BOOST_DI_REPEAT_INDIRECT() BOOST_DI_REPEAT
#define BOOST_DI_VARARG_IMPL2(m, n, ...) m##n(__VA_ARGS__)
#define BOOST_DI_VARARG_IMPL(m, n, ...) BOOST_DI_VARARG_IMPL2(m, n, __VA_ARGS__)
#define BOOST_DI_ARG_N( _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, N, ...) N
#define BOOST_DI_RSEQ_N() 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define BOOST_DI_HAS_COMMA(...) BOOST_DI_ARG_N(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0)
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
#define BOOST_DI_DEC(x) BOOST_DI_PRIMITIVE_CAT(BOOST_DI_DEC_, x)
#define BOOST_DI_DEC_0 0
#define BOOST_DI_DEC_1 0
#define BOOST_DI_DEC_2 1
#define BOOST_DI_DEC_3 2
#define BOOST_DI_DEC_4 3
#define BOOST_DI_DEC_5 4
#define BOOST_DI_DEC_6 5
#define BOOST_DI_DEC_7 6
#define BOOST_DI_DEC_8 7
#define BOOST_DI_DEC_9 8
#define BOOST_DI_DEC_10 9

#endif

