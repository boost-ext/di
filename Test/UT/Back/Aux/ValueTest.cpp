//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "QDeps/Back/Aux/Value.hpp"

namespace QDeps
{
namespace Back
{
namespace Aux
{
namespace UT
{

using namespace boost;
using namespace boost::mpl;

BOOST_AUTO_TEST_CASE(Generic)
{
    BOOST_CHECK(!Value<int>::value);
}

BOOST_AUTO_TEST_CASE(mpl_int)
{
    const int i = 42;
    BOOST_CHECK((Value<int_<0> >::value));
    BOOST_CHECK_EQUAL(i, Value<int_<i> >::create());
}

BOOST_AUTO_TEST_CASE(mpl_string)
{
    const std::string s = "s";
    BOOST_CHECK((Value<string<'s'> >::value));
    BOOST_CHECK_EQUAL(s, Value<string<'s'> >::create());
}

} // namespace UT
} // namespace Aux
} // namespace Back
} // namespace QDeps

