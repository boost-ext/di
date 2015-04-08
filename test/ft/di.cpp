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

test create_ptr = [] {
    struct c {
        c(i1* ptr) { delete ptr; }
    };

    auto injector = di::make_injector(
        di::bind<i1, impl1>
    );

    injector.create<c>();
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

static auto called = 0;

class config : public di::config {
public:
    auto policies() const noexcept {
        return di::make_policies([](auto){++called;});
    }
};

test call_policy_lambda = [] {
    called = 0;
    auto injector = di::make_injector<config>();
    expect_eq(0, injector.create<int>());
    expect_eq(1, called);
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

class config_provider : public di::config {
public:
    auto provider() const noexcept {
        ++called;
        return di::providers::heap{};
    }
};

test call_provider = [] {
    called = 0;
    auto injector = di::make_injector<config_provider>();
    injector.create<int>();
    expect_eq(1, called);
};

struct deleter_provider {
    static auto& called() {
        static auto i = 0;
        return i;
    }

    template<class...>
    struct is_creatable {
        static constexpr auto value = true;
    };

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

class config_deleter_provider : public di::config {
public:
    auto provider() const noexcept {
        return deleter_provider{};
    }
};

test call_provider_with_deleter = [] {
    deleter_provider::called() = 0;
    auto injector = di::make_injector<config_deleter_provider>();
    injector.create<int>();
    expect_eq(1, deleter_provider::called());
};

class config_policies : public di::config {
public:
    auto policies() const noexcept {
        using namespace di::policies;
        using namespace di::policies::operators;
        return di::make_policies(constructible(is_root{} || std::is_same<_, double>{} || is_bound<_>{}));
    }
};

test constructible_policy = [] {
    struct example {
        int i = 0;
        double d = 0.0;
    };

    auto injector = di::make_injector<config_policies>(di::bind<int>.to(42));
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

