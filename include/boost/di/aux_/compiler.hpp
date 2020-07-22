//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_COMPILER_HPP
#define BOOST_DI_AUX_COMPILER_HPP

#define __BOOST_DI_COMPILER(arg, ...) __BOOST_DI_COMPILER_IMPL(arg, __VA_ARGS__)
#define __BOOST_DI_COMPILER_IMPL(arg, ...) arg##__VA_ARGS__

#if defined(__clang__)  // clang, clang-cl
#define __CLANG__ __BOOST_DI_COMPILER(__clang_major__, __clang_minor__)
#define __BOOST_DI_UNUSED __attribute__((unused))
#define __BOOST_DI_DEPRECATED(...) [[deprecated(__VA_ARGS__)]]
#define __BOOST_DI_TYPE_WKND(T)
#define __BOOST_DI_ACCESS_WKND private
#define __BOOST_DI_VARIABLE_TEMPLATE_INIT_WKND \
  {}
#elif defined(__GNUC__)  // gcc
#define __GCC__
#define __BOOST_DI_UNUSED __attribute__((unused))
#define __BOOST_DI_DEPRECATED(...) [[deprecated(__VA_ARGS__)]]
#define __BOOST_DI_TYPE_WKND(T)
#define __BOOST_DI_ACCESS_WKND private
#define __BOOST_DI_VARIABLE_TEMPLATE_INIT_WKND \
  {}
#elif defined(_MSC_VER)  // msvc
#define __MSVC__
#if !defined(__has_include)
#define __has_include(...) 0
#endif
#define __BOOST_DI_UNUSED
#define __BOOST_DI_DEPRECATED(...) __declspec(deprecated(__VA_ARGS__))  // error C2059: syntax error: '<L_ATTRIBUTE_SPECIFIER>'
#define __BOOST_DI_TYPE_WKND(T) (T &&)
#define __BOOST_DI_ACCESS_WKND public
#define __BOOST_DI_VARIABLE_TEMPLATE_INIT_WKND
#endif

#if !defined(__has_builtin)
#define __has_builtin(...) 0
#endif
#if !defined(__has_extension)
#define __has_extension(...) 0
#endif

#endif
