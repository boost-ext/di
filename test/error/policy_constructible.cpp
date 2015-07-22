//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di.hpp"

namespace di = boost::di;

class config : public di::config {
public:
    static auto policies(...) noexcept {
        using namespace di::policies;
        return di::make_policies(constructible(is_bound<di::_>{}));
    }
};

test error_policy_constructible = [] {
    auto injector = di::make_injector<config>();
    injector.create<int>();
};

