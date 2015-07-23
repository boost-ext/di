//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>
#include <memory>
#include <cassert>

namespace di = boost::di;

struct i1 { virtual ~i1() noexcept = default; virtual void dummy1() = 0; };
struct i2 { virtual ~i2() noexcept = default; virtual void dummy2() = 0; };
struct impl : i1, i2 { void dummy1() override { } void dummy2() override { } };

int main() {
    auto injector = di::make_injector(
        di::bind<di::any_of<i1, i2>, impl>()
    );

    auto object1 = injector.create<std::shared_ptr<i1>>();
    auto object2 = injector.create<std::shared_ptr<i2>>();
    assert(dynamic_cast<impl*>(object1.get()));
    assert(dynamic_cast<impl*>(object2.get()));
}

