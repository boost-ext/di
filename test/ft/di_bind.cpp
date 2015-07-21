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

auto a = []{};
auto b = []{};
struct name_{ } name;

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
        di::bind<int>().named(a).to(i)
      , di::bind<double>().to(d).named(b)
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
        di::bind<i1, impl1>().named(name)
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
        di::bind<int>().to(i).named(name)
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
        di::bind<i1, impl1>().named(a).in(di::unique)
      , di::bind<i1>().named(b).to(std::make_shared<impl1>())
    );

    auto object = injector.create<c>();

    expect(object.n1 != object.n2);
};

test any_of = [] {
    auto injector = di::make_injector(
        di::bind<impl1_2>()
    );

    auto object = injector.create<std::unique_ptr<impl1_2>>();
    expect(dynamic_cast<i1*>(object.get()));
    expect(dynamic_cast<i2*>(object.get()));
    expect(!dynamic_cast<i3*>(object.get()));
};

test any_of_with_scope = [] {
    auto test = [](auto scope, auto same) {
        auto injector = di::make_injector(
            di::bind<di::any_of<i2, i1>, impl1_2>().in(scope)
        );

        auto object_1 = injector.template create<std::shared_ptr<i1>>();
        auto object_2 = injector.template create<std::shared_ptr<i2>>();
        auto result = dynamic_cast<impl1_2*>(object_1.get()) == dynamic_cast<impl1_2*>(object_2.get());
        expect_eq(result, same);
    };

    test(di::unique, false);
    test(di::singleton, true);
};

test any_of_with_scope_split = [] {
    auto test = [](auto scope, auto same) {
        auto injector = di::make_injector(
            di::bind<i1, impl1_2>().in(scope)
          , di::bind<i2, impl1_2>().in(scope)
        );

        auto object_1 = injector.template create<std::shared_ptr<i1>>();
        auto object_2 = injector.template create<std::shared_ptr<i2>>();
        auto result = dynamic_cast<impl1_2*>(object_1.get()) == dynamic_cast<impl1_2*>(object_2.get());
        expect_eq(result, same);
    };

    test(di::unique, false);
    test(di::singleton, false);
};

test any_of_unique = [] {
    auto injector = di::make_injector(
        di::bind<di::any_of<i1, i2>, impl1_2>().in(di::unique)
    );

    auto object_1 = injector.create<std::shared_ptr<i1>>();
    auto object_2 = injector.create<std::shared_ptr<i2>>();
    expect(dynamic_cast<impl1_2*>(object_1.get()) != dynamic_cast<impl1_2*>(object_2.get()));
};



test bind_int_to_static_value = [] {
    auto injector = di::make_injector(
        di::bind<int, std::integral_constant<int, 42>>()
    );

    auto object = injector.create<int>();

    expect_eq(42, object);
};

test override_priority = [] {
    auto injector = di::make_injector(
        di::bind<int>().to(12) [di::override]
      , di::bind<int, std::integral_constant<int, 42>>()
    );

    auto object = injector.create<int>();

    expect_eq(12, object);
};

test override_priority_order = [] {
    auto injector = di::make_injector(
        di::bind<int, std::integral_constant<int, 41>>()
      , di::bind<int>().to([]{return 42;}) [di::override]
    );

    expect_eq(42, injector.create<int>());
};

test override_priority_interface = [] {
    auto injector = di::make_injector(
        di::bind<i1, impl1>()
      , di::bind<i1, impl1_int>() [di::override]
    );

    auto object = injector.create<std::unique_ptr<i1>>();
    expect(dynamic_cast<impl1_int*>(object.get()));
};

test override_priority_interface_module = [] {
    struct module {
        auto configure() const {
            return di::make_injector(
                di::bind<i1, impl1_int>()
            );
        }
    };

    auto injector = di::make_injector(
        module{}
      , di::bind<i1, impl1>() [di::override]
    );

    auto object = injector.create<std::unique_ptr<i1>>();
    expect(dynamic_cast<impl1*>(object.get()));
};

#if defined(__cpp_variable_templates)
    test bind_mix = [] {
        constexpr auto i = 42;

        struct c {
            c(int i_, std::unique_ptr<i1> i1_, std::unique_ptr<i2> i2_)
                : i_(i_), i1_(std::move(i1_)), i2_(std::move(i2_))
            { }

            int i_ = 0;
            std::unique_ptr<i1> i1_;
            std::unique_ptr<i2> i2_;
        };

        auto injector = di::make_injector(
            di::bind<i1, impl1>()() // cross platform call dependency extension
          , di::bind<i2, impl2>() // requires variable templates
          , di::bind<int>().to(i)
        );

        auto object = injector.create<c>();
        expect_eq(i, object.i_);
        expect(dynamic_cast<impl1*>(object.i1_.get()));
        expect(dynamic_cast<impl2*>(object.i2_.get()));
    };
#endif

