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

test empty_module = [] {
    struct empty {
        auto configure() const {
            return di::make_injector();
        }
    };

    auto injector = di::make_injector(empty{});
    expect_eq(0, injector.create<int>());
};

test modules_mix_make_injector = [] {
    constexpr auto i = 42;
    constexpr auto d = 87.0;
    constexpr auto f = 123.0f;
    const std::string s = "string";

    auto injector_string = make_injector(
        di::bind<std::string>().to(s)
    );

    struct empty {
        auto configure() const {
            return di::make_injector();
        }
    };

    struct module1 {
        di::injector<i1> configure() const {
            return di::make_injector(
                di::bind<i1, impl1>()
            );
        }
    };

    struct module2 {
        di::injector<int> configure() const {
            return di::make_injector(
                di::bind<int>().to(i_)
            );
        }

        explicit module2(int i)
            : i_(i)
        { }

        int i_ = 0;
    };

    auto injector = di::make_injector(
        empty{}
      , di::bind<double>().to(d)
      , module1{}
      , make_injector(
            di::bind<float>().to(f)
        )
      , injector_string
      , module2{i}
    );

    expect(dynamic_cast<impl1*>(injector.create<std::unique_ptr<i1>>().get()));
    expect_eq(i, injector.create<int>());
    expect_eq(d, injector.create<double>());
    expect_eq(f, injector.create<float>());
    expect_eq(s, injector.create<std::string>());
};

