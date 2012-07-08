//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
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
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/aux_/na_fwd.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/config.hpp>

#if !defined(BOOST_DI_CTOR_CFG_VA_ARGS) && !defined(BOOST_DI_CTOR_CFG_BRACKET)
    #define BOOST_DI_CTOR_CFG_VA_ARGS
#endif

#if !defined(BOOST_DI_CTOR_UNIQUE_NAME)
    #define BOOST_DI_CTOR_UNIQUE_NAME boost_di_ctor
#endif

#if defined(BOOST_NO_VARIADIC_TEMPLATES) ||                 \
    defined(BOOST_NO_RVALUE_REFERENCES)  ||                 \
    defined(BOOST_DI_NO_CXX11_FEATURES)

    #if !defined(BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE)
        #define BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE 10
    #endif

    #define BOOST_DI_ITERATION_PARAMS(start, limit, file)   \
         3, (start, limit, file)

//-----------------------------------------------------------

    #define BOOST_DI_TYPES_DEFAULT_IMPL(limit, TArg, TNa)   \
         BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(               \
             limit                                          \
           , typename TArg                                  \
           , TNa                                            \
         )

    #define BOOST_DI_TYPES_IMPL(limit, TArg)                \
         BOOST_PP_ENUM_PARAMS(                              \
             limit                                          \
           , typename TArg                                  \
         )

    #define BOOST_DI_TYPES_PASS_IMPL(limit, TArg)           \
         BOOST_PP_ENUM_PARAMS(                              \
             limit                                          \
           , TArg                                           \
         )

    #define BOOST_DI_ARGS_IMPL(limit, TArg, arg)            \
         BOOST_PP_ENUM_BINARY_PARAMS(                       \
             limit                                          \
           , const TArg                                     \
           , &arg                                           \
         )

    #define BOOST_DI_ARGS_NA_IMPL(limit, count, TNa)        \
         BOOST_PP_ENUM_PARAMS(                              \
             BOOST_PP_SUB(                                  \
                 limit                                      \
               , count                                      \
             )                                              \
           , TNa BOOST_PP_INTERCEPT                         \
         )

    #define BOOST_DI_ARGS_NOT_USED_IMPL(limit, TArg)        \
         BOOST_PP_ENUM_BINARY_PARAMS(                       \
             limit                                          \
           , const TArg                                     \
           , & BOOST_PP_INTERCEPT                           \
         )

    #define BOOST_DI_ARGS_PASS_IMPL(limit, arg)             \
         BOOST_PP_ENUM_PARAMS(                              \
             limit                                          \
           , arg                                            \
         )

    #define BOOST_DI_ARGS_FORWARD_IMPL(limit, arg)          \
         BOOST_PP_ENUM_PARAMS(                              \
             limit                                          \
           , arg                                            \
         )

//-----------------------------------------------------------

    #define BOOST_DI_TYPES_DEFAULT_MPL(TArg)                \
         BOOST_DI_TYPES_DEFAULT_IMPL(                       \
             BOOST_MPL_LIMIT_VECTOR_SIZE                    \
           , TArg                                           \
           , ::boost::mpl::na                               \
         )

    #define BOOST_DI_TYPES_MPL(TArg)                        \
         BOOST_DI_TYPES_IMPL(                               \
             BOOST_MPL_LIMIT_VECTOR_SIZE                    \
           , TArg                                           \
         )

    #define BOOST_DI_TYPES_MPL_NA(count)                    \
         BOOST_DI_ARGS_NA_IMPL(                             \
             BOOST_MPL_LIMIT_VECTOR_SIZE                    \
           , count                                          \
           , ::boost::mpl::na                               \
         )

    #define BOOST_DI_TYPES_PASS_MPL(TArg)                   \
         BOOST_DI_TYPES_PASS_IMPL(                          \
             BOOST_MPL_LIMIT_VECTOR_SIZE                    \
           , TArg                                           \
         )

//-----------------------------------------------------------

    #define BOOST_DI_TYPES(TArg)                            \
         BOOST_DI_TYPES_IMPL(                               \
             BOOST_PP_ITERATION()                           \
           , TArg                                           \
         )

    #define BOOST_DI_TYPES_PASS(TArg)                       \
         BOOST_DI_TYPES_PASS_IMPL(                          \
             BOOST_PP_ITERATION()                           \
           , TArg                                           \
         )

    #define BOOST_DI_ARGS(TArg, arg)                        \
         BOOST_DI_ARGS_IMPL(                                \
             BOOST_PP_ITERATION()                           \
           , TArg                                           \
           , arg                                            \
         )

    #define BOOST_DI_ARGS_NOT_USED(TArg)                    \
         BOOST_DI_ARGS_NOT_USED_IMPL(                       \
             BOOST_PP_ITERATION()                           \
           , TArg                                           \
         )

    #define BOOST_DI_ARGS_PASS(arg)                         \
         BOOST_DI_ARGS_PASS_IMPL(                           \
             BOOST_PP_ITERATION()                           \
           , arg                                            \
         )

    #define BOOST_DI_ARGS_FORWARD(arg)                      \
         BOOST_DI_ARGS_FORWARD_IMPL(                        \
             BOOST_PP_ITERATION()                           \
           , arg                                            \
         )

//-----------------------------------------------------------

#else

    #undef BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE
    #define BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE 0 // infinity

    #define BOOST_DI_ITERATION_PARAMS(start, limit, file)   \
         3, (0, 0, file)

//-----------------------------------------------------------

    #define BOOST_DI_TYPES_DEFAULT_IMPL(limit, TArgs, TNa)  \
         typename... TArgs

    #define BOOST_DI_TYPES_IMPL(limit, TArgs)               \
         typename... TArgs

    #define BOOST_DI_TYPES_PASS_IMPL(limit, args)           \
         args...

    #define BOOST_DI_ARGS_IMPL(limit, TArgs, args)          \
         TArgs&&... args

    #define BOOST_DI_ARGS_NA_IMPL(limit, count, TNa)        \
         // empty

    #define BOOST_DI_ARGS_NOT_USED_IMPL(limit, TArgs)       \
         TArgs&&...

    #define BOOST_DI_ARGS_PASS_IMPL(limit, args)            \
         args

    #define BOOST_DI_ARGS_FORWARD_IMPL(limit, args)         \
         std::forward(args)...

//-----------------------------------------------------------

    #define BOOST_DI_TYPES_DEFAULT_MPL(TArgs)               \
         BOOST_DI_TYPES_DEFAULT_IMPL(~, TArgs, ~)

    #define BOOST_DI_TYPES_MPL(TArgs)                       \
         BOOST_DI_TYPES_IMPL(~, TArgs)

    #define BOOST_DI_TYPES_MPL_NA(count)                    \
         //empty

    #define BOOST_DI_TYPES_PASS_MPL(TArgs)                  \
         BOOST_DI_TYPES_PASS_IMPL(~, TArgs)

//-----------------------------------------------------------

    #define BOOST_DI_TYPES(TArgs)                           \
         BOOST_DI_TYPES_IMPL(~, TArgs)

    #define BOOST_DI_TYPES_PASS(args)                       \
         BOOST_DI_TYPES_PASS_IMPL(~, args)

    #define BOOST_DI_ARGS(TArgs, args)                      \
         BOOST_DI_ARGS_IMPL(~, TArgs, args)

    #define BOOST_DI_ARGS_NOT_USED(TArgs)                   \
         BOOST_DI_ARGS_NOT_USED_IMPL(~, TArgs)

    #define BOOST_DI_ARGS_PASS(args)                        \
         BOOST_DI_ARGS_PASS_IMPL(~, args)

    #define BOOST_DI_ARGS_FORWARD(args)                     \
         BOOST_DI_ARGS_FORWARD_IMPL(~, args)

//-----------------------------------------------------------

#endif

#if BOOST_WORKAROUND(BOOST_MSVC, < 1700)
    #define BOOST_DI_TEMPLATE_QUALIFIER
#else
    #define BOOST_DI_TEMPLATE_QUALIFIER template
#endif

#endif

