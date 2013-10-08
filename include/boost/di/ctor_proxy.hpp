//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CTOR_PROXY_HPP
#define BOOST_DI_CTOR_PROXY_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/elem.hpp>

#include "boost/di/ctor.hpp"

#define BOOST_DI_CTOR_PROXY_IMPL_ARGS(_, n, args)                       \
    BOOST_PP_COMMA_IF(n)                                                \
    typename ::boost::mpl::at_c<TCtor, n>::type args##n

#define BOOST_DI_CTOR_PROXY_IMPL(_, n, seq)                             \
    template<typename T, typename TCtor>                                \
    struct BOOST_PP_SEQ_ELEM(0, seq)<                                   \
        T                                                               \
      , TCtor                                                           \
      , typename ::boost::enable_if_c<                                  \
            ::boost::mpl::size<TCtor>::value == n                       \
        >::type                                                         \
    >                                                                   \
        : BOOST_PP_SEQ_ELEM(1, seq)                                     \
    {                                                                   \
        static void BOOST_DI_CONSTRUCTOR(                               \
            BOOST_PP_REPEAT(n, BOOST_DI_CTOR_PROXY_IMPL_ARGS, args)     \
        );                                                              \
                                                                        \
        BOOST_PP_SEQ_ELEM(0, seq)(                                      \
            BOOST_PP_REPEAT(n, BOOST_DI_CTOR_PROXY_IMPL_ARGS, args)     \
        ) {                                                             \
            BOOST_PP_REPEAT(n, BOOST_PP_SEQ_ELEM(2, seq), args)         \
        }                                                               \
    };

#define BOOST_DI_CTOR_PROXY(name, type, ctor)                           \
    template<typename, typename, typename = void>                       \
    struct name;                                                        \
                                                                        \
    BOOST_PP_REPEAT_FROM_TO(                                            \
        1                                                               \
      , BOOST_MPL_LIMIT_VECTOR_SIZE                                     \
      , BOOST_DI_CTOR_PROXY_IMPL                                        \
      , (name)(type)(ctor)                                              \
   )

#endif

