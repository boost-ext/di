//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/fixed.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>

namespace boost {
namespace di {
namespace scopes {

struct empty { };
struct double_value
{
    static double value;
};
double double_value::value = 0.0;

BOOST_AUTO_TEST_CASE(value_generic) {
    BOOST_CHECK(!aux::fixed_impl<int>::value);
    BOOST_CHECK(!aux::fixed_impl<empty>::value);
}

BOOST_AUTO_TEST_CASE(value_mpl_int) {
    const int i = 42;
    BOOST_CHECK((aux::fixed_impl<mpl::int_<0> >::value));
    BOOST_CHECK_EQUAL(i, aux::fixed_impl<mpl::int_<i> >::create());
}

BOOST_AUTO_TEST_CASE(value_mpl_string) {
    const std::string s = "s";
    BOOST_CHECK((aux::fixed_impl<mpl::string<'s'> >::value));
    BOOST_CHECK_EQUAL(s, aux::fixed_impl<mpl::string<'s'> >::create());
}

BOOST_AUTO_TEST_CASE(value_has_value_type) {
    const double d = 42.0;
    double_value::value = d;
    BOOST_CHECK((aux::fixed_impl<double_value>::value));
    BOOST_CHECK_EQUAL(d, aux::fixed_impl<double_value>::create());
}

} // namespace scopes
} // namespace di
} // namespace boost

