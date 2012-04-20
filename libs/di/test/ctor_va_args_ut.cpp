//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>

#define BOOST_DI_CTOR_CFG_VA_ARGS
#include "boost/di/ctor.hpp"

namespace boost {
namespace di {
namespace test {

BOOST_AUTO_TEST_CASE(ctor_va_args_empty)
{
    struct c
    {
        BOOST_DI_CTOR_TRAITS(void);
        c() { }
    };
}

BOOST_AUTO_TEST_CASE(ctor_va_args_explicit_with_default)
{
    struct c
    {
        enum { DEFAULT = 42 };

        BOOST_DI_CTOR(explicit c, int i = DEFAULT)
            : i(i)
        { }

        int i;
    };

    c c_;

    BOOST_CHECK_EQUAL(static_cast<int>(c::DEFAULT), c_.i);
}

BOOST_AUTO_TEST_CASE(ctor_va_args_params)
{
    struct c
    {
        BOOST_DI_CTOR(c, int i, double d)
            : i(i), d(d)
        { }

        int i;
        double d;
    };

    const int i = 1;
    const double d = 2.0;

    c c_(i, d);

    BOOST_CHECK_EQUAL(i, c_.i);
    BOOST_CHECK_EQUAL(d, c_.d);
}

BOOST_AUTO_TEST_CASE(ctor_va_args_traits)
{
    const int i = 1;
    const double d = 2.0;

    struct c
    {
        BOOST_DI_CTOR_TRAITS(int i, double d);

        c(int i, double d)
            : i(i), d(d)
        { }

        int i;
        double d;
    };

    c c_(i, d);

    BOOST_CHECK_EQUAL(i, c_.i);
    BOOST_CHECK_EQUAL(d, c_.d);
}

} // namespace test
} // namespace di
} // namespace boost

