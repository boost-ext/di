//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_CONFIG_CPP_11_HPP
#define BOOST_DI_AUX_CONFIG_CPP_11_HPP

#include <boost/preprocessor/iteration/iterate.hpp>

#define BOOST_DI_CTOR_LIMIT_FROM(begin)             \
    (begin, BOOST_DI_CFG_CTOR_LIMIT_SIZE)

#define BOOST_DI_CTOR_LIMIT_FROM_FORCE(begin)       \
    (begin, begin)

#define BOOST_DI_TYPES(T)                           \
    typename... T

#define BOOST_DI_TYPES_PASS(T)                      \
    T...

#define BOOST_DI_MPL_VECTOR_TYPES_PASS(T)           \
    ::boost::di::aux::mpl::vector<T...>

#define BOOST_DI_ARGS(T, arg)                       \
    const T&... arg

#define BOOST_DI_ARGS_PASS(arg)                     \
    arg...

#define BOOST_DI_ARGS_CALL(arg, f)                  \
    f(arg)...

#define BOOST_DI_FORWARD_ARGS(T, arg)               \
    T&&... arg

#define BOOST_DI_ARGS_FORWARD(arg)                  \
    ::std::forward<decltype(arg)>(arg)...

#define BOOST_DI_TYPES_MPL_LIMIT_FROM(begin)        \
    (begin, begin)

#define BOOST_DI_TYPES_MPL_LIMIT_FROM_FORCE(begin)  \
    (begin, BOOST_MPL_LIMIT_VECTOR_SIZE)

#define BOOST_DI_TYPES_DEFAULT_MPL(T)               \
    typename... T##_MPL

#define BOOST_DI_TYPES_MPL(T)                       \
    typename... T##_MPL

#define BOOST_DI_TYPES_PASS_MPL(T)                  \
    T##_MPL...

#define BOOST_DI_MPL_VECTOR_TYPES_PASS_MPL(T)       \
    ::boost::di::aux::mpl::vector<T##_MPL...>

#endif

