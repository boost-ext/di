//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di.hpp"

namespace di = boost::di;

test error_scope_traits_external_not_referable = [] {
    struct c {
        c(int&) { }
    };

    auto injector = di::make_injector(
        di::bind<int>().to(42) // lvalue can't be converted to reference
    );

    injector.create<c>();
};

