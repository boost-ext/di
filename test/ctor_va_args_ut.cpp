//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#define BOOST_DI_CTOR_CFG_VA_ARGS
#include "boost/di/ctor.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {

BOOST_AUTO_TEST_CASE(empty_ctor) {
    struct c
    {
        BOOST_DI_CTOR(c, void) { }
    };

    c c_;
}

BOOST_AUTO_TEST_CASE(empty_traits) {
    struct c
    {
        BOOST_DI_CTOR_TRAITS();
        c() { }
    };

    c c_;
}

BOOST_AUTO_TEST_CASE(explicit_with_default) {
    struct c
    {
        enum { DEFAULT = 42 };

        BOOST_DI_CTOR(explicit c, int i = DEFAULT)
            : i(i)
        { }

        int i = 0;
    };

    c c_;

    BOOST_CHECK_EQUAL(static_cast<int>(c::DEFAULT), c_.i);
}

BOOST_AUTO_TEST_CASE(params) {
    struct c
    {
        BOOST_DI_CTOR(c, int i, double d)
            : i(i), d(d)
        { }

        int i = 0;
        double d = 0.0;
    };

    const int i = 1;
    const double d = 2.0;

    c c_(i, d);

    BOOST_CHECK_EQUAL(i, c_.i);
    BOOST_CHECK_EQUAL(d, c_.d);
}

BOOST_AUTO_TEST_CASE(traits) {
    const int i = 1;
    const double d = 2.0;

    struct c
    {
        BOOST_DI_CTOR_TRAITS(int i, double d);

        c(int i, double d)
            : i(i), d(d)
        { }

        int i = 0;
        double d = 0.0;
    };

    c c_(i, d);

    BOOST_CHECK_EQUAL(i, c_.i);
    BOOST_CHECK_EQUAL(d, c_.d);
}

BOOST_AUTO_TEST_CASE(function) {
    const int i = 1;
    const double d = 2.0;

    struct c
    {
        static void BOOST_DI_CONSTRUCTOR(int i, double d);

        c(int i, double d)
            : i(i), d(d)
        { }

        int i = 0;
        double d = 0.0;
    };

    c c_(i, d);

    BOOST_CHECK_EQUAL(i, c_.i);
    BOOST_CHECK_EQUAL(d, c_.d);
}

#if !defined(__INTEL_COMPILER)
BOOST_AUTO_TEST_CASE(inheriting_ctors) {
    const int i = 1;
    const double d = 2.0;

    struct c0
    {
        BOOST_DI_CTOR(c0, int i, double d)
            : i(i), d(d)
        { }

        int i = 0;
        double d = 0.0;
    };

    struct c1 : public c0
    {
        using c0::c0;
    };

    c1 c1_(i, d);


    BOOST_CHECK_EQUAL(i, c1_.i);
    BOOST_CHECK_EQUAL(d, c1_.d);
}
#endif

} // namespace di
} // namespace boost

