//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include "boost/di.hpp"

namespace di = boost::di;

struct i {
    virtual ~i() noexcept = default; virtual void dummy() = 0;
};

struct c {
    c(int, std::unique_ptr<i>) {}
};

test exposed_not_creatable = [] {
    di::injector<i> injector = di::make_injector();
};

