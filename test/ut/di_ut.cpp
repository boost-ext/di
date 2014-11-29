#include <boost/di.hpp>
#include <iostream>
#include <memory>

#include <boost/units/detail/utility.hpp>

#include "test.hpp"

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
    //expect(i == injector.create<const di::named<int, name>&>());
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

test scopes_priority = [] {
    auto injector = di::make_injector(
        di::bind<int>.to(41)
      , di::bind<int>.to([]{return 42;})
      //, di::bind<int>.to(40)
    );

    expect_eq(42, injector.create<int>());
};

test scopes_injector_lambda_injector = [] {
    constexpr short s = 42;
    auto injector = di::make_injector(
        di::bind<short>.to(s)
      , di::bind<int>.to([](auto& injector){ return injector.template create<short>(); })
    );

    expect_eq(s, injector.create<int>());
};

test scopes_external_ref = [] {
    int i = 42;

    auto injector = di::make_injector(
        di::bind<int>.to(i) //ref
    );

    (void)injector;
};

