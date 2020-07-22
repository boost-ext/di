//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_PREPROCESSOR_HPP
#define BOOST_DI_AUX_PREPROCESSOR_HPP

#include "boost/di/aux_/compiler.hpp"

#define __BOOST_DI_IF(cond, t, f) __BOOST_DI_IF_I(cond, t, f)
#define __BOOST_DI_REPEAT(i, m, ...) __BOOST_DI_REPEAT_N(i, m, __VA_ARGS__)
#define __BOOST_DI_CAT(a, ...) __BOOST_DI_PRIMITIVE_CAT(a, __VA_ARGS__)
#define __BOOST_DI_EMPTY()
#define __BOOST_DI_COMMA() ,
#define __BOOST_DI_EAT(...)
#define __BOOST_DI_EXPAND(...) __VA_ARGS__
#define __BOOST_DI_SIZE(...) __BOOST_DI_CAT(__BOOST_DI_VARIADIC_SIZE_I(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, ), )
#define __BOOST_DI_PRIMITIVE_CAT(a, ...) a##__VA_ARGS__
#define __BOOST_DI_ELEM(n, ...) __BOOST_DI_ELEM_I(n, __VA_ARGS__)
#define __BOOST_DI_IS_EMPTY(...)                              \
  __BOOST_DI_DETAIL_IS_EMPTY_IIF(__BOOST_DI_IBP(__VA_ARGS__)) \
  (__BOOST_DI_DETAIL_IS_EMPTY_GEN_ZERO, __BOOST_DI_DETAIL_IS_EMPTY_PROCESS)(__VA_ARGS__)

#define __BOOST_DI_DETAIL_IS_EMPTY_PRIMITIVE_CAT(a, b) a##b
#define __BOOST_DI_DETAIL_IS_EMPTY_IIF(bit) __BOOST_DI_DETAIL_IS_EMPTY_PRIMITIVE_CAT(__BOOST_DI_DETAIL_IS_EMPTY_IIF_, bit)
#define __BOOST_DI_DETAIL_IS_EMPTY_NON_FUNCTION_C(...) ()
#define __BOOST_DI_DETAIL_IS_EMPTY_GEN_ZERO(...) 0
#define __BOOST_DI_VARIADIC_SIZE_I(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, size, ...) size
#define __BOOST_DI_IF_I(cond, t, f) __BOOST_DI_IIF(cond, t, f)
#define __BOOST_DI_IIF_0(t, f) f
#define __BOOST_DI_IIF_1(t, f) t
#define __BOOST_DI_IIF_2(t, f) t
#define __BOOST_DI_IIF_3(t, f) t
#define __BOOST_DI_IIF_4(t, f) t
#define __BOOST_DI_IIF_5(t, f) t
#define __BOOST_DI_IIF_6(t, f) t
#define __BOOST_DI_IIF_7(t, f) t
#define __BOOST_DI_IIF_8(t, f) t
#define __BOOST_DI_IIF_9(t, f) t
#define __BOOST_DI_ELEM_I(n, ...) __BOOST_DI_CAT(__BOOST_DI_CAT(__BOOST_DI_ELEM, n)(__VA_ARGS__, ), )
#define __BOOST_DI_ELEM0(p1, ...) p1
#define __BOOST_DI_ELEM1(p1, p2, ...) p2
#define __BOOST_DI_ELEM2(p1, p2, p3, ...) p3
#define __BOOST_DI_ELEM3(p1, p2, p3, p4, ...) p4
#define __BOOST_DI_ELEM4(p1, p2, p3, p4, p5, ...) p5
#define __BOOST_DI_ELEM5(p1, p2, p3, p4, p5, p6, ...) p6
#define __BOOST_DI_ELEM6(p1, p2, p3, p4, p5, p6, p7, ...) p7
#define __BOOST_DI_ELEM7(p1, p2, p3, p4, p5, p6, p7, p8, ...) p8
#define __BOOST_DI_ELEM8(p1, p2, p3, p4, p5, p6, p7, p8, p9, ...) p9
#define __BOOST_DI_ELEM9(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, ...) p10
#define __BOOST_DI_REPEAT_N(i, m, ...) __BOOST_DI_REPEAT_##i(m, __VA_ARGS__)
#define __BOOST_DI_REPEAT_1(m, ...) m(0, __VA_ARGS__)
#define __BOOST_DI_REPEAT_2(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__)
#define __BOOST_DI_REPEAT_3(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__)
#define __BOOST_DI_REPEAT_4(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__)
#define __BOOST_DI_REPEAT_5(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__) m(4, __VA_ARGS__)
#define __BOOST_DI_REPEAT_6(m, ...) \
  m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__) m(4, __VA_ARGS__) m(5, __VA_ARGS__)
#define __BOOST_DI_REPEAT_7(m, ...) \
  m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__) m(4, __VA_ARGS__) m(5, __VA_ARGS__) m(6, __VA_ARGS__)
#define __BOOST_DI_REPEAT_8(m, ...)                                                                           \
  m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__) m(4, __VA_ARGS__) m(5, __VA_ARGS__) \
      m(6, __VA_ARGS__) m(7, __VA_ARGS__)
#define __BOOST_DI_REPEAT_9(m, ...)                                                                           \
  m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__) m(4, __VA_ARGS__) m(5, __VA_ARGS__) \
      m(6, __VA_ARGS__) m(7, __VA_ARGS__) m(8, __VA_ARGS__)
#define __BOOST_DI_REPEAT_10(m, ...)                                                                          \
  m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__) m(4, __VA_ARGS__) m(5, __VA_ARGS__) \
      m(6, __VA_ARGS__) m(7, __VA_ARGS__) m(8, __VA_ARGS__) m(9, __VA_ARGS__)

#if defined(__MSVC__)
#define __BOOST_DI_VD_IBP_CAT(a, b) __BOOST_DI_VD_IBP_CAT_I(a, b)
#define __BOOST_DI_VD_IBP_CAT_I(a, b) __BOOST_DI_VD_IBP_CAT_II(a##b)
#define __BOOST_DI_VD_IBP_CAT_II(res) res
#define __BOOST_DI_IBP_SPLIT(i, ...) \
  __BOOST_DI_VD_IBP_CAT(__BOOST_DI_IBP_PRIMITIVE_CAT(__BOOST_DI_IBP_SPLIT_, i)(__VA_ARGS__), __BOOST_DI_EMPTY())
#define __BOOST_DI_IBP_IS_VARIADIC_C(...) 1 1
#define __BOOST_DI_IBP_SPLIT_0(a, ...) a
#define __BOOST_DI_IBP_SPLIT_1(a, ...) __VA_ARGS__
#define __BOOST_DI_IBP_CAT(a, ...) __BOOST_DI_IBP_PRIMITIVE_CAT(a, __VA_ARGS__)
#define __BOOST_DI_IBP_PRIMITIVE_CAT(a, ...) a##__VA_ARGS__
#define __BOOST_DI_IBP_IS_VARIADIC_R_1 1,
#define __BOOST_DI_IBP_IS_VARIADIC_R___BOOST_DI_IBP_IS_VARIADIC_C 0,
#define __BOOST_DI_IBP(...) \
  __BOOST_DI_IBP_SPLIT(0, __BOOST_DI_IBP_CAT(__BOOST_DI_IBP_IS_VARIADIC_R_, __BOOST_DI_IBP_IS_VARIADIC_C __VA_ARGS__))
#define __BOOST_DI_IIF(bit, t, f) __BOOST_DI_IIF_OO((bit, t, f))
#define __BOOST_DI_IIF_OO(par) __BOOST_DI_IIF_I##par
#define __BOOST_DI_IIF_I(bit, t, f) __BOOST_DI_IIF_##bit(t, f)
#define __BOOST_DI_DETAIL_IS_EMPTY_IIF_0(t, b) b
#define __BOOST_DI_DETAIL_IS_EMPTY_IIF_1(t, b) t
#define __BOOST_DI_DETAIL_IS_EMPTY_PROCESS(...) __BOOST_DI_IBP(__BOOST_DI_DETAIL_IS_EMPTY_NON_FUNCTION_C __VA_ARGS__())
#else
#define __BOOST_DI_IBP_SPLIT(i, ...) __BOOST_DI_PRIMITIVE_CAT(__BOOST_DI_IBP_SPLIT_, i)(__VA_ARGS__)
#define __BOOST_DI_IBP_SPLIT_0(a, ...) a
#define __BOOST_DI_IBP_SPLIT_1(a, ...) __VA_ARGS__
#define __BOOST_DI_IBP_IS_VARIADIC_C(...) 1
#define __BOOST_DI_IBP_IS_VARIADIC_R_1 1,
#define __BOOST_DI_IBP_IS_VARIADIC_R___BOOST_DI_IBP_IS_VARIADIC_C 0,
#define __BOOST_DI_IBP(...) \
  __BOOST_DI_IBP_SPLIT(0, __BOOST_DI_CAT(__BOOST_DI_IBP_IS_VARIADIC_R_, __BOOST_DI_IBP_IS_VARIADIC_C __VA_ARGS__))
#define __BOOST_DI_IIF(bit, t, f) __BOOST_DI_IIF_I(bit, t, f)
#define __BOOST_DI_IIF_I(bit, t, f) __BOOST_DI_IIF_II(__BOOST_DI_IIF_##bit(t, f))
#define __BOOST_DI_IIF_II(id) id
#define __BOOST_DI_DETAIL_IS_EMPTY_IIF_0(t, ...) __VA_ARGS__
#define __BOOST_DI_DETAIL_IS_EMPTY_IIF_1(t, ...) t
#define __BOOST_DI_DETAIL_IS_EMPTY_PROCESS(...) __BOOST_DI_IBP(__BOOST_DI_DETAIL_IS_EMPTY_NON_FUNCTION_C __VA_ARGS__())
#endif

#endif
