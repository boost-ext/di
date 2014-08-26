//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/make_injector.hpp"
#include "boost/di/named.hpp"

#include <boost/test/unit_test.hpp>

#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

namespace boost {
namespace di {

#define BOOST_DI_NAME_GEN(_, n, name) struct name##n { };
    BOOST_PP_REPEAT(
        BOOST_DI_CFG_CTOR_LIMIT_SIZE
      , BOOST_DI_NAME_GEN
      , name
    )
#undef BOOST_DI_NAME_GEN

struct limit
{
    limit(
        #define BOOST_DI_NAMED_ARG_GEN(_, n, name) BOOST_PP_COMMA_IF(n) named<int, name##n> arg##n
            BOOST_PP_REPEAT(
                BOOST_DI_CFG_CTOR_LIMIT_SIZE
              , BOOST_DI_NAMED_ARG_GEN
              , name
            )
        #undef BOOST_DI_NAMED_ARG_GEN
    ) :
        #define BOOST_DI_INIT_LIST_GEN(_, n, arg) BOOST_PP_COMMA_IF(n) arg##n(arg##n)
            BOOST_PP_REPEAT(
                BOOST_DI_CFG_CTOR_LIMIT_SIZE
              , BOOST_DI_INIT_LIST_GEN
              , arg
            )
        #undef BOOST_DI_INIT_LIST_GEN
    { }

    #define BOOST_DI_MEMBER_GEN(_, n, arg) int arg##n;
        BOOST_PP_REPEAT(
            BOOST_DI_CFG_CTOR_LIMIT_SIZE
          , BOOST_DI_MEMBER_GEN
          , arg
        )
    #undef BOOST_DI_MEMBER_GEN
};

BOOST_AUTO_TEST_CASE(di_limit_size) {
    limit limit_ = make_injector(
        #define BOOST_DI_BIND_NAMED_GEN(_, n, arg) BOOST_PP_COMMA_IF(n) bind<int>::named<name##n>::to(n)
            BOOST_PP_REPEAT(
                BOOST_DI_CFG_CTOR_LIMIT_SIZE
              , BOOST_DI_BIND_NAMED_GEN
              , name
            )
        #undef BOOST_DI_BIND_NAMED_GEN
    ).create<limit>();

    #define BOOST_DI_CHECK_EQUAL(_, n, arg) BOOST_CHECK_EQUAL(n, limit_.arg##n);
        BOOST_PP_REPEAT(
            BOOST_DI_CFG_CTOR_LIMIT_SIZE
          , BOOST_DI_CHECK_EQUAL
          , arg
        )
    #undef BOOST_DI_CHECK_EQUAL
}

} // namespace di
} // namespace boost

