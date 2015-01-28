//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>
#include <memory>
#include <cassert>

namespace di = boost::di;

struct i1 { virtual ~i1() noexcept = default; virtual void dummy1() = 0; };
struct impl1 : i1 { void dummy1() override { } };

auto my_session = []{};

int main() {
    auto injector = di::make_injector(
        di::bind<i1, impl1>.in(
            di::session(my_session)
        )
    );

    assert(nullptr == injector.create<std::shared_ptr<i1>>());
    injector.call(di::session_entry(my_session));
    assert(injector.create<std::shared_ptr<i1>>()
           ==
           injector.create<std::shared_ptr<i1>>()
    );
    injector.call(di::session_exit(my_session));
    assert(nullptr == injector.create<std::shared_ptr<i1>>());
}

