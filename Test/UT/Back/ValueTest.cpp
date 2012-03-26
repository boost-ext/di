//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "QDeps/Back/Value.hpp"

namespace QDeps
{
namespace Back
{
namespace Detail
{
namespace UT
{

using namespace boost;
using namespace boost::mpl;

TEST(Value, Generic)
{
    EXPECT_FALSE((Value<int>::value));
}

TEST(Value, mpl_int)
{
    const int i = 42;
    EXPECT_TRUE((Value<boost::mpl::int_<0> >::value));
    EXPECT_EQ(i, *Value<boost::mpl::int_<i> >::create<int>());
}

TEST(Value, mpl_string)
{
    const std::string s = "s";
    EXPECT_TRUE((Value<boost::mpl::string<'s'> >::value));
    EXPECT_EQ(s, *Value<boost::mpl::string<'s'> >::create<std::string>());
}

} // namespace UT
} // namespace Detail
} // namespace Back
} // namespace QDeps

