//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include "Test/Common/Ctors.hpp"
#include "QDeps/Utility/Scopes/PerRequest.hpp"

namespace QDeps
{
namespace Utility
{
namespace Scopes
{
namespace UT
{

using namespace Test::Common;

TEST(PerRequest, Create)
{
    PerRequest l_pr;

    EXPECT_TRUE(l_pr.create<int>() != l_pr.create<int>());
}

TEST(PerRequest, CreateArgs)
{
    PerRequest l_pr;

    EXPECT_TRUE((l_pr.create<C2, int, double, char>(0, 0.0, '0') != l_pr.create<C2, int, double, char>(0, 0.0, '0')));
}

} // namespace UT
} // namespace Scopes
} // namespace Utility
} // namespace QDeps

