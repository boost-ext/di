//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>

#define BOOST_DI_CTOR_CFG_VA_ARGS
#include "di/front/ctor.hpp"

namespace di
{
namespace utility
{
namespace scopes
{
namespace ut
{

BOOST_AUTO_TEST_CASE(VaArgsctorEmpty)
{
    struct C
    {
        BOOST_DI_CTOR_TRAITS(void);
        C() { }
    };
}

BOOST_AUTO_TEST_CASE(VaArgsctorExplicitwithDefault)
{
    struct C
    {
        enum { DEFAULT = 42 };

        BOOST_DI_CTOR(explicit C, int i = DEFAULT)
            : i(i)
        { }

        int i;
    };

    C c;

    BOOST_CHECK_EQUAL(static_cast<int>(C::DEFAULT), c.i);
}

BOOST_AUTO_TEST_CASE(VaArgsctorParams)
{
    struct C
    {
        BOOST_DI_CTOR(C, int i, double d)
            : i(i), d(d)
        { }

        int i;
        double d;
    };

    const int i = 1;
    const double d = 2.0;

    C c(i, d);

    BOOST_CHECK_EQUAL(i, c.i);
    BOOST_CHECK_EQUAL(d, c.d);
}

BOOST_AUTO_TEST_CASE(VaArgsctor_traits)
{
    const int i = 1;
    const double d = 2.0;

    struct C
    {
        BOOST_DI_CTOR_TRAITS(int i, double d);

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

} // namespace ut
} // namespace scopes
} // namespace utility
} // namespace di

