//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di.hpp"
#include "boost/di/policies/constructible.hpp"

namespace di = boost::di;

template<class>
class config : public di::config<> {
public:
    auto policies() const noexcept {
        using namespace di::policies;
        return di::make_policies(constructible(is_bound<_>{}));
    }
};

test error_policy_constructible = [] {
    auto injector = di::make_injector<config>();
    injector.create<int>();
};

