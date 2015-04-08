//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include "boost/di.hpp"

namespace di = boost::di;

test scopes_external_shared = [] {
    auto i = std::make_shared<int>(42);

    auto injector = di::make_injector(
        di::bind<int>.to(i)
    );

    {
    auto object = injector.create<std::shared_ptr<int>>();
    expect_eq(i.get(), object.get());
    expect_eq(42, *i);
    }

    {
    ++*i;
    auto object = injector.create<std::shared_ptr<int>>();
    expect_eq(43, *i);
    }
};

test scopes_external_lambda = [] {
    auto i = std::make_shared<int>(42);

    auto injector = di::make_injector(
        di::bind<int>.to([&i]{return i;})
    );

    {
    auto object = injector.create<std::shared_ptr<int>>();
    expect_eq(i.get(), object.get());
    expect_eq(42, *i);
    }

    {
    ++*i;
    auto object = injector.create<std::shared_ptr<int>>();
    expect_eq(43, *i);
    }
};

test scopes_external_lambda_injector = [] {
    auto i = std::make_shared<int>(42);

    auto injector = di::make_injector(
        di::bind<int>.to([&i](const auto&){return i;})
    );

    {
    auto object = injector.create<std::shared_ptr<int>>();
    expect_eq(i.get(), object.get());
    expect_eq(42, *i);
    }

    {
    ++*i;
    auto object = injector.create<std::shared_ptr<int>>();
    expect_eq(43, *i);
    }
};

test externals_ref_cref = [] {
    auto i = 42;
    const auto d = 87.0;

    struct refs {
        BOOST_DI_INJECT(refs
            , int& i
            , const double& d
        ) : i_(i)
          , d_(d)
        { }

        int& i_;
        const double& d_;
    };

    auto injector = make_injector(
        di::bind<int>.to(std::ref(i))
      , di::bind<double>.to(std::cref(d))
    );

    auto object = injector.create<refs>();

    expect_eq(i, object.i_);
    expect_eq(d, object.d_);
};

