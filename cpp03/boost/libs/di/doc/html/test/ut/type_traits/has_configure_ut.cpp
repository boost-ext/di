//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/has_configure.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {
namespace type_traits {

struct empty { };

struct with_configure
{
    int configure() { return 42; };
};

struct with_configure_derived
    : with_configure
{ };

struct with_configure_const
{
    int configure() const { return 42; };
};

BOOST_AUTO_TEST_CASE(configures) {
    BOOST_CHECK((!has_configure<empty>::value));

    BOOST_CHECK((has_configure<with_configure>::value));
    BOOST_CHECK((has_configure<with_configure_derived>::value));
    BOOST_CHECK((has_configure<with_configure_const>::value));
}

} // namespace type_traits
} // namespace di
} // namespace boost


