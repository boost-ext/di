//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include "Test/Common/Data.hpp"
#include "QDeps/Back/Scopes/Singleton.hpp"

namespace QDeps
{
namespace Back
{
namespace Scopes
{
namespace UT
{

using namespace Test::Common;

TEST(Singleton, Create)
{
    Singleton l_s;

    EXPECT_TRUE(l_s.create<int>() == l_s.create<int>());
}

TEST(Singleton, CreateArgs)
{
    Singleton l_s;

    EXPECT_TRUE((l_s.create<C2, int, double, char>(0, 0.0, '0') == l_s.create<C2, int, double, char>(0, 0.0, '0')));
}

} // namespace UT
} // namespace Scopes
} // namespace Back
} // namespace QDeps

