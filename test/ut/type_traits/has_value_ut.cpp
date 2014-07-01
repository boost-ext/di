//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/has_value.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {
namespace type_traits {

struct value_int { int value; };
struct value_double { double value; };
struct value_int_static { static double value; };
struct value_derived : value_int { };

BOOST_AUTO_TEST_CASE(values) {
    BOOST_CHECK(!has_value<int>::value);
    BOOST_CHECK(!has_value<void>::value);

    BOOST_CHECK(has_value<value_int>::value);
    BOOST_CHECK(has_value<value_double>::value);
    BOOST_CHECK(has_value<value_int_static>::value);
    BOOST_CHECK(has_value<value_derived>::value);
}

} // namespace type_traits
} // namespace di
} // namespace boost

