//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <boost/mpl/vector.hpp>
#include "QDeps/Back/Aux/Utility.hpp"
#include "Test/Common/Data.hpp"

namespace QDeps
{
namespace Back
{
namespace Aux
{
namespace UT
{

using namespace Test::Common;
using namespace boost::mpl;

TEST(Utility, IsUnique)
{
    EXPECT_TRUE((IsUnique< vector<> >::value));
    EXPECT_TRUE((IsUnique< vector<C1, C2, C3> >::value));
    EXPECT_FALSE((IsUnique< vector<C1, C1> >::value));
    EXPECT_FALSE((IsUnique< vector<C1, C2, C1> >::value));
    EXPECT_FALSE((IsUnique< vector<C1, C2, C2> >::value));
}

} // namespace UT
} // namespace Aux
} // namespace Back
} // namespace QDeps

