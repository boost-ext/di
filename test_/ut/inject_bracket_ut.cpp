//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#define BOOST_DI_CFG_INJECT_BRACKET
#include "boost/di/cpp_0x/inject.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/config.hpp>

namespace boost {
namespace di {

BOOST_AUTO_TEST_CASE(empty) {
    struct c
    {
        BOOST_DI_INJECT(c, ()) { }
    };

    c c_;
}

BOOST_AUTO_TEST_CASE(empty_traits) {
    struct c
    {
        BOOST_DI_INJECT_TRAITS(());
        c() { }
    };

    c c_;
}

BOOST_AUTO_TEST_CASE(explicit_with_default) {
    struct c
    {
        enum { DEFAULT = 42 };

        BOOST_DI_INJECT(explicit c, (int i = DEFAULT))
            : i(i)
        { }

        int i = 0;
    };

    c c_;

    BOOST_CHECK_EQUAL(static_cast<int>(c::DEFAULT), c_.i);
}

BOOST_AUTO_TEST_CASE(set_to_default) {
    struct c
    {
        BOOST_DI_INJECT(c, ()) = default;
        int i = 0;
    };

    c c_;

    BOOST_CHECK_EQUAL(0, c_.i);
}

BOOST_AUTO_TEST_CASE(params) {
    const int i = 1;
    const double d = 2.0;

    struct c
    {
        BOOST_DI_INJECT(c, (int i, double d))
            : i(i), d(d)
        { }

        int i = 0;
        double d = 0.0;
    };

    c c_(i, d);

    BOOST_CHECK_EQUAL(i, c_.i);
    BOOST_CHECK_EQUAL(d, c_.d);
}

BOOST_AUTO_TEST_CASE(traits) {
    const int i = 1;
    const double d = 2.0;

    struct c
    {
        BOOST_DI_INJECT_TRAITS((int i, double d));

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
        static void BOOST_DI_INJECTOR(int i, double d);

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

#if !defined(BOOST_INTEL) &&                                                    \
    !defined(BOOST_MSVC) &&                                                     \
    !(defined(BOOST_GCC) && (BOOST_GCC < 40800)) &&                             \
    !(defined(BOOST_CLANG) && (__clang_major__ >= 3) && (__clang_minor__ < 3))

BOOST_AUTO_TEST_CASE(inheriting_ctors) {
    const int i = 1;
    const double d = 2.0;

    struct c0
    {
        BOOST_DI_INJECT(c0, (int i, double d))
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

