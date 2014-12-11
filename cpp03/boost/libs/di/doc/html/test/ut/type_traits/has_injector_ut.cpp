//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/has_injector.hpp"

#include <boost/test/unit_test.hpp>

#include <boost/preprocessor/cat.hpp>

namespace boost {
namespace di {
namespace type_traits {

struct empty { };

struct injector_empty
{
    static void BOOST_DI_INJECTOR();
};

struct injector_args
{
    static void BOOST_DI_INJECTOR(int, double);
};

struct injector_derived
    : injector_empty
{ };

BOOST_AUTO_TEST_CASE(injectors) {
    BOOST_CHECK(!BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<void>::value);
    BOOST_CHECK(!BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<int>::value);
    BOOST_CHECK(!BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<empty>::value);

    BOOST_CHECK(BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<injector_empty>::value);
    BOOST_CHECK(BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<injector_args>::value);
    BOOST_CHECK(BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<injector_derived>::value);
}

} // namespace type_traits
} // namespace di
} // namespace boost

