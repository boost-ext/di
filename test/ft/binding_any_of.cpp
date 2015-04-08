//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include "boost/di.hpp"

namespace di = boost::di;

struct i1 { virtual ~i1() noexcept = default; virtual void dummy1() = 0; };
struct i2 { virtual ~i2() noexcept = default; virtual void dummy2() = 0; };
struct i3 { virtual ~i3() noexcept = default; virtual void dummy3() = 0; };
struct impl1 : i1 { void dummy1() override { } };
struct impl1_int : i1 { impl1_int(int i) : i(i) { } void dummy1() override { } int i = 0; };
struct impl2 : i2 { void dummy2() override { } };
struct impl1_2 : i1, i2 { void dummy1() override { } void dummy2() override { } };

test any_of = [] {
    auto injector = di::make_injector(
        di::bind<impl1_2>
    );

    auto object = injector.create<std::unique_ptr<impl1_2>>();
    expect(dynamic_cast<i1*>(object.get()));
    expect(dynamic_cast<i2*>(object.get()));
    expect(!dynamic_cast<i3*>(object.get()));
};

test any_of_with_scope = [] {
    auto test = [](auto scope, auto same) {
        auto injector = di::make_injector(
            di::bind<di::any_of<i2, i1>, impl1_2>.in(scope)
        );

        auto object_1 = injector.template create<std::shared_ptr<i1>>();
        auto object_2 = injector.template create<std::shared_ptr<i2>>();
        auto result = dynamic_cast<impl1_2*>(object_1.get()) == dynamic_cast<impl1_2*>(object_2.get());
        expect_eq(result, same);
    };

    test(di::unique, false);
    test(di::shared, true);
    test(di::singleton, true);
};

test any_of_with_scope_split = [] {
    auto test = [](auto scope, auto same) {
        auto injector = di::make_injector(
            di::bind<i1, impl1_2>.in(scope)
          , di::bind<i2, impl1_2>.in(scope)
        );

        auto object_1 = injector.template create<std::shared_ptr<i1>>();
        auto object_2 = injector.template create<std::shared_ptr<i2>>();
        auto result = dynamic_cast<impl1_2*>(object_1.get()) == dynamic_cast<impl1_2*>(object_2.get());
        expect_eq(result, same);
    };

    test(di::unique, false);
    test(di::shared, false);
    test(di::singleton, false);
};

test any_of_unique = [] {
    auto injector = di::make_injector(
        di::bind<di::any_of<i1, i2>, impl1_2>.in(di::unique)
    );

    auto object_1 = injector.create<std::shared_ptr<i1>>();
    auto object_2 = injector.create<std::shared_ptr<i2>>();
    expect(dynamic_cast<impl1_2*>(object_1.get()) != dynamic_cast<impl1_2*>(object_2.get()));
};


