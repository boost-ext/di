#include <boost/di.hpp>
#include <iostream>
#include <memory>
#include <boost/shared_ptr.hpp>

#include <boost/units/detail/utility.hpp>

namespace di = boost::di;

struct name{};
struct i1 { virtual ~i1() = default; virtual void dummy1() = 0; };
struct i2 { virtual ~i2() = default; virtual void dummy2() = 0; };
struct i3 { virtual ~i3() = default; virtual void dummy3() = 0; };
struct impl1 : i1 { void dummy1() override { } };
struct impl2 : i2 { void dummy2() override { } };
struct impl1_2 : i1, i2 { void dummy1() override { } void dummy2() override { } };
struct impl4 : impl1_2 { };
struct complex1 {
    explicit complex1(const std::shared_ptr<i1>& i1)
        : i1(i1)
    { }

    std::shared_ptr<i1> i1;
};

struct complex2 {
    int i;
    complex1 c1;
};

struct complex3 {
    complex2 c2;
};

test named_params = [] {
	constexpr auto i = 42;
	auto injector = di::make_injector(
		di::bind<int>.named(name{}).to(i)
	);
	expect(0 == injector.create<int>());
	expect(i == injector.create<di::named<int, name>>());
};

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

test external_with_scope = [] {
	constexpr auto i = 42;

	auto injector = di::make_injector(
		di::bind<int>.in(di::shared).to(i)
	);

	expect_eq(i, injector.create<int>());
};

test injectors_mix = [] {
    auto injector = di::make_injector(
        di::make_injector(
            di::bind<i1, impl1>
        )
    );

    auto object = injector.create<std::shared_ptr<complex1>>();
    expect(object->i1.get());
};

test exposed_type = [] {
    di::injector<complex1> injector = di::make_injector(
        di::bind<i1, impl1>
    );

    auto object = injector.create<std::shared_ptr<complex1>>();
    expect(dynamic_cast<i1*>(object->i1.get()));
};

test exposed_type_by_injector = [] {
    constexpr auto i = 42;

    di::injector<complex1> injector1 = di::make_injector(
        di::bind<i1, impl1>
    );

    auto injector = di::make_injector(
        injector1
      , di::bind<int>.to(i)
    );

    auto object = injector.create<std::shared_ptr<complex2>>();
    expect(dynamic_cast<i1*>(object->c1.i1.get()));
    expect_eq(i, object->i);
};

test exposed_type_by_component = [] {
    struct component {
        di::injector<complex1> configure() const {
            return di::make_injector(di::bind<i1, impl1>);
        }
    };

    constexpr auto i = 42;

    auto injector = di::make_injector(
        component{}
      , di::bind<int>.to(i)
    );

    auto object = injector.create<std::shared_ptr<complex2>>();
    expect(dynamic_cast<i1*>(object->c1.i1.get()));
    expect_eq(i, object->i);
};

test exposed_type_by_component_twice = [] {
    struct component {
        di::injector<complex1> configure() const {
            return di::make_injector(di::bind<i1, impl1>);
        }
    };

    constexpr auto i = 42;

    di::injector<complex2> injector = di::make_injector(
        component{}
      , di::bind<int>.to(i)
    );

    auto object = injector.create<std::shared_ptr<complex2>>();
    expect(dynamic_cast<i1*>(object->c1.i1.get()));
    expect_eq(i, object->i);
};

test exposed_type_by_component_mix = [] {
    static constexpr auto i = 42;

    struct component1 {
        di::injector<complex1> configure() const {
            return di::make_injector(di::bind<i1, impl1>);
        }
    };

    struct component2 {
        di::injector<complex2> configure() const {
            return di::make_injector(di::bind<int>.to(i), component1{});
        }
    };

    auto injector = di::make_injector(
        component2{}
    );

    {
    auto object = injector.create<std::shared_ptr<complex2>>();
    expect(object.get());
    }

    {
    auto object = injector.create<std::shared_ptr<complex3>>();
    expect(object.get());
    }
};

test exposed_many = [] {
    constexpr auto i = 42;

    di::injector<complex1, i1> injector1 = di::make_injector(
        di::bind<i1, impl1>
    );

    auto injector = di::make_injector(
        injector1
      , di::bind<int>.to(i)
    );

    {
    auto object = injector.create<std::shared_ptr<complex2>>();
    expect(dynamic_cast<i1*>(object->c1.i1.get()));
    expect_eq(i, object->i);
    }

    {
    auto object = injector.create<std::shared_ptr<i1>>();
    expect(dynamic_cast<impl1*>(object.get()));
    }
};

test scopes_priority = [] {
    auto injector = di::make_injector(
        di::bind<int>.to(12)
      , di::bind<int, std::integral_constant<int, 42>>
    );

    auto object = injector.create<int>();

    expect_eq(12, object);
};

test scopes_order = [] {
    auto injector = di::make_injector(
        di::bind<int>.to(41)
      , di::bind<int>.to([]{return 42;})
    );

    expect_eq(42, injector.create<int>());
};

test scopes_injector_lambda_injector = [] {
    constexpr short s = 42;
    auto injector = di::make_injector(
        di::bind<short>.to(s)
      , di::bind<int>.to([](const auto& injector){ return static_cast<int>(injector.template create<short>()); })
    );

    expect_eq(s, injector.create<int>());
};

test session_call = [] {
    struct name { };
    auto injector = di::make_injector(
        di::bind<i1, impl1>.in(di::session(name{}))
    );

    {
    injector.call(di::session_entry(name{}));
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
    injector.call(di::session_exit(name{}));
    auto object = injector.create<std::shared_ptr<i1>>();
    expect(!object.get());
    }
};

test string_creation = [] {
    struct string {
        std::string str;
    };

    expect_eq("", di::make_injector().create<string>().str);
};

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

test conversion_to_boost_shared_ptr = [] {
    struct c {
        boost::shared_ptr<int> sp;
    };

    auto injector = di::make_injector(
        di::bind<int>.in(di::shared)
    );

    auto object = injector.create<c>();
    expect(object.sp.get());
};

test one_arg_class = [] {
    struct c {
        c(int i) : i(i) { }
        int i = 0;
    };

    auto injector = di::make_injector(
        di::bind<int>.in(di::unique)
    );

    auto object = injector.create<c>();
    expect_eq(0, object.i);
};

test request_value_and_ptr_in_unique = [] {
    struct c {
        int i = 0;
        int* ptr = nullptr;
    };

    auto injector = di::make_injector(
        di::bind<int>.in(di::unique)
    );

    injector.create<c>();
};

test inject = [] {
    constexpr auto i = 42;

    struct c {
        c(std::initializer_list<int>) { }

        c(int, double, float) { }

        BOOST_DI_INJECT(c, int i, double d)
            : i(i), d(d)
        { }

        int i = 0;
        double d = 0.0;
    };

    auto injector = di::make_injector(
        di::bind<int>.to(i)
    );

    auto object = injector.create<c>();

    expect_eq(i, object.i);
    expect_eq(0.0, object.d);
};

test automatic_inject = [] {
    constexpr auto i = 42;

    struct c {
        c(std::initializer_list<int>) { }

        c(int i, double d)
            : i(i), d(d)
        { }

        int i = 0;
        double d = 0.0;
    };

    auto injector = di::make_injector(
        di::bind<int>.to(i)
    );

    auto object = injector.create<c>();

    expect_eq(i, object.i);
    expect_eq(0.0, object.d);
};

test automatic_inject_with_initializer_list = [] {
    constexpr auto i = 42;

    struct c {
        c(int i, std::initializer_list<int> il)
            : i(i), il(il)
        { }

        int i = 0;
        std::initializer_list<int> il;
    };

    auto injector = di::make_injector(
        di::bind<int>.to(i)
    );

    auto object = injector.create<c>();

    expect_eq(i, object.i);
    expect_eq(0, object.il.size());
};

test named_polymorphic_agreggate = [] {
    struct c {
        di::named<std::shared_ptr<i1>, name> sp;
    };

    auto injector = di::make_injector(
        di::bind<i1, impl1>.named(name{})
    );

    auto object = injector.create<c>();
    auto sp = static_cast<std::shared_ptr<i1>>(object.sp);

	expect(dynamic_cast<impl1*>(sp.get()));
};

test named_polymorphic = [] {
    struct c {
        explicit c(di::named<std::shared_ptr<i1>, name> sp)
            : sp(sp)
        { }

        std::shared_ptr<i1> sp;
    };

    auto injector = di::make_injector(
        di::bind<i1, impl1>.named(name{})
    );

    auto object = injector.create<c>();

	expect(dynamic_cast<impl1*>(object.sp.get()));
};

test bind_chars_to_string = [] {
    auto injector = di::make_injector(
        di::bind<std::string>.to("str")
    );

    expect_eq("str", injector.create<std::string>());
};

test ctor_refs = [] {
    struct c {
        c(const std::shared_ptr<i1>& sp, int& i, const double& d)
            : i(i), d(d)
        {
            sp->dummy1();
        }

        int& i;
        const double& d;
    };

    struct c_inject {
        BOOST_DI_INJECT(c_inject, const std::shared_ptr<i1>& sp, int& i, const double& d)
            : i(i), d(d)
        {
            sp->dummy1();
        }

        int& i;
        const double& d;
    };

    struct c_aggregate {
        const std::shared_ptr<i1>& sp;
        int& i;
        const double& d;
    };

	auto test = [](auto type, const auto& bind_i1) {
        auto i = 0;
        constexpr auto d = 0.0;

        auto injector = di::make_injector(
            di::bind<int>.to(i)
          , di::bind<double>.to(d)
          , bind_i1
        );

        auto object = injector.template create<typename decltype(type)::type>();

        expect_eq(&i, &object.i);
        expect_eq(&d, &object.d);
    };

    test(test_type<c>{}, di::bind<i1, impl1>);
    test(test_type<c_inject>{}, di::bind<i1, impl1>);
    test(test_type<c_aggregate>{}, di::bind<i1, impl1>);

    std::shared_ptr<i1> sp = std::make_shared<impl1>();
    test(test_type<c>{}, di::bind<i1>.to(sp));
    test(test_type<c_inject>{}, di::bind<i1>.to(sp));
    test(test_type<c_aggregate>{}, di::bind<i1>.to(sp));
};

#if 0
test bind_not_compatibile_types = [] {
    di::make_injector(di::bind<int, impl1>).create<int>();
};
#endif

