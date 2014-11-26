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
struct complex1 { complex1(std::shared_ptr<i1>) { } };

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
        //di::bind<impl1_2>
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

	expect_eq(42, injector.create<int>());
};

test lazy_scope = [] {
    di::injector<complex1> injector1 = di::make_injector(
        di::bind<i1, impl1>
    );

    std::cout << boost::units::detail::demangle(typeid(decltype(injector1)::deps).name()) << std::endl;
    //expect(injector1.create<complex1*>());

    //auto injector = di::make_injector(
        //di::core::dependency<di::scopes::deduce, i1>{injector1.create<i1*>()}
    //);

    //auto object = injector.create<std::shared_ptr<i1>>();
    //expect(object.get());
};

