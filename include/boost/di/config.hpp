//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONFIG_HPP
#define BOOST_DI_CONFIG_HPP

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/aux_/na_fwd.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/config.hpp>

#if defined(BOOST_NO_CXX11_SMART_PTR) &&                \
    __clang_major__ >= 3 &&                             \
    __clang_minor__ >= 2 &&                             \
    __cplusplus >= 201100L
    #undef BOOST_NO_CXX11_SMART_PTR
#endif

#if defined(BOOST_MSVC)
    #define BOOST_DI_TEMPLATE_QUALIFIER
    #define BOOST_DI_TYPEOF(type) decltype(type)
#else
    #define BOOST_DI_TEMPLATE_QUALIFIER template
    #define BOOST_DI_TYPEOF(type) BOOST_TYPEOF_TPL(&type)
#endif

#if !defined(BOOST_DI_CTOR_CFG_VA_ARGS) &&              \
    !defined(BOOST_DI_CTOR_CFG_BRACKET)
    #define BOOST_DI_CTOR_CFG_VA_ARGS
#endif

#if !defined(BOOST_DI_CONSTRUCTOR)
    #define BOOST_DI_CONSTRUCTOR boost_di_constructor__
#endif

#if !defined(BOOST_DI_CREATE)
    #define BOOST_DI_CREATE boost_di_create__
#endif

#if !defined(BOOST_DI_LIMIT_SIZE)
    #define BOOST_DI_LIMIT_SIZE BOOST_MPL_LIMIT_VECTOR_SIZE
#endif

#define BOOST_DI_ITERATION_PARAMS(start, limit, file)   \
     3, (start, limit, file)

#define BOOST_DI_TYPES_DEFAULT_MPL(TArg)                \
     BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(               \
         BOOST_DI_LIMIT_SIZE                            \
       , typename TArg                                  \
       , ::boost::mpl::na                               \
     )

#define BOOST_DI_TYPES_MPL(TArg)                        \
     BOOST_PP_ENUM_PARAMS(                              \
         BOOST_DI_LIMIT_SIZE                            \
       , typename TArg                                  \
     )

#define BOOST_DI_TYPES_MPL_NA(count)                    \
     BOOST_PP_ENUM_PARAMS(                              \
         BOOST_PP_SUB(                                  \
             BOOST_DI_LIMIT_SIZE                        \
           , count                                      \
         )                                              \
       , ::boost::mpl::na BOOST_PP_INTERCEPT            \
     )

#define BOOST_DI_TYPES_PASS_MPL(TArg)                   \
     BOOST_PP_ENUM_PARAMS(                              \
         BOOST_DI_LIMIT_SIZE                            \
       , TArg                                           \
     )

#define BOOST_DI_TYPES(TArg)                            \
     BOOST_PP_ENUM_PARAMS(                              \
         BOOST_PP_ITERATION()                           \
       , typename TArg                                  \
     )

#define BOOST_DI_TYPES_PASS(TArg)                       \
     BOOST_PP_ENUM_PARAMS(                              \
         BOOST_PP_ITERATION()                           \
       , TArg                                           \
     )

#define BOOST_DI_ARGS(TArg, arg)                        \
     BOOST_PP_ENUM_BINARY_PARAMS(                       \
         BOOST_PP_ITERATION()                           \
       , const TArg                                     \
       , &arg                                           \
     )

#define BOOST_DI_ARGS_NOT_USED(TArg)                    \
     BOOST_PP_ENUM_BINARY_PARAMS(                       \
         BOOST_PP_ITERATION()                           \
       , const TArg                                     \
       , & BOOST_PP_INTERCEPT                           \
     )

#define BOOST_DI_ARGS_PASS(arg)                         \
     BOOST_PP_ENUM_PARAMS(                              \
         BOOST_PP_ITERATION()                           \
       , arg                                            \
     )

#endif

