//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#define BOOST_DI_CFG_CTOR_LIMIT_SIZE 3
#include "boost/di.hpp"

namespace di = boost::di;

test error_inject_limit_out_of_range = [] {
    struct c {
        c(int, int, int, int) { }
    };

    auto injector = di::make_injector();

    injector.create<c>();
};

