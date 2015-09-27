//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include <string>
#include "boost/di.hpp"

namespace di = boost::di;

struct i1 { virtual ~i1() noexcept = default; virtual void dummy1() = 0; };
struct i2 { virtual ~i2() noexcept = default; virtual void dummy2() = 0; };
struct impl1 : i1 { void dummy1() override { } };
struct impl2 : i2 { void dummy2() override { } };

struct complex1 {
    explicit complex1(const std::shared_ptr<i1>& i1)
        : i1_(i1)
    { }

    std::shared_ptr<i1> i1_;
};

struct complex2 {
    int i;
    complex1 c1;
};

struct complex3 {
    complex2 c2;
};

auto name = []{};

test empty_module = [] {
    struct empty {
        auto configure() const {
            return di::make_injector();
        }
    };

    auto injector = di::make_injector(empty{});
    expect(0 == injector.create<int>());
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
                di::bind<i1>().to<impl1>()
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
    expect(i == injector.create<int>());
    expect(d == injector.create<double>());
    expect(f == injector.create<float>());
    expect(s == injector.create<std::string>());
};

test empty_exposed_module = [] {
    struct empty {
        di::injector<> configure() const {
            return di::make_injector();
        }
    };

    auto injector = di::make_injector(empty{});
    expect(0 == injector.create<int>());
};

test exposed_type = [] {
    di::injector<complex1> injector = di::make_injector(
        di::bind<i1>().to<impl1>()
    );

    auto object = injector.create<std::shared_ptr<complex1>>();
    expect(dynamic_cast<i1*>(object->i1_.get()));
};

test exposed_type_by_injector = [] {
    constexpr auto i = 42;

    di::injector<complex1> injector1 = di::make_injector(
        di::bind<i1>().to<impl1>()
    );

    auto injector = di::make_injector(
        injector1
      , di::bind<int>().to(i)
    );

    auto object = injector.create<std::shared_ptr<complex2>>();
    expect(dynamic_cast<i1*>(object->c1.i1_.get()));
    expect(i == object->i);
};

test exposed_type_by_module = [] {
    struct module {
        di::injector<complex1> configure() const {
            return di::make_injector(di::bind<i1>().to<impl1>());
        }
    };

    constexpr auto i = 42;

    auto injector = di::make_injector(
        module{}
      , di::bind<int>().to(i)
    );

    auto object = injector.create<std::shared_ptr<complex2>>();
    expect(dynamic_cast<i1*>(object->c1.i1_.get()));
    expect(i == object->i);
};

test exposed_type_by_module_twice = [] {
    struct module {
        di::injector<complex1> configure() const {
            return di::make_injector(di::bind<i1>().to<impl1>());
        }
    };

    constexpr auto i = 42;

    di::injector<complex2> injector = di::make_injector(
        module{}
      , di::bind<int>().to(i)
    );

    auto object = injector.create<std::shared_ptr<complex2>>();
    expect(dynamic_cast<i1*>(object->c1.i1_.get()));
    expect(i == object->i);
};

test exposed_type_by_module_mix = [] {
    static constexpr auto i = 42;

    struct module1 {
        di::injector<complex1> configure() const {
            return di::make_injector(di::bind<i1>().to<impl1>());
        }
    };

    struct module2 {
        di::injector<complex2> configure() const {
            return di::make_injector(di::bind<int>().to(i), module1{});
        }
    };

    auto injector = di::make_injector(
        module2{}
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
        di::bind<i1>().to<impl1>()
    );

    auto injector = di::make_injector(
        injector1
      , di::bind<int>().to(i)
    );

    {
    auto object = injector.create<std::shared_ptr<complex2>>();
    expect(dynamic_cast<i1*>(object->c1.i1_.get()));
    expect(i == object->i);
    }

    {
    auto object = injector.create<std::shared_ptr<i1>>();
    expect(dynamic_cast<impl1*>(object.get()));
    }
};

test exposed_with_external = [] {
    constexpr auto i = 42;

    di::injector<int> injector = di::make_injector(
        di::bind<int>().to(i)
    );

    expect(i == injector.create<int>());
};

test exposed_bind_deduced = [] {
    static constexpr auto i = 42;

    struct module {
        di::injector<int> configure() const noexcept {
            return di::make_injector(
                di::bind<int>().to(i)
            );
        }
    };

    auto injector = di::make_injector(
        di::bind<int>().to<module>()
    );

    expect(i == injector.create<int>());
};

test exposed_bind = [] {
    static constexpr auto i = 42;

    struct c {
        BOOST_DI_INJECT(c, (named = name) int i)
            : i(i)
        { }

        int i = 0;
    };

    struct module {
        di::injector<int> configure() const noexcept {
            return di::make_injector(
                di::bind<int>().to(i)
            );
        }
    };

    auto injector = di::make_injector(
        di::bind<int>().named(name).in(di::unique).to(module{})
    );

    auto object = injector.create<c>();

    expect(i == object.i);
};

test exposed_bind_interface = [] {
    struct c {
        BOOST_DI_INJECT(c, (named = name) std::unique_ptr<i1> i)
            : i(std::move(i))
        { }

        std::unique_ptr<i1> i;
    };

    di::injector<i1> module = di::make_injector(
        di::bind<i1>().to<impl1>()
    );

    auto injector = di::make_injector(
        di::bind<i1>().named(name).to(module)
    );

    auto object = injector.create<std::unique_ptr<c>>();

    expect(dynamic_cast<impl1*>(object->i.get()));
};

test exposed_bind_interface_module = [] {
    struct c {
        BOOST_DI_INJECT(c, (named = name) std::unique_ptr<i1> i)
            : i(std::move(i))
        { }

        std::unique_ptr<i1> i;
    };

    struct module {
        di::injector<i1> configure() const noexcept {
            return di::make_injector(
                di::bind<i1>().to<impl1>()
            );
        }
    };

    auto injector = di::make_injector(
        di::bind<i1>().named(name).to(module{})
    );

    auto object = injector.create<std::unique_ptr<c>>();

    expect(dynamic_cast<impl1*>(object->i.get()));
};

test exposed_module_with_unique_ptr = [] {
    struct c {
        c(std::unique_ptr<i1> i1
        , std::unique_ptr<i2> i2
        , int i) : i1_(std::move(i1)), i2_(std::move(i2)), i(i)
        { }

        std::unique_ptr<i1> i1_;
        std::unique_ptr<i2> i2_;
        int i = 0;
    };

    struct module {
        di::injector<c> configure() const noexcept {
            return di::make_injector(
                di::bind<i1>().to<impl1>()
              , di::bind<i2>().to<impl2>()
              , di::bind<int>().to(i)
            );
        }

        explicit module(int i)
            : i(i)
        { }

        int i = 0;
    };

    auto injector = di::make_injector(
        module{42}
    );

    auto object = injector.create<std::unique_ptr<c>>();
    expect(dynamic_cast<impl1*>(object->i1_.get()));
    expect(dynamic_cast<impl2*>(object->i2_.get()));
    expect(42 == object->i);
};

