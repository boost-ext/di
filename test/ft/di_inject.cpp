//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di.hpp"

namespace di = boost::di;

auto name = []{};
auto other_name = []{};

struct i1 { virtual ~i1() noexcept = default; virtual void dummy1() = 0; };
struct impl1 : i1 { void dummy1() override { } };

test inject_ctor_limit_size = [] {
    struct c {
        BOOST_DI_INJECT(c, int /*p1*/, int /*p2*/, int /*p3*/, int /*p4*/, int /*p5*/, int /*p6*/, int /*p7*/, int /*p8*/, int /*p9*/, int /*p10*/)
        { }
    };

    auto injector = di::make_injector();
    injector.create<c>();
};

test ctor_limit_size = [] {
    struct c {
        c(int /*p1*/, int /*p2*/, int /*p3*/, int /*p4*/, int /*p5*/, int /*p6*/, int /*p7*/, int /*p8*/, int /*p9*/, int /*p10*/)
        { }
    };

    auto injector = di::make_injector();
    injector.create<c>();
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
        di::bind<int>().to(i)
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
        di::bind<int>().to(i)
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
        di::bind<int>().to(i)
    );

    auto object = injector.create<c>();

    expect_eq(i, object.i);
    expect_eq(0, object.il.size());
};

test ctor_refs = [] {
    struct c {
#if !defined(BOOST_DI_MSVC)
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
#else
        BOOST_DI_INJECT(c
                      , const std::shared_ptr<i1>& sp
                      , int& i
                      , const double& d
                      , const std::string& str
                      , (named = name) const std::string& nstr
                      , long&& l
                      , short s)
            : i(i), d(d), str(str), nstr(nstr), l(std::move(l)), s(s)
#endif
        {
            sp->dummy1();
        }

        int& i;
        const double& d;
        std::string str;
        std::string nstr;
#if !defined(BOOST_DI_MSVC)
        std::function<int()> f;
#endif
        long l = 0;
        short s = 0;
    };

    struct c_inject {
#if !defined(BOOST_DI_MSVC)
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
#else
        BOOST_DI_INJECT(c_inject
                      , const std::shared_ptr<i1>& sp
                      , int& i
                      , const double& d
                      , const std::string& str
                      , (named = name) const std::string& nstr
                      , long&& l
                      , short s)
            : i(i), d(d), str(str), nstr(nstr), l(std::move(l)), s(s)
#endif
        {
            sp->dummy1();
        }

        int& i;
        const double& d;
        std::string str;
        std::string nstr;
#if !defined(BOOST_DI_MSVC)
        std::function<int()> f;
#endif
        long l = 0;
        short s = 0;
    };

    struct c_aggregate {
        const std::shared_ptr<i1>& sp;
        int& i;
        const double& d;
        std::string str = {};
        std::string nstr = {};
#if !defined(BOOST_DI_MSVC)
        std::function<int()> f = {};
#endif
        long l = 0;
        short s = 0;
    };

    auto test = [](auto type, const auto& bind_i1) {
        auto i = 0;
        constexpr auto d = 0.0;

        auto injector = di::make_injector(
            di::bind<int>().to(i)
          , di::bind<double>().to(d)
          , di::bind<std::string>().to("str")
          , di::bind<std::string>().named(name).to("named str")
          , bind_i1
          , di::bind<short>().to(short{42})
          , di::bind<long>().to(123l)
#if !defined(BOOST_DI_MSVC)
          , di::bind<std::function<int()>>().to([]{return 87;})
#endif
        );

        auto object = injector.template create<typename decltype(type)::type>();
        expect_eq(&i, &object.i);
        expect_eq(&d, &object.d);
        expect_eq("str", object.str);
        expect_eq(42, object.s);
#if !defined(BOOST_DI_MSVC)
        expect_eq(87, object.f());
#endif
        expect_eq(123, object.l);
    };

    test(test_type<c>{}, di::bind<i1, impl1>());
    test(test_type<c_inject>{}, di::bind<i1, impl1>());
#if !defined(BOOST_DI_MSVC)
    test(test_type<c_aggregate>{}, di::bind<i1, impl1>());
#endif

    test(test_type<c>{}, di::bind<i1>().to(std::make_shared<impl1>()));
    test(test_type<c_inject>{}, di::bind<i1>().to(std::make_shared<impl1>()));
#if !defined(BOOST_DI_MSVC)
    test(test_type<c_aggregate>{}, di::bind<i1>().to(std::make_shared<impl1>()));
#endif
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
        di::bind<std::string>().named(name).to(ref)
      , di::bind<int>().named(name).to(i)
      , di::bind<int>().named(other_name).to(i)
      , di::bind<int>().to(i)
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
        di::bind<std::string>().named(name).to(ref)
      , di::bind<int>().named(name).to(i)
      , di::bind<int>().named(other_name).to(i)
      , di::bind<int>().to(i)
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
        di::bind<std::string>().named(name).to(ref)
      , di::bind<int>().named(name).to(i)
      , di::bind<int>().named(other_name).to(i)
      , di::bind<int>().to(i)
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
        di::bind<std::string>().named(name).to(ref)
      , di::bind<int>().named(name).to(i)
      , di::bind<int>().named(other_name).to(i)
      , di::bind<int>().to(i)
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
        di::bind<int>().to(i)
    );

    auto object = injector.create<default_values>();

    expect_eq(i, object.i);
    expect_eq(42.0, object.f);
    expect_eq(87.0, object.d);
};

test smart_pointers = [] {
    struct c {
        c(std::unique_ptr<i1> up1, const std::shared_ptr<i1>& sp1, std::shared_ptr<i1> sp2) {
            expect(dynamic_cast<impl1*>(up1.get()));
            expect(dynamic_cast<impl1*>(sp1.get()));
            expect(dynamic_cast<impl1*>(sp2.get()));
            expect_eq(sp1, sp2);
        }
    };

    auto injector = di::make_injector(
        di::bind<i1, impl1>()
    );

    injector.create<c>();
};

test one_arg_class = [] {
    struct c {
        c(int i) : i(i) { }
        int i = 0;
    };

    auto injector = di::make_injector(
        di::bind<int>().in(di::unique)
    );

    auto object = injector.create<c>();
    expect_eq(0, object.i);
};

test string_creation = [] {
    struct string {
        std::string str;
    };

    expect_eq("", di::make_injector().create<string>().str);
};

