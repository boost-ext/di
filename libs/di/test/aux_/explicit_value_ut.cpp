//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/aux_/explicit_value.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>

namespace boost {
namespace di {
namespace aux_ {

struct empty { };
struct double_value
{
    static double value;
};
double double_value::value = 0.0;

BOOST_AUTO_TEST_CASE(value_generic)
{
    BOOST_CHECK(!explicit_value<int>::value);
    BOOST_CHECK(!explicit_value<empty>::value);
}

BOOST_AUTO_TEST_CASE(value_mpl_int)
{
    const int i = 42;
    BOOST_CHECK((explicit_value<mpl::int_<0> >::value));
    BOOST_CHECK_EQUAL(i, explicit_value<mpl::int_<i> >::create());
}

BOOST_AUTO_TEST_CASE(value_mpl_string)
{
    const std::string s = "s";
    BOOST_CHECK((explicit_value<mpl::string<'s'> >::value));
    BOOST_CHECK_EQUAL(s, explicit_value<mpl::string<'s'> >::create());
}

BOOST_AUTO_TEST_CASE(value_has_value_type)
{
    const double d = 42.0;
    double_value::value = d;
    BOOST_CHECK((explicit_value<double_value>::value));
    BOOST_CHECK_EQUAL(d, explicit_value<double_value>::create());
}

} // namespace aux_
} // namespace di
} // namespace boost

