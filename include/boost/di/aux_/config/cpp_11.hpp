//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_CONFIG_CPP_11_HPP
#define BOOST_DI_AUX_CONFIG_CPP_11_HPP

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/mpl/aux_/na.hpp>

#define BOOST_DI_CTOR_LIMIT_FROM(begin)             \
    (begin, BOOST_DI_CFG_CTOR_LIMIT_SIZE)

#define BOOST_DI_CTOR_LIMIT_FROM_FORCE(begin)       \
    (begin, begin)

#define BOOST_DI_TYPES(T)                           \
    typename... TArgs

#define BOOST_DI_TYPES_PASS(T)                      \
    TArgs...

#define BOOST_DI_MPL_VECTOR_TYPES_PASS(T)           \
    mpl::vector<TArgs...>

#define BOOST_DI_ARGS(T, arg)                       \
    const TArgs&... arg

#define BOOST_DI_ARGS_PASS(arg)                     \
    arg...

#define BOOST_DI_ARGS_CALL(arg, f)                  \
    f(arg)...

#define BOOST_DI_FORWARD_ARGS(T, arg)               \
    TArgs&&... arg

#define BOOST_DI_ARGS_FORWARD(arg)                  \
    std::forward<TArgs>(arg)...

#define BOOST_DI_TYPES_MPL_LIMIT_FROM(begin)        \
    (begin, begin)

#define BOOST_DI_TYPES_MPL_LIMIT_FROM_FORCE(begin)  \
    (begin, BOOST_MPL_LIMIT_VECTOR_SIZE)

#define BOOST_DI_TYPES_DEFAULT_MPL(T)               \
    typename... TArgs_

#define BOOST_DI_TYPES_MPL(T)                       \
    typename... TArgs_

#define BOOST_DI_TYPES_PASS_MPL(T)                  \
    TArgs_...

#define BOOST_DI_MPL_VECTOR_TYPES_PASS_MPL(T)       \
    mpl::vector<TArgs_...>

#endif

