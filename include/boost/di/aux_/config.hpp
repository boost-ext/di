//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_CONFIG_HPP
#define BOOST_DI_AUX_CONFIG_HPP

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/aux_/na.hpp>
#include <boost/mpl/assert.hpp>

// ctor

#if !defined(BOOST_DI_CTOR_LIMIT_SIZE)
    #define BOOST_DI_CTOR_LIMIT_SIZE 10
#endif

#define BOOST_DI_CTOR_LIMIT_FROM(begin)         \
    (begin, BOOST_DI_CTOR_LIMIT_SIZE)

#define BOOST_DI_TYPES(T)                       \
     BOOST_PP_ENUM_PARAMS(                      \
         BOOST_PP_ITERATION()                   \
       , typename T                             \
     )

#define BOOST_DI_TYPES_PASS(T)                  \
     BOOST_PP_ENUM_PARAMS(                      \
         BOOST_PP_ITERATION()                   \
       , T                                      \
     )

#define BOOST_DI_ARGS(T, arg)                   \
     BOOST_PP_ENUM_BINARY_PARAMS(               \
         BOOST_PP_ITERATION()                   \
       , T                                      \
       , arg                                    \
     )

#define BOOST_DI_ARGS_NOT_USED(T)               \
     BOOST_PP_ENUM_BINARY_PARAMS(               \
         BOOST_PP_ITERATION()                   \
       , T                                      \
       , BOOST_PP_INTERCEPT                     \
     )

#define BOOST_DI_ARGS_PASS(arg)                 \
     BOOST_PP_ENUM_PARAMS(                      \
         BOOST_PP_ITERATION()                   \
       , arg                                    \
     )

// mpl

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

// feature

#define BOOST_DI_FEATURE(n, feature) \
    BOOST_DI_FEATURE_##n##_##feature

#if defined(BOOST_NO_RVALUE_REFERENCES)
    #define BOOST_DI_FEATURE_1_RVALUE_REFERENCES(a1)
    #define BOOST_DI_FEATURE_2_RVALUE_REFERENCES(a1, a2)
    #define BOOST_DI_FEATURE_3_RVALUE_REFERENCES(a1, a2, a3)
    #define BOOST_DI_FEATURE_4_RVALUE_REFERENCES(a1, a2, a3, a4)
    #define BOOST_DI_FEATURE_5_RVALUE_REFERENCES(a1, a2, a3, a4, a5)
#else
    #define BOOST_DI_FEATURE_1_RVALUE_REFERENCES(a1) a1
    #define BOOST_DI_FEATURE_2_RVALUE_REFERENCES(a1, a2) a1, a2
    #define BOOST_DI_FEATURE_3_RVALUE_REFERENCES(a1, a2, a3) a1, a2, a3
    #define BOOST_DI_FEATURE_4_RVALUE_REFERENCES(a1, a2, a3, a4) a1, a2, a3, a4
    #define BOOST_DI_FEATURE_5_RVALUE_REFERENCES(a1, a2, a3, a4, a5) a1, a2, a3, a4, a5
#endif

#if defined(BOOST_NO_RVALUE_REFERENCES)
    #define BOOST_DI_FEATURE_1_NO_RVALUE_REFERENCES(a1) a1
    #define BOOST_DI_FEATURE_2_NO_RVALUE_REFERENCES(a1, a2) a1, a2
    #define BOOST_DI_FEATURE_3_NO_RVALUE_REFERENCES(a1, a2, a3) a1, a2, a3
    #define BOOST_DI_FEATURE_4_NO_RVALUE_REFERENCES(a1, a2, a3, a4) a1, a2, a3, a4
    #define BOOST_DI_FEATURE_5_NO_RVALUE_REFERENCES(a1, a2, a3, a4, a5) a1, a2, a3, a4, a5
#else
    #define BOOST_DI_FEATURE_1_NO_RVALUE_REFERENCES(a1)
    #define BOOST_DI_FEATURE_2_NO_RVALUE_REFERENCES(a1, a2)
    #define BOOST_DI_FEATURE_3_NO_RVALUE_REFERENCES(a1, a2, a3)
    #define BOOST_DI_FEATURE_4_NO_RVALUE_REFERENCES(a1, a2, a3, a4)
    #define BOOST_DI_FEATURE_5_NO_RVALUE_REFERENCES(a1, a2, a3, a4, a5)
#endif

#if defined(BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS)
    #define BOOST_DI_FEATURE_1_FUNCTION_TEMPLATE_DEFAULT_ARGS(a1)
    #define BOOST_DI_FEATURE_2_FUNCTION_TEMPLATE_DEFAULT_ARGS(a1, a2)
    #define BOOST_DI_FEATURE_3_FUNCTION_TEMPLATE_DEFAULT_ARGS(a1, a2, a3)
    #define BOOST_DI_FEATURE_4_FUNCTION_TEMPLATE_DEFAULT_ARGS(a1, a2, a3, a4)
    #define BOOST_DI_FEATURE_5_FUNCTION_TEMPLATE_DEFAULT_ARGS(a1, a2, a3, a4, a5)
#else
    #define BOOST_DI_FEATURE_1_FUNCTION_TEMPLATE_DEFAULT_ARGS(a1) a1
    #define BOOST_DI_FEATURE_2_FUNCTION_TEMPLATE_DEFAULT_ARGS(a1, a2) a1, a2
    #define BOOST_DI_FEATURE_3_FUNCTION_TEMPLATE_DEFAULT_ARGS(a1, a2, a3) a1, a2, a3
    #define BOOST_DI_FEATURE_4_FUNCTION_TEMPLATE_DEFAULT_ARGS(a1, a2, a3, a4) a1, a2, a3, a4
    #define BOOST_DI_FEATURE_5_FUNCTION_TEMPLATE_DEFAULT_ARGS(a1, a2, a3, a4, a5) a1, a2, a3, a4, a5
#endif

#if defined(BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS)
    #define BOOST_DI_FEATURE_1_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS(a1) a1
    #define BOOST_DI_FEATURE_2_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS(a1, a2) a1, a2
    #define BOOST_DI_FEATURE_3_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS(a1, a2, a3) a1, a2, a3
    #define BOOST_DI_FEATURE_4_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS(a1, a2, a3, a4) a1, a2, a3, a4
    #define BOOST_DI_FEATURE_5_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS(a1, a2, a3, a4, a5) a1, a2, a3, a4, a5
#else
    #define BOOST_DI_FEATURE_1_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS(a1)
    #define BOOST_DI_FEATURE_2_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS(a1, a2)
    #define BOOST_DI_FEATURE_3_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS(a1, a2, a3)
    #define BOOST_DI_FEATURE_4_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS(a1, a2, a3, a4)
    #define BOOST_DI_FEATURE_5_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS(a1, a2, a3, a4, a5)
#endif

#define BOOST_DI_COMMA() ,

// assert

#if !defined(BOOST_DI_ASSERT_MSG)
    #define BOOST_DI_ASSERT_MSG(c, msg, type)   \
        BOOST_MPL_ASSERT_MSG(c, msg, (type))
#endif

#endif

