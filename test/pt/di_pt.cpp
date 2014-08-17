// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di.hpp"

#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/cat.hpp>

namespace di = boost::di;

template<int>
struct level0
{ };

#define BOOST_DI_LEVEL_GEN(_, n, level)                 \
    BOOST_PP_COMMA_IF(n) level<n>

#define BOOST_PP_LOCAL_MACRO(n)                         \
    template<int>                                       \
    struct level##n                                     \
    {                                                   \
        BOOST_DI_INJECT(level##n,                       \
            BOOST_PP_REPEAT(                            \
                BOOST_DI_CFG_TEST_LEVELS_LIMIT_SIZE     \
              , BOOST_DI_LEVEL_GEN                      \
              , BOOST_PP_CAT(level, BOOST_PP_SUB(n, 1)) \
            )                                           \
        ) { }                                           \
    };

#define BOOST_PP_LOCAL_LIMITS (1, BOOST_DI_CFG_TEST_LEVELS_LIMIT_SIZE)
#include BOOST_PP_LOCAL_ITERATE()

#undef BOOST_DI_LEVEL_GEN

int main() {
    di::make_injector().create<
        BOOST_PP_CAT(level, BOOST_DI_CFG_TEST_LEVELS_LIMIT_SIZE)<0>
    >();

    return 0;
}

