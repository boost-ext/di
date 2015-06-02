//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#define BOOST_DI_CFG_CTOR_LIMIT_SIZE 10
#include <boost/di.hpp>

namespace di = boost::di;

test inject_ctor_limit_size = [] {
    struct c {
        BOOST_DI_INJECT(c, int /*p1*/, int /*p2*/, int /*p3*/, int /*p4*/, int /*p5*/, int /*p6*/, int /*p7*/, int /*p8*/, int /*p9*/, int /*p10*/)
        { }
    };

    auto injector = di::make_injector();
    injector.create<c>();
};

