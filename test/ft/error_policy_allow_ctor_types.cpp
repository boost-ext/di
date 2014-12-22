//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di.hpp"
#include "boost/di/policies/allow_ctor_types.hpp"

namespace di = boost::di;

class config : public di::config {
public:
    auto policies() const noexcept {
        using namespace di::policies;
        return di::make_policies(allow_ctor_types(is_bound<_>{}));
    }
};

test error_policy_allow_ctor_types = [] {
    auto injector = di::make_injector<config>();
    injector.create<int>();
};

