//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>

#define QDEPS_CTOR_CFG_VA_ARGS
#include "QDeps/Front/Ctor.hpp"

namespace QDeps
{
namespace Utility
{
namespace Scopes
{
namespace UT
{

TEST(CtorVaArgs, Empty)
{
    struct C
    {
        QDEPS_CTOR_TRAITS();
        C() { }
    };
}

TEST(CtorVaArgs, ExplicitWithDefault)
{
    struct C
    {
        enum { DEFAULT = 42 };

        QDEPS_CTOR(explicit C, int i = DEFAULT)
            : i(i)
        { }

        int i;
    };

    C c;

    EXPECT_EQ(static_cast<int>(C::DEFAULT), c.i);
}

TEST(CtorVaArgs, Params)
{
    struct C
    {
        QDEPS_CTOR(C, int i, double d)
            : i(i), d(d)
        { }

        int i;
        double d;
    };

    const int i = 1;
    const double d = 2.0;

    C c(i, d);

    EXPECT_EQ(i, c.i);
    EXPECT_EQ(d, c.d);
}

TEST(CtorVaArgs, Traits)
{
    const int i = 1;
    const double d = 2.0;

    struct C
    {
        QDEPS_CTOR_TRAITS(int i, double d);

        C(int i, double d)
            : i(i), d(d)
        { }

        int i;
        double d;
    };

    C c(i, d);

    EXPECT_EQ(i, c.i);
    EXPECT_EQ(d, c.d);
}

} // namespace UT
} // namespace Scopes
} // namespace Utility
} // namespace QDeps

