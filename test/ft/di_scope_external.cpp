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
struct i2 { virtual ~i2() noexcept = default; virtual void dummy2() = 0; };
struct i3 { virtual ~i3() noexcept = default; virtual void dummy3() = 0; };
struct impl1 : i1 { void dummy1() override { } };
struct impl1_int : i1 { impl1_int(int i) : i(i) { } void dummy1() override { } int i = 0; };
struct impl2 : i2 { void dummy2() override { } };
struct impl1_2 : i1, i2 { void dummy1() override { } void dummy2() override { } };
struct impl4 : impl1_2 { };

struct impl1_with_i2 : i1 {
    explicit impl1_with_i2(std::shared_ptr<i2> i2)
        : i2_(i2)
    { }

    void dummy1() override { }

    std::shared_ptr<i2> i2_;
};

test scopes_external_shared = [] {
    auto i = std::make_shared<int>(42);

    auto injector = di::make_injector(
        di::bind<int>().to(i)
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
        di::bind<int>().to([&i]{return i;})
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
        di::bind<int>().to([&i](const auto&){return i;})
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
        di::bind<int>().to(i)
      , di::bind<double>().to(d)
    );

    auto object = injector.create<refs>();

    expect_eq(i, object.i_);
    expect_eq(d, object.d_);
};

test bind_chars_to_string = [] {
    auto injector = di::make_injector(
        di::bind<std::string>().to("str")
    );

    expect_eq("str", injector.create<std::string>());
};

test dynamic_binding_using_polymorphic_lambdas_with_dependend_interfaces = [] {
    auto test = [&](bool debug_property) {
        auto injector = make_injector(
            di::bind<i1>().to([&](const auto& injector) -> std::shared_ptr<i1> {
                if (debug_property) {
                    return std::make_shared<impl1>();
                }

                return injector.template create<std::shared_ptr<impl1_with_i2>>();
            })
          , di::bind<i2, impl2>()
        );

        return injector.create<std::shared_ptr<i1>>();
    };

    {
    auto object = test(false);
    expect(dynamic_cast<impl1_with_i2*>(object.get()));
    expect(dynamic_cast<impl2*>(dynamic_cast<impl1_with_i2*>(object.get())->i2_.get()));
    }

    {
    auto object = test(true);
    expect(dynamic_cast<impl1*>(object.get()));
    }
};

double return_double(double d) { return d; }
long return_long(long l) { return l; }

test bind_to_function_ptr = [] {
    constexpr auto i = 42;
    constexpr auto d = 87.0;

    struct functions {
        functions(const std::function<int()>& fi, std::function<double()> fd)
            : fi(fi)
            , fd(fd)
        { }

        std::function<int()> fi;
        std::function<double()> fd;
    };

    auto injector = di::make_injector(
        di::bind<std::function<int()>>.to([&]{ return i; })
      , di::bind<std::function<double()>>.to(std::bind(&return_double, d))
    );

    auto object = injector.create<functions>();

    expect_eq(i, object.fi());
    expect_eq(d, object.fd());
};

test runtime_factory_impl = [] {
    constexpr auto i = 42;

    auto test = [&](bool debug_property) {
        auto injector = make_injector(
            di::bind<int>().to(i)
          , di::bind<i1>().to([&](const auto& injector) -> std::shared_ptr<i1> {
                if (debug_property) {
                    return std::make_shared<impl1>();
                }

                return injector.template create<std::shared_ptr<impl1_int>>();
            })
        );

        return injector.create<std::shared_ptr<i1>>();
    };

    {
    auto object = test(false);
    expect(dynamic_cast<impl1_int*>(object.get()));
    expect_eq(i, dynamic_cast<impl1_int*>(object.get())->i);
    }

    {
    auto object = test(true);
    expect(dynamic_cast<impl1*>(object.get()));
    }
};

struct call_operator {
    bool& b;

    template<class TInjector>
    std::shared_ptr<i1> operator()(const TInjector& injector) const {
        if (b) {
            return injector.template create<std::shared_ptr<impl1>>();
        }

        return injector.template create<std::shared_ptr<impl1_int>>();
    }
};

test runtime_factory_call_operator_impl = [] {
    constexpr auto i = 42;

    auto test = [&](bool debug_property) {
        auto injector = make_injector(
            di::bind<int>().to(i)
          , di::bind<i1>().to(call_operator{debug_property})
        );

        return injector.create<std::shared_ptr<i1>>();
    };

    {
    auto object = test(false);
    expect(dynamic_cast<impl1_int*>(object.get()));
    expect_eq(i, dynamic_cast<impl1_int*>(object.get())->i);
    }

    {
    auto object = test(true);
    expect(dynamic_cast<impl1*>(object.get()));
    }
};

test scopes_injector_lambda_injector = [] {
    constexpr short s = 42;
    auto injector = di::make_injector(
        di::bind<short>().to(s)
      , di::bind<int>().to([](const auto& injector){ return static_cast<int>(injector.template create<short>()); })
    );

    expect_eq(s, injector.create<int>());
};

