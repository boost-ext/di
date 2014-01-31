//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <boost/di.hpp>

namespace di = boost::di;

namespace {

struct c
{
    BOOST_DI_INJECT(c, int i) {
        assert(i == 42);
    }
};

} // namespace

int main() {
    auto injector = di::make_injector(
        di::bind<int>::to(42)
    );

    injector.create<c>();
}

