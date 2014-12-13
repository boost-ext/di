//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/inject.hpp"

namespace boost { namespace di {

test empty_ctor = [] {
    struct c {
        BOOST_DI_INJECT(c, void) { }
    };

    c c_;
};

test empty_traits = [] {
    struct c {
        BOOST_DI_INJECT_TRAITS();
        c() { }
    };

    c c_;
};

test explicit_with_default = [] {
    struct c {
        enum { DEFAULT = 42 };

        BOOST_DI_INJECT(explicit c, int i = DEFAULT)
            : i(i)
        { }

        int i = 0;
    };

    c c_;

    expect_eq(static_cast<int>(c::DEFAULT), c_.i);
};

test set_to_default = [] {
    struct c {
        BOOST_DI_INJECT(c,) = default;
        int i = 0;
    };

    c c_;

    expect_eq(0, c_.i);
};

test params = [] {
    struct c {
        BOOST_DI_INJECT(c, int i, double d)
            : i(i), d(d)
        { }

        int i = 0;
        double d = 0.0;
    };

    constexpr int i = 1;
    constexpr double d = 2.0;

    c c_(i, d);

    expect_eq(i, c_.i);
    expect_eq(d, c_.d);
};

test traits = [] {
    constexpr int i = 1;
    constexpr double d = 2.0;

    struct c {
        BOOST_DI_INJECT_TRAITS(int i, double d);

        c(int i, double d)
            : i(i), d(d)
        { }

        int i = 0;
        double d = 0.0;
    };

    c c_(i, d);

    expect_eq(i, c_.i);
    expect_eq(d, c_.d);
};

test function = [] {
    constexpr int i = 1;
    constexpr double d = 2.0;

    struct c {
        static void BOOST_DI_INJECTOR(int i, double d);

        c(int i, double d)
            : i(i), d(d)
        { }

        int i = 0;
        double d = 0.0;
    };

    c c_(i, d);

    expect_eq(i, c_.i);
    expect_eq(d, c_.d);
};

test inheriting_ctors = [] {
    constexpr int i = 1;
    constexpr double d = 2.0;

    struct c0 {
        BOOST_DI_INJECT(c0, int i, double d)
            : i(i), d(d)
        { }

        int i = 0;
        double d = 0.0;
    };

    struct c1 : public c0 {
        using c0::c0;
    };

    c1 c1_(i, d);

    expect_eq(i, c1_.i);
    expect_eq(d, c1_.d);
};

}} // boost::di

