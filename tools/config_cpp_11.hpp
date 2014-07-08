//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TOOLS_CONFIG_CPP_11_HPP
#define BOOST_DI_TOOLS_CONFIG_CPP_11_HPP

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/cat.hpp>

#define BOOST_DI_CTOR_LIMIT_FROM_(begin)    \
    (begin, begin)

#define BOOST_DI_TYPES_MPL_LIMIT_FROM_(begin)         \
    (begin, begin)

#define BOOST_DI_TYPES_(T)                       \
    typename... TArgs

#define BOOST_DI_TYPES_PASS_(T)                  \
    TArgs...

#define BOOST_DI_MPL_VECTOR_TYPES_PASS_(T)       \
    mpl::vector<TArgs...>

#define BOOST_DI_ARGS_(T, arg)                   \
    TArgs&&... arg

#define BOOST_DI_ARGS__(T, arg)                   \
    const TArgs&... arg

#define BOOST_DI_ARGS_PASS__(arg)                 \
    arg...

#define BOOST_DI_ARGS_PASS_(arg)                 \
    std::forward<TArgs>(arg)...

#define BOOST_DI_ARGS_NOT_USED_(T)               \
    TArgs&&...

#define BOOST_DI_ARGS_COPY_(T, arg)              \
    TArgs&&... arg

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
      , T                                 \
      , && arg                                   \
    )

#define BOOST_DI_ARGS_NOT_USED(T)               \
    BOOST_PP_ENUM_BINARY_PARAMS(                \
        BOOST_PP_ITERATION()                    \
      , T                                 \
      , && BOOST_PP_INTERCEPT                    \
    )

#define BOOST_DI_ARGS_COPY(T, arg)              \
    BOOST_PP_ENUM_BINARY_PARAMS(                \
        BOOST_PP_ITERATION()                    \
      , T                                       \
      , && arg                                     \
    )

#define PASSS(_, n, arg) \
    BOOST_PP_COMMA_IF(n) std::forward<decltype(arg##n)>(arg##n)


#define BOOST_DI_ARGS_PASS(arg)                 \
    BOOST_PP_REPEAT(BOOST_PP_ITERATION(), PASSS, arg)

#define BOOST_DI_TYPES_MPL_LIMIT_FROM__(begin)    \
    (begin, BOOST_MPL_LIMIT_VECTOR_SIZE)
    
#define BOOST_DI_TYPES_MPL_LIMIT_FROM(begin)    \
    (begin, begin)

#define BOOST_DI_TYPES_DEFAULT_MPL(T)           \
    typename... TArgs

#define BOOST_DI_TYPES_MPL(T)                   \
    typename... TArgs

#define BOOST_DI_TYPES_PASS_MPL(T)              \
    TArgs...

#define BOOST_DI_MPL_VECTOR_TYPES_PASS_MPL(T)   \
    mpl::vector<TArgs...>

#if !defined(BOOST_DI_ASSERT_MSG)
    #define BOOST_DI_ASSERT_MSG(c, msg, type)   \
        BOOST_MPL_ASSERT_MSG(c, msg, (type))
#endif

#endif

