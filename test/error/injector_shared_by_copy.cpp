//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di.hpp"

namespace di = boost::di;

test error_injector_shared_by_copy = [] {
    struct c {
        c(int) { }
    };

    auto injector = di::make_injector(
        di::bind<int>.in(di::shared)
    );

    injector.create<c>();
};

