//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>

#define QDEPS_CTOR_CFG_BRACKET
#include "QDeps/Front/Ctor.hpp"

namespace QDeps
{
namespace Utility
{
namespace Scopes
{
namespace UT
{

BOOST_AUTO_TEST_CASE(BracketCtorEmpty)
{
    struct C
    {
        QDEPS_CTOR(C, ()) { }
    };
}

BOOST_AUTO_TEST_CASE(BracketCtorExplicitWithDefault)
{
    struct C
    {
        enum { DEFAULT = 42 };

        QDEPS_CTOR(explicit C, (int i = DEFAULT))
            : i(i)
        { }

        int i;
    };

    C c;

    BOOST_CHECK_EQUAL(static_cast<int>(C::DEFAULT), c.i);
}

BOOST_AUTO_TEST_CASE(BracketCtorParams)
{
    const int i = 1;
    const double d = 2.0;

    struct C
    {
        QDEPS_CTOR(C, (int i, double d))
            : i(i), d(d)
        { }

        int i;
        double d;
    };

    C c(i, d);

    BOOST_CHECK_EQUAL(i, c.i);
    BOOST_CHECK_EQUAL(d, c.d);
}

BOOST_AUTO_TEST_CASE(BracketCtorTraits)
{
    const int i = 1;
    const double d = 2.0;

    struct C
    {
        QDEPS_CTOR_TRAITS((int i, double d));

        C(int i, double d)
            : i(i), d(d)
        { }

        int i;
        double d;
    };

    C c(i, d);

    BOOST_CHECK_EQUAL(i, c.i);
    BOOST_CHECK_EQUAL(d, c.d);
}

} // namespace UT
} // namespace Scopes
} // namespace Utility
} // namespace QDeps

