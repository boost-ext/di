//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include "QDeps/Back/Aux/Utility.hpp"

namespace QDeps
{
namespace Back
{
namespace Aux
{
namespace UT
{

struct C0 { };

struct C1
{
    void operator()() { }
};

struct C2
{
    template<typename T, typename T2, typename T3> void operator()() const { }
};

TEST(Utility, HasCallOperator)
{
    EXPECT_FALSE(Detail::HasCallOperator<C0>::value);
    EXPECT_TRUE(Detail::HasCallOperator<C1>::value);
    EXPECT_TRUE(Detail::HasCallOperator<C2>::value);

}

} // namespace UT
} // namespace Aux
} // namespace Back
} // namespace QDeps

