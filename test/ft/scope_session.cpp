//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include "boost/di.hpp"

namespace di = boost::di;

struct i1 { virtual ~i1() noexcept = default; virtual void dummy1() = 0; };
struct impl1 : i1 { void dummy1() override { } };

auto name = []{};

test session_call = [] {
    auto injector = di::make_injector(
        di::bind<i1, impl1>.in(di::session(name))
    );

    {
    injector.call(di::session_entry(name));
    auto object = injector.create<std::shared_ptr<i1>>();
    expect(object.get());
    expect(dynamic_cast<i1*>(object.get()));
    }

    {
    injector.call(di::session_exit(int{}));
    auto object = injector.create<std::shared_ptr<i1>>();
    expect(object.get());
    }

    {
    injector.call(di::session_exit(name));
    auto object = injector.create<std::shared_ptr<i1>>();
    expect(!object.get());
    }
};

