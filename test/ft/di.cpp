//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#if (__has_include(<boost/shared_ptr.hpp>))
    #include <boost/shared_ptr.hpp>
#endif
#include "boost/di.hpp"
#include "boost/di/providers/heap.hpp"
#include "boost/di/policies/constructible.hpp"

namespace di = boost::di;

auto name = []{};
auto other_name = []{};
auto a = []{};
auto b = []{};

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

struct empty { };

test empty_injector = [] {
    auto injector = di::make_injector();
    expect_eq(0, injector.create<int>());
};

test create_using_copy = [] {
    auto injector = di::make_injector();
    injector.create<empty>();
};

test create_using_ptr = [] {
    auto injector = di::make_injector();
    std::unique_ptr<empty> object{injector.create<empty*>()};
    expect(object.get());
};

test create_using_const_ptr = [] {
    auto injector = di::make_injector();
    std::unique_ptr<const empty> object{injector.create<const empty*>()};
    expect(object.get());
};

test create_using_unique_ptr = [] {
    auto injector = di::make_injector();
    auto object = injector.create<std::unique_ptr<empty>>();
    expect(object.get());
};

test create_using_shared_ptr = [] {
    auto injector = di::make_injector();
    auto object = injector.create<std::shared_ptr<empty>>();
    expect(object.get());
};

test empty_module = [] {
    struct empty {
        auto configure() const {
            return di::make_injector();
        }
    };

    auto injector = di::make_injector(empty{});
    expect_eq(0, injector.create<int>());
};

test create_ptr = [] {
    struct c {
        c(i1* ptr) { delete ptr; }
    };

    auto injector = di::make_injector(
        di::bind<i1, impl1>
    );

    injector.create<c>();
};

test empty_exposed_module = [] {
    struct empty {
        di::injector<> configure() const {
            return di::make_injector();
        }
    };

    auto injector = di::make_injector(empty{});
    expect_eq(0, injector.create<int>());
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

test create_interface_when_impl_with_one_arg_ctor = [] {
    struct impl : i1 { impl(int) { } void dummy1() override { } };

    auto injector = di::make_injector(
        di::bind<i1, impl>
    );

    auto object = injector.create<std::unique_ptr<i1>>();
    expect(object.get());
};

test injectors_mix = [] {
    auto injector = di::make_injector(
        di::make_injector(
            di::bind<i1, impl1>
        )
    );

    auto object = injector.create<std::shared_ptr<complex1>>();
    expect(object->i1_.get());
};

test exposed_type = [] {
    di::injector<complex1> injector = di::make_injector(
        di::bind<i1, impl1>
    );

    auto object = injector.create<std::shared_ptr<complex1>>();
    expect(dynamic_cast<i1*>(object->i1_.get()));
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
    expect(dynamic_cast<i1*>(object->c1.i1_.get()));
    expect_eq(i, object->i);
};

test exposed_type_by_module = [] {
    struct module {
        di::injector<complex1> configure() const {
            return di::make_injector(di::bind<i1, impl1>);
        }
    };

    constexpr auto i = 42;

    auto injector = di::make_injector(
        module{}
      , di::bind<int>.to(i)
    );

    auto object = injector.create<std::shared_ptr<complex2>>();
    expect(dynamic_cast<i1*>(object->c1.i1_.get()));
    expect_eq(i, object->i);
};

test exposed_type_by_module_twice = [] {
    struct module {
        di::injector<complex1> configure() const {
            return di::make_injector(di::bind<i1, impl1>);
        }
    };

    constexpr auto i = 42;

    di::injector<complex2> injector = di::make_injector(
        module{}
      , di::bind<int>.to(i)
    );

    auto object = injector.create<std::shared_ptr<complex2>>();
    expect(dynamic_cast<i1*>(object->c1.i1_.get()));
    expect_eq(i, object->i);
};

test exposed_type_by_module_mix = [] {
    static constexpr auto i = 42;

    struct module1 {
        di::injector<complex1> configure() const {
            return di::make_injector(di::bind<i1, impl1>);
        }
    };

    struct module2 {
        di::injector<complex2> configure() const {
            return di::make_injector(di::bind<int>.to(i), module1{});
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
        di::bind<i1, impl1>
    );

    auto injector = di::make_injector(
        injector1
      , di::bind<int>.to(i)
    );

    {
    auto object = injector.create<std::shared_ptr<complex2>>();
    expect(dynamic_cast<i1*>(object->c1.i1_.get()));
    expect_eq(i, object->i);
    }

    {
    auto object = injector.create<std::shared_ptr<i1>>();
    expect(dynamic_cast<impl1*>(object.get()));
    }
};

test exposed_with_external = [] {
    constexpr auto i = 42;

    di::injector<int> injector = di::make_injector(
        di::bind<int>.to(i)
    );

    expect_eq(i, injector.create<int>());
};

test exposed_bind_deduced = [] {
    static constexpr auto i = 42;

    struct module {
        di::injector<int> configure() const noexcept {
            return di::make_injector(
                di::bind<int>.to(i)
            );
        }
    };

    auto injector = di::make_injector(
        di::bind<int>.to(module{})
    );

    expect_eq(i, injector.create<int>());
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
                di::bind<int>.to(i)
            );
        }
    };

    auto injector = di::make_injector(
        di::bind<int>.named(name).in(di::unique).to(module{})
    );

    auto object = injector.create<c>();

    expect_eq(i, object.i);
};

test exposed_bind_interface = [] {
    struct c {
        BOOST_DI_INJECT(c, (named = name) std::unique_ptr<i1> i)
            : i(std::move(i))
        { }

        std::unique_ptr<i1> i;
    };

    di::injector<i1> module = di::make_injector(
        di::bind<i1, impl1>
    );

    auto injector = di::make_injector(
        di::bind<i1>.named(name).to(module)
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
                di::bind<i1, impl1>
            );
        }
    };

    auto injector = di::make_injector(
        di::bind<i1>.named(name).to(module{})
    );

    auto object = injector.create<std::unique_ptr<c>>();

    expect(dynamic_cast<impl1*>(object->i.get()));
};

test exposed_module_with_unique_ptr = [] {
    struct i1 { virtual ~i1() noexcept = default; virtual void dummy1() = 0; };
    struct i2 { virtual ~i2() noexcept = default; virtual void dummy2() = 0; };
    struct impl1 : i1 { void dummy1() override { } };
    struct impl2 : i2 { void dummy2() override { } };

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
                di::bind<i1, impl1>
              , di::bind<i2, impl2>
              , di::bind<int>.to(i)
            );
        }

        int i = 0;
    };

    auto injector = di::make_injector(
        module{42}
    );

    auto object = injector.create<std::unique_ptr<c>>();
    assert(dynamic_cast<impl1*>(object->i1_.get()));
    assert(dynamic_cast<impl2*>(object->i2_.get()));
    assert(42 == object->i);
};

test scopes_priority = [] {
    auto injector = di::make_injector(
        di::bind<int>.to(12)
      , di::bind<int, std::integral_constant<int, 42>>
    );

    auto object = injector.create<int>();

    expect_eq(12, object);
};

test bind_int_to_static_value = [] {
    auto injector = di::make_injector(
        di::bind<int, std::integral_constant<int, 42>>
    );

    auto object = injector.create<int>();

    expect_eq(42, object);
};

test scopes_order = [] {
    auto injector = di::make_injector(
        di::bind<int, std::integral_constant<int, 41>>
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
    auto injector = di::make_injector(
        di::bind<i1, impl1>.in(di::session(name))
    );

    {
    injector.call(di::session_entry(name));
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
    injector.call(di::session_exit(name));
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

#if (__has_include(<boost/shared_ptr.hpp>))
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
#endif

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

    auto object = injector.create<c>();
    delete object.ptr;
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
        di::bind<int>.named(name).to(i)
    );

    auto object = injector.create<c>();

    expect_eq(i, object.i);
};

test bind_chars_to_string = [] {
    auto injector = di::make_injector(
        di::bind<std::string>.to("str")
    );

    expect_eq("str", injector.create<std::string>());
};

test ctor_refs = [] {
    struct c {
        BOOST_DI_INJECT(c
        , const std::shared_ptr<i1>& sp
        , int& i
        , const double& d
        , const std::string& str
        , (named = name) const std::string& nstr
        , const std::function<int()>& f
        , long&& l
        , short s)
            : i(i), d(d), str(str), nstr(nstr), f(f), l(std::move(l)), s(s)
        {
            sp->dummy1();
        }

        int& i;
        const double& d;
        std::string str;
        std::string nstr;
        std::function<int()> f;
        long l = 0;
        short s = 0;
    };

    struct c_inject {
        BOOST_DI_INJECT(c_inject
                      , const std::shared_ptr<i1>& sp
                      , int& i
                      , const double& d
                      , const std::string& str
                      , (named = name) const std::string& nstr
                      , std::function<int()> f
                      , long&& l
                      , short s)
            : i(i), d(d), str(str), nstr(nstr), f(f), l(std::move(l)), s(s)
        {
            sp->dummy1();
        }

        int& i;
        const double& d;
        std::string str;
        std::string nstr;
        std::function<int()> f;
        long l = 0;
        short s = 0;
    };

    struct c_aggregate {
        const std::shared_ptr<i1>& sp;
        int& i;
        const double& d;
        std::string str;
        std::string nstr;
        std::function<int()> f;
        long l = 0;
        short s = 0;
    };

    auto test = [](auto type, const auto& bind_i1) {
        auto i = 0;
        constexpr auto d = 0.0;

        auto injector = di::make_injector(
            di::bind<int>.to(std::ref(i))
          , di::bind<double>.to(std::cref(d))
          , di::bind<std::string>.to("str")
          , di::bind<std::string>.named(name).to("named str")
          , bind_i1
          , di::bind<short>.to(short{42})
          , di::bind<long>.to(123)
          , di::bind<std::function<int()>>.to([]{return 87;})
        );

        auto object = injector.template create<typename decltype(type)::type>();
        expect_eq(&i, &object.i);
        expect_eq(&d, &object.d);
        expect_eq("str", object.str);
        expect_eq(42, object.s);
        expect_eq(87, object.f());
        expect_eq(123, object.l);
    };

    test(test_type<c>{}, di::bind<i1, impl1>);
    test(test_type<c_inject>{}, di::bind<i1, impl1>);
    test(test_type<c_aggregate>{}, di::bind<i1, impl1>);

    test(test_type<c>{}, di::bind<i1>.to(std::make_shared<impl1>()));
    test(test_type<c_inject>{}, di::bind<i1>.to(std::make_shared<impl1>()));
    test(test_type<c_aggregate>{}, di::bind<i1>.to(std::make_shared<impl1>()));
};

test refs_vs_copy = [] {
    struct cc {
        BOOST_DI_INJECT(cc
         , (named = name) const int& i
         , (named = name) const std::string& s
         , (named = other_name) int& i_ref
         , int& ii)
            : str(s), i(i), i_ref(i_ref), ii(ii)
        { }

        std::string str;
        int i = 0;
        int& i_ref;
        int& ii;
    };

    struct cc_inject {
        BOOST_DI_INJECT(cc_inject
                      , (named = name) const int& i
                      , (named = name) const std::string& s
                      , (named = other_name) int& i_ref
                      , int& ii)
            : str(s), i(i), i_ref(i_ref), ii(ii)
        { }

        std::string str;
        int i = 0;
        int& i_ref;
        int& ii;
    };

    std::string ref = "named str";
    auto i = 42;

    {
    auto injector = di::make_injector(
        di::bind<std::string>.named(name).to(std::ref(ref))
      , di::bind<int>.named(name).to(std::cref(i))
      , di::bind<int>.named(other_name).to(std::ref(i))
      , di::bind<int>.to(std::ref(i))
    );
    auto object = injector.create<cc>();
    expect_eq(ref, object.str);
    expect_eq(i, object.i);
    expect_eq(i, object.i_ref);
    expect_eq(&i, &object.i_ref);
    expect_eq(i, object.ii);
    expect_eq(&i, &object.ii);
    }

    {
    auto injector = di::make_injector(
        di::bind<std::string>.named(name).to(std::ref(ref))
      , di::bind<int>.named(name).to(std::cref(i))
      , di::bind<int>.named(other_name).to(std::ref(i))
      , di::bind<int>.to(std::ref(i))
    );
    auto object = injector.create<cc_inject>();
    expect_eq(ref, object.str);
    expect_eq(i, object.i);
    expect_eq(i, object.i_ref);
    expect_eq(&i, &object.i_ref);
    expect_eq(i, object.ii);
    expect_eq(&i, &object.ii);
    }

    {
    auto injector = di::make_injector(
        di::bind<std::string>.named(name).to(ref)
      , di::bind<int>.named(name).to(i)
      , di::bind<int>.named(other_name).to(std::ref(i))
      , di::bind<int>.to(std::ref(i))
    );
    auto object = injector.create<cc>();
    expect_eq(ref, object.str);
    expect_eq(i, object.i);
    expect_eq(i, object.i_ref);
    expect_eq(&i, &object.i_ref);
    expect_eq(i, object.ii);
    expect_eq(&i, &object.ii);
    }

    {
    auto injector = di::make_injector(
        di::bind<std::string>.named(name).to(ref)
      , di::bind<int>.named(name).to(i)
      , di::bind<int>.named(other_name).to(std::ref(i))
      , di::bind<int>.to(std::ref(i))
    );
    auto object = injector.create<cc_inject>();
    expect_eq(ref, object.str);
    expect_eq(i, object.i);
    expect_eq(i, object.i_ref);
    expect_eq(&i, &object.i_ref);
    expect_eq(i, object.ii);
    expect_eq(&i, &object.ii);
    }
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

static auto called = false;
test call_policy_lambda = [] {
    class config : public di::config {
    public:
        auto policies() const noexcept {
            return di::make_policies([](auto){called = true;});
        }
    };

    auto injector = di::make_injector<config>();

    expect_eq(0, injector.create<int>());
    expect(called);
};

test modules_mix_make_injector = [] {
    constexpr auto i = 42;
    constexpr auto d = 87.0;
    constexpr auto f = 123.0f;
    const std::string s = "string";

    auto injector_string = make_injector(
        di::bind<std::string>.to(s)
    );

    struct empty {
        auto configure() const {
            return di::make_injector();
        }
    };

    struct module1 {
        di::injector<i1> configure() const {
            return di::make_injector(
                di::bind<i1, impl1>
            );
        }
    };

    struct module2 {
        di::injector<int> configure() const {
            return di::make_injector(
                di::bind<int>.to(i_)
            );
        }

        int i_ = 0;
    };

    auto injector = di::make_injector(
        empty{}
      , di::bind<double>.to(d)
      , module1{}
      , make_injector(
            di::bind<float>.to(f)
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

test runtime_factory_impl = [] {
    constexpr auto i = 42;

    auto test = [&](bool debug_property) {
        auto injector = make_injector(
            di::bind<int>.to(i)
          , di::bind<i1>.to([&](const auto& injector) -> std::shared_ptr<i1> {
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
            di::bind<int>.to(i)
          , di::bind<i1>.to(call_operator{debug_property})
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

test dynamic_binding_using_polymorphic_lambdas_with_dependend_interfaces = [] {
    auto test = [&](bool debug_property) {
        auto injector = make_injector(
            di::bind<i1>.to([&](const auto& injector) -> std::shared_ptr<i1> {
                if (debug_property) {
                    return std::make_shared<impl1>();
                }

                return injector.template create<std::shared_ptr<impl1_with_i2>>();
            })
          , di::bind<i2, impl2>
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

test create_with_default_values = [] {
    constexpr auto i = 42;

    struct default_values {
        BOOST_DI_INJECT_TRAITS(int);

        default_values(int i, float f = 42.0, double d = 87.0)
            : i(i), f(f), d(d)
        { }

        int i = 0;
        float f = 0.0;
        double d = 0.0;
    };

    auto injector = make_injector(
        di::bind<int>.to(i)
    );

    auto object = injector.create<default_values>();

    expect_eq(i, object.i);
    expect_eq(42.0, object.f);
    expect_eq(87.0, object.d);
};

test inject_traits_no_limits = [] {
    struct c {
        BOOST_DI_INJECT_TRAITS_NO_LIMITS(int, int, int, int, int, int, int, int, int, int, int);
        c(int, int, int, int, int, int, int, int, int, int, int) { }
    };

    auto injector = di::make_injector();
    injector.create<c>();
};

struct c_no_limits {
    c_no_limits(int, int, int, int, int, int, int, int, int, int, int) { }
};

namespace boost { namespace di {

template<>
struct ctor_traits<c_no_limits> {
    BOOST_DI_INJECT_TRAITS_NO_LIMITS(int, int, int, int, int, int, int, int, int, int, int);
};

}} // boost::di

test inject_traits_no_limits_via_ctor_traits = [] {
    auto injector = di::make_injector();
    injector.create<c_no_limits>();
};

test call_provider = [] {
    static auto called = 0;
    class config : public di::config {
    public:
        auto provider() const noexcept {
            ++called;
            return di::providers::heap{};
        }
    };

    auto injector = di::make_injector<config>();
    injector.create<int>();
    expect_eq(1, called);
};

struct deleter_provider {
    static auto& called() {
        static auto i = 0;
        return i;
    }

    template<class I, class T, class TInitialization, class TMemory, class... TArgs>
    auto get(const TInitialization& // direct/uniform
           , const TMemory& // stack/heap
           , TArgs&&... args) const {
            ++called();
        return std::unique_ptr<T, std::default_delete<I>>{
            new T(std::forward<TArgs>(args)...)
        };
    }
};

test call_provider_with_deleter = [] {
    class config : public di::config {
    public:
        auto provider() const noexcept {
            return deleter_provider{};
        }
    };

    deleter_provider::called() = 0;
    auto injector = di::make_injector<config>();
    injector.create<int>();
    expect_eq(1, deleter_provider::called());
};

test constructible_policy = [] {
    class config : public di::config {
    public:
        auto policies() const noexcept {
            using namespace di::policies;
            using namespace di::policies::operators;
            return di::make_policies(constructible(is_root{} || std::is_same<_, double>{} || is_bound<_>{}));
        }
    };

    struct example {
        int i = 0;
        double d = 0.0;
    };

    auto injector = di::make_injector<config>(di::bind<int>.to(42));
    injector.create<example>();
};

struct policy {
    static auto& called() {
        static auto i = 0;
        return i;
    }

    template<class T>
    void operator()(const T&) const noexcept {
        ++called();
    }
};

class custom_policies : public di::config {
public:
    auto policies() const noexcept {
        return di::make_policies(
            policy{}
          , [](auto) { ++policy::called(); }
          , [](auto type, auto dependency, auto... ctor) { ++policy::called(); }
        );
    }
};

test call_custom_policies = [] {
    policy::called() = 0;
    auto injector = di::make_injector<custom_policies>();
    injector.create<int>();
    expect_eq(3, policy::called());
};

test call_custom_policies_with_exposed_injector = [] {
    policy::called() = 0;

    di::injector<i1> injector = di::make_injector<custom_policies>(
        di::bind<i1, impl1>
    );

    auto object = injector.create<std::unique_ptr<i1>>();
    expect(dynamic_cast<i1*>(object.get()));
    expect_eq(3, policy::called());
};

