//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TOOLS_CONFIG_HPP
#define BOOST_DI_TOOLS_CONFIG_HPP

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/cat.hpp>

#define BOOST_DI_CTOR_LIMIT_FROM(begin)         \
    (begin, BOOST_DI_CFG_CTOR_LIMIT_SIZE)

#define BOOST_DI_TYPES(T)                       \
    BOOST_PP_ENUM_PARAMS(                       \
        BOOST_PP_ITERATION()                    \
      , typename T                              \
    )

#define BOOST_DI_TYPES_PASS(T)                  \
    BOOST_PP_ENUM_PARAMS(                       \
        BOOST_PP_ITERATION()                    \
      , T                                       \
    )

#define BOOST_DI_MPL_VECTOR_TYPES_PASS(T)       \
    BOOST_PP_CAT(                               \
        mpl::vector                             \
     ,  BOOST_PP_ITERATION()                    \
    )<BOOST_PP_ENUM_PARAMS(                     \
        BOOST_PP_ITERATION()                    \
      , T                                       \
    )>

#define BOOST_DI_ARGS(T, arg)                   \
    BOOST_PP_ENUM_BINARY_PARAMS(                \
        BOOST_PP_ITERATION()                    \
      , const T                                 \
      , & arg                                   \
    )

#define BOOST_DI_ARGS_NOT_USED(T)               \
    BOOST_PP_ENUM_BINARY_PARAMS(                \
        BOOST_PP_ITERATION()                    \
      , const T                                 \
      , & BOOST_PP_INTERCEPT                    \
    )

#define BOOST_DI_ARGS_COPY(T, arg)              \
    BOOST_PP_ENUM_BINARY_PARAMS(                \
        BOOST_PP_ITERATION()                    \
      , T                                       \
      , arg                                     \
    )

#define BOOST_DI_ARGS_PASS(arg)                 \
    BOOST_PP_ENUM_PARAMS(                       \
        BOOST_PP_ITERATION()                    \
      , arg                                     \
    )

#define BOOST_DI_TYPES_MPL_LIMIT_FROM(begin)    \
    (begin, BOOST_MPL_LIMIT_VECTOR_SIZE)

#define BOOST_DI_TYPES_DEFAULT_MPL(T)           \
     BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(       \
         BOOST_MPL_LIMIT_VECTOR_SIZE            \
       , typename T                             \
       , ::boost::mpl::na                       \
     )

#define BOOST_DI_TYPES_MPL(T)                   \
     BOOST_PP_ENUM_PARAMS(                      \
         BOOST_MPL_LIMIT_VECTOR_SIZE            \
       , typename T                             \
     )

#define BOOST_DI_TYPES_PASS_MPL(T)              \
     BOOST_PP_ENUM_PARAMS(                      \
         BOOST_MPL_LIMIT_VECTOR_SIZE            \
       , T                                      \
     )

#define BOOST_DI_MPL_VECTOR_TYPES_PASS_MPL(T)   \
    mpl::vector<BOOST_PP_ENUM_PARAMS(           \
        BOOST_MPL_LIMIT_VECTOR_SIZE             \
      , T                                       \
    )>

#if !defined(BOOST_DI_ASSERT_MSG)
    #define BOOST_DI_ASSERT_MSG(c, msg, type)   \
        BOOST_MPL_ASSERT_MSG(c, msg, (type))
#endif

#endif

