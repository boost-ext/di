//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include <string>
#include <functional>
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

    expect(i == object.i_);
    expect(d == object.d_);
};

test named_polymorphic = [] {
    struct c {
        BOOST_DI_INJECT(explicit c, (named = name) std::shared_ptr<i1> sp)
            : sp(sp)
        { }

        std::shared_ptr<i1> sp;
    };

    auto injector = di::make_injector(
        di::bind<i1>().named(name).to<impl1>()
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

    expect(i == object.i);
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
        di::bind<i1>().named(a).in(di::unique).to<impl1>()
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
        #if defined(_MSC_VER)
            di::bind<i2, i1>().in(scope).to<impl1_2>()
        #else
            di::bind<i2, i1>().in(scope).template to<impl1_2>()
        #endif
        );

        std::shared_ptr<i1> object_1 = injector;
        std::shared_ptr<i1> object_2 = injector;
        auto result = dynamic_cast<impl1_2*>(object_1.get()) == dynamic_cast<impl1_2*>(object_2.get());
        expect(result == same);
    };

    test(di::unique, false);
    test(di::singleton, true);
};

test any_of_with_scope_split = [] {
    auto test = [](auto scope, auto same) {
        auto injector = di::make_injector(
        #if defined(_MSC_VER)
            di::bind<i1>().in(scope).to<impl1_2>()
          , di::bind<i2>().in(scope).to<impl1_2>()
        #else
            di::bind<i1>().in(scope).template to<impl1_2>()
          , di::bind<i2>().in(scope).template to<impl1_2>()
        #endif
        );

        std::shared_ptr<i1> object_1 = injector;
        std::shared_ptr<i2> object_2 = injector;
        auto result = dynamic_cast<impl1_2*>(object_1.get()) == dynamic_cast<impl1_2*>(object_2.get());
        expect(result == same);
    };

    test(di::unique, false);
    test(di::singleton, false);
};

test any_of_unique = [] {
    auto injector = di::make_injector(
        di::bind<i1, i2>().in(di::unique).to<impl1_2>()
    );

    auto object_1 = injector.create<std::shared_ptr<i1>>();
    auto object_2 = injector.create<std::shared_ptr<i2>>();
    expect(dynamic_cast<impl1_2*>(object_1.get()) != dynamic_cast<impl1_2*>(object_2.get()));
};

test bind_int_to_static_value = [] {
    auto injector = di::make_injector(
        di::bind<int>().to<std::integral_constant<int, 42>>()
    );

    auto object = injector.create<int>();

    expect(42 == object);
};

test override_priority = [] {
    auto injector = di::make_injector(
        di::bind<int>().to(12) [di::override]
      , di::bind<int>().to<std::integral_constant<int, 42>>()
    );

    auto object = injector.create<int>();

    expect(12 == object);
};

test override_priority_order = [] {
    auto injector = di::make_injector(
        di::bind<int>().to<std::integral_constant<int, 41>>()
      , di::bind<int>().to([]{return 42;}) [di::override]
    );

    expect(42 == injector.create<int>());
};

test override_priority_interface = [] {
    auto injector = di::make_injector(
        di::bind<i1>().to<impl1>()
      , di::bind<i1>().to<impl1_int>() [di::override]
    );

    auto object = injector.create<std::unique_ptr<i1>>();
    expect(dynamic_cast<impl1_int*>(object.get()));
};

test override_priority_interface_module = [] {
    struct module {
        auto configure() const {
            return di::make_injector(
                di::bind<i1>().to<impl1_int>()
            );
        }
    };

    auto injector = di::make_injector(
        module{}
      , di::bind<i1>().to<impl1>() [di::override]
    );

    auto object = injector.create<std::unique_ptr<i1>>();
    expect(dynamic_cast<impl1*>(object.get()));
};

test bind_non_interface_in_singleton_scope = [] {
    struct c { };

    auto injector = di::make_injector(
        di::bind<c>().in(di::singleton)
    );

    expect(injector.create<std::shared_ptr<c>>() == injector.create<std::shared_ptr<c>>());
};

test bind_const_ref_type_in_singleton_scope = [] {
    struct c_inject {
        BOOST_DI_INJECT(c_inject, const int& i1, const int& i2) {
            expect(&i1 == &i2);
        }
    };

    struct c {
        c(const int& i1, const int& i2) {
            expect(&i1 == &i2);
        }
    };

    auto injector = di::make_injector(
        di::bind<int>().in(di::singleton)
    );

    injector.create<c_inject>();
    di::make_injector().create<c_inject>();

    injector.create<c>();
    di::make_injector().create<c>();
};

test scopes_external_shared = [] {
    auto i = std::make_shared<int>(42);

    auto injector = di::make_injector(
        di::bind<int>().to(i)
    );

    {
    auto object = injector.create<std::shared_ptr<int>>();
    expect(i.get() == object.get());
    expect(42 == *i);
    }

    {
    ++*i;
    auto object = injector.create<std::shared_ptr<int>>();
    expect(43 == *i);
    }
};

test scopes_external_lambda = [] {
    auto i = std::make_shared<int>(42);

    auto injector = di::make_injector(
        di::bind<int>().to([&i]{return i;})
    );

    {
    auto object = injector.create<std::shared_ptr<int>>();
    expect(i.get() == object.get());
    expect(42 == *i);
    }

    {
    ++*i;
    auto object = injector.create<std::shared_ptr<int>>();
    expect(43 == *i);
    }
};

test scopes_external_lambda_injector = [] {
    auto i = std::make_shared<int>(42);

    auto injector = di::make_injector(
        di::bind<int>().to([&i](const auto&){return i;})
    );

    {
    auto object = injector.create<std::shared_ptr<int>>();
    expect(i.get() == object.get());
    expect(42 == *i);
    }

    {
    ++*i;
    auto object = injector.create<std::shared_ptr<int>>();
    expect(43 == *i);
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

    expect(i == object.i_);
    expect(d == object.d_);
};

test bind_chars_to_string = [] {
    auto injector = di::make_injector(
        di::bind<std::string>().to("str")
    );

    expect("str" == injector.create<std::string>());
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
          , di::bind<i2>().to<impl2>()
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
        BOOST_DI_INJECT(functions, const std::function<int()>& fi, std::function<double()> fd)
            : fi(fi)
            , fd(fd)
        { }

        std::function<int()> fi;
        std::function<double()> fd;
    };

    auto injector = di::make_injector(
        di::bind<std::function<int()>>().to([&]{ return i; })
      , di::bind<std::function<double()>>().to(std::bind(&return_double, d))
    );

    auto object = injector.create<functions>();

    expect(i == object.fi());
    expect(d == object.fd());
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
    expect(i == dynamic_cast<impl1_int*>(object.get())->i);
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
    expect(i == dynamic_cast<impl1_int*>(object.get())->i);
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

    expect(s == injector.create<int>());
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
            di::bind<i1>().to<impl1>() // cross compiler call
          , di::bind<i2>.to<impl2>() // requires variable templates
          , di::bind<int>().to(i)
        );

        auto object = injector.create<c>();
        expect(i == object.i_);
        expect(dynamic_cast<impl1*>(object.i1_.get()));
        expect(dynamic_cast<impl2*>(object.i2_.get()));
    };
#endif

