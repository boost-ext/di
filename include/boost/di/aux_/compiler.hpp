//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_COMPILER_HPP
#define BOOST_DI_AUX_COMPILER_HPP

#if defined(__clang__)  // clang, clang-cl
#define __CLANG__
#define BOOST_DI_UNUSED __attribute__((unused))
#define BOOST_DI_DEPRECATED(...) [[deprecated(__VA_ARGS__)]]
#define BOOST_DI_TYPE_WKND(T)
#elif defined(__GNUC__)  // gcc
#define __GCC__
#define BOOST_DI_UNUSED __attribute__((unused))
#define BOOST_DI_DEPRECATED(...) [[deprecated(__VA_ARGS__)]]
#define BOOST_DI_TYPE_WKND(T)
#elif defined(_MSC_VER)  // msvc
#define __MSVC__
#define __has_include(...) 0
#define BOOST_DI_UNUSED
#define BOOST_DI_DEPRECATED(...) \
  __declspec(deprecated(__VA_ARGS__))  // error C2059: syntax error: '<L_ATTRIBUTE_SPECIFIER>'
#define BOOST_DI_TYPE_WKND(T) (T && )
#endif

#endif
