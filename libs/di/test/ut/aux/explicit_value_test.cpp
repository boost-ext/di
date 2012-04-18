//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "di/back/aux/explicit_value.hpp"

namespace di
{
namespace back
{
namespace aux
{
namespace ut
{

using namespace boost;
using namespace boost::mpl;

struct Empty { };
struct Doublevalue
{
    static double value;
};
double Doublevalue::value = 0.0;

BOOST_AUTO_TEST_CASE(Generic)
{
    BOOST_CHECK(!explicit_value<int>::value);
    BOOST_CHECK(!explicit_value<Empty>::value);
}

BOOST_AUTO_TEST_CASE(MplInt)
{
    const int i = 42;
    BOOST_CHECK((explicit_value<int_<0> >::value));
    BOOST_CHECK_EQUAL(i, explicit_value<int_<i> >::create());
}

BOOST_AUTO_TEST_CASE(MplString)
{
    const std::string s = "s";
    BOOST_CHECK((explicit_value<string<'s'> >::value));
    BOOST_CHECK_EQUAL(s, explicit_value<string<'s'> >::create());
}

BOOST_AUTO_TEST_CASE(has_value_type)
{
    const double d = 42.0;
    Doublevalue::value = d;
    BOOST_CHECK((explicit_value<Doublevalue>::value));
    BOOST_CHECK_EQUAL(d, explicit_value<Doublevalue>::create());
}

} // namespace ut
} // namespace aux
} // namespace back
} // namespace di

