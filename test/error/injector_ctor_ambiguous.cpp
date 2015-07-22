//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di.hpp"

namespace di = boost::di;

test injector_ctor_ambiguous = [] {
    struct ctor {
        ctor(int, double) { }
        ctor(double, int) { }
    };

    di::make_injector().create<ctor>();
};

