//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_CONFIG_HPP
#define BOOST_DI_AUX_CONFIG_HPP

#include <boost/config.hpp>
#include <boost/mpl/limits/vector.hpp>

#if !defined(BOOST_DI_INJECTOR)
    #define BOOST_DI_INJECTOR boost_di_injector__
#endif

#if !defined(BOOST_DI_CFG_CTOR_LIMIT_SIZE)
    #define BOOST_DI_CFG_CTOR_LIMIT_SIZE 10
#endif

#if defined(BOOST_DI_CFG_NO_PREPROCESSED_HEADERS) || \
    (BOOST_DI_CFG_CTOR_LIMIT_SIZE % 10) ||           \
    (BOOST_MPL_LIMIT_VECTOR_SIZE % 10)

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
    #include <boost/preprocessor/repetition/enum_binary_params.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/preprocessor/facilities/intercept.hpp>
    #include <boost/preprocessor/arithmetic/sub.hpp>
    #include <boost/mpl/aux_/na.hpp>

    #define BOOST_DI_CTOR_LIMIT_FROM(begin)         \
        (begin, BOOST_DI_CFG_CTOR_LIMIT_SIZE)

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

#endif

#if defined(BOOST_GCC) || defined(BOOST_CLANG)
    #pragma GCC system_header
    #pragma GCC diagnostic ignored "-Wvariadic-macros"
#endif

#define BOOST_DI_FEATURE(feature) \
    BOOST_DI_FEATURE_##feature

#if defined(BOOST_NO_RVALUE_REFERENCES)
    #define BOOST_DI_FEATURE_RVALUE_REFERENCES(...)
    #define BOOST_DI_FEATURE_NO_RVALUE_REFERENCES(...) __VA_ARGS__
#else
    #define BOOST_DI_FEATURE_RVALUE_REFERENCES(...) __VA_ARGS__
    #define BOOST_DI_FEATURE_NO_RVALUE_REFERENCES(...)
#endif

#if defined(BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS)
    #define BOOST_DI_FEATURE_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS(...) __VA_ARGS__
    #define BOOST_DI_FEATURE_FUNCTION_TEMPLATE_DEFAULT_ARGS(...)
#else
    #define BOOST_DI_FEATURE_FUNCTION_TEMPLATE_DEFAULT_ARGS(...) __VA_ARGS__
    #define BOOST_DI_FEATURE_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS(...)
#endif

#if defined(BOOST_NO_CXX11_DECLTYPE)
    #include <boost/typeof/typeof.hpp>
    #define BOOST_DI_FEATURE_DECLTYPE(...) BOOST_TYPEOF_TPL(__VA_ARGS__)
#else
    #define BOOST_DI_FEATURE_DECLTYPE(...) decltype(__VA_ARGS__)
#endif

#if defined(BOOST_DI_CFG_EXAMINE_CALL_STACK)
    #define BOOST_DI_FEATURE_EXAMINE_CALL_STACK(...) __VA_ARGS__
    #define BOOST_DI_FEATURE_NO_EXAMINE_CALL_STACK(...)
#else
    #define BOOST_DI_FEATURE_EXAMINE_CALL_STACK(...)
    #define BOOST_DI_FEATURE_NO_EXAMINE_CALL_STACK(...) __VA_ARGS__
#endif

#define BOOST_DI_WKND(wknd) \
    BOOST_DI_WKND_##wknd

#if defined(BOOST_MSVC)
    #define BOOST_DI_WKND_MSVC(...) __VA_ARGS__
    #define BOOST_DI_WKND_NO_MSVC(...)
#else
    #define BOOST_DI_WKND_MSVC(...)
    #define BOOST_DI_WKND_NO_MSVC(...) __VA_ARGS__
#endif

#if !defined(BOOST_DI_ASSERT_MSG)
    #include <boost/mpl/assert.hpp>
    #define BOOST_DI_ASSERT_MSG(c, msg, type)   \
        BOOST_MPL_ASSERT_MSG(c, msg, (type))
#endif

#endif

