//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include "boost/di.hpp"

namespace di = boost::di;

auto a = []{};
auto b = []{};
auto name = []{};

struct i1 { virtual ~i1() noexcept = default; virtual void dummy1() = 0; };
struct impl1 : i1 { void dummy1() override { } };

test named_to = [] {
    constexpr auto i = 42;
    constexpr auto d = 87.0;

    struct c {
        BOOST_DI_INJECT(c, (named = a) int i, (named = b) double d)
            : i_(i), d_(d)
        { }

        int i_ = 0;
        double d_ = 0.0;
    };

    auto injector = di::make_injector(
        di::bind<int>.named(a).to(i)
      , di::bind<double>.to(d).named(b)
    );

    auto object = injector.create<c>();

    expect_eq(i, object.i_);
    expect_eq(d, object.d_);
};

test named_polymorphic = [] {
    struct c {
        BOOST_DI_INJECT(explicit c, (named = name) std::shared_ptr<i1> sp)
            : sp(sp)
        { }

        std::shared_ptr<i1> sp;
    };

    auto injector = di::make_injector(
        di::bind<i1, impl1>.named(name)
    );

    auto object = injector.create<c>();

    expect(dynamic_cast<impl1*>(object.sp.get()));
};

struct c {
    BOOST_DI_INJECT(explicit c, (named = name) int i);
    int i = 0;
};

c::c(int i) : i(i) { }

test named_with_ctor_def_decl = [] {
    constexpr auto i = 42;

    auto injector = di::make_injector(
        di::bind<int>.to(i).named(name)
    );

    auto object = injector.create<c>();

    expect_eq(i, object.i);
};

test named_parameters_with_shared_scope = [] {
    struct c {
        BOOST_DI_INJECT(c, (named = a) const std::shared_ptr<i1>& n1, (named = b) std::shared_ptr<i1> n2)
            : n1(n1), n2(n2)
        { }

        std::shared_ptr<i1> n1;
        std::shared_ptr<i1> n2;
    };

    auto injector = di::make_injector(
        di::bind<i1, impl1>.named(a).in(di::unique)
      , di::bind<i1>.named(b).to(std::make_shared<impl1>())
    );

    auto object = injector.create<c>();

    expect(object.n1 != object.n2);
};

