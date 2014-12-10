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

test exposed_with_external = [] {
    constexpr auto i = 42;

    di::injector<int> injector = di::make_injector(
        di::bind<int>.to(i)
    );

    expect_eq(i, injector.create<int>());
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
        c(const std::shared_ptr<i1>& sp
        , int& i
        , const double& d
        , const std::string& str
        , di::named<const std::string&, name> nstr
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

    struct c_inject {
        BOOST_DI_INJECT(c_inject
                      , const std::shared_ptr<i1>& sp
                      , int& i
                      , const double& d
                      , const std::string& str
                      , di::named<const std::string&, name> nstr
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
        std::string str; // possible ref to copy with const std::string&
        di::named<std::string, name> nstr; // possible ref to copy with const std::string&
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
          , di::bind<std::string>.named(name{}).to("named str")
          , bind_i1
          , di::bind<short>.to(42)
          , di::bind<long>.to(123)
          , di::bind<std::function<int()>>.to([]{return 87;})
        );

        auto object = injector.template create<typename decltype(type)::type>();
        expect_eq(&i, &object.i);
        expect_eq(&d, &object.d);
        expect_eq("str", object.str);
        expect_eq("named str", static_cast<const std::string&>(object.nstr));
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

test named_parameters_with_shared_scope = [] {
    struct a { };
    struct b { };

    struct c {
        c(di::named<std::shared_ptr<i1>, a> n1, di::named<std::shared_ptr<i1>, b> n2)
            : n1(n1), n2(n2)
        { }

        std::shared_ptr<i1> n1;
        std::shared_ptr<i1> n2;
    };

    auto injector = di::make_injector(
        di::bind<i1, impl1>.named(a{}).in(di::unique)
      , di::bind<i1>.named(b{}).to(std::make_shared<impl1>())
    );

    auto object = injector.create<c>();

    expect(object.n1 != object.n2);
    expect(object.n1 != static_cast<std::shared_ptr<i1>>(injector.create<di::named<std::shared_ptr<i1>, a>>()));
    expect(object.n2 == static_cast<std::shared_ptr<i1>>(injector.create<di::named<std::shared_ptr<i1>, b>>()));
};


//struct empty_module {
    //auto configure() const {
        //return di::make_injector();
    //}
//};

//struct module1 {
    //di::injector<c0if0> configure() const {
        //return di::make_injector();
    //}
//};

//struct module2 {
    //using injector_t = injector<decltype(bind<int>::to(int()))>;

    //explicit module2(int i)
        //: i_(i)
    //{ }

    //injector_t configure() const {
        //return injector_t(bind<int>::to(i_));
    //}

    //int i_ = 0;
//};

//test = ctor [] {
    //using injector_c0 = injector<
        //c0if0
    //>;

    //aux::shared_ptr<c5> c5_ = injector_c0().create<aux::shared_ptr<c5>>();
    //BOOST_CHECK_EQUAL(0, c5_->c2_->i);
//}

//BOOST_AUTO_TEST_CASE(empty) {
    //auto injector_empty = make_injector();
    //BOOST_CHECK_EQUAL(0, injector_empty.create<c3>().i);
//}

//BOOST_AUTO_TEST_CASE(by_value) {
    //const int i = 42;
    //using injector_c0 = injector<
        //c0if0
    //>;

    //auto injector_c0_int_ = make_injector(
        //injector_c0()
      //, bind<int>::to(i)
    //);

    //aux::shared_ptr<c5> c5_ = injector_c0_int_.create<aux::shared_ptr<c5>>();
    //BOOST_CHECK_EQUAL(i, c5_->c2_->i);
//}

//BOOST_AUTO_TEST_CASE(runtime_factory_impl) {
    //const auto debug_property = false;
    //const auto value = 42;

    //auto common = make_injector(
        //bind<int>::to(value)
    //);

    //auto all = make_injector(
        //common
      //, bind<i>::to([&]() -> aux::shared_ptr<i> {
            //if (debug_property) {
                //return aux::shared_ptr<i>(new fake());
            //}

            //return common.create<aux::shared_ptr<impl>>();
        //})
    //);

    //auto i_ = all.create<aux::shared_ptr<i>>();

    //BOOST_CHECK(dynamic_cast<impl*>(i_.get()));
    //BOOST_CHECK_EQUAL(value, dynamic_cast<impl*>(i_.get())->i);
//}

//BOOST_AUTO_TEST_CASE(runtime_factory_fake) {
    //const auto debug_property = true;
    //const auto value = 42;

    //auto common = make_injector(
        //bind<int>::to(value)
    //);

    //auto all = make_injector(
        //common
      //, bind<i>::to([&]() -> aux::shared_ptr<i> {
            //if (debug_property) {
                //return aux::shared_ptr<i>(new fake());
            //}

            //return common.create<aux::shared_ptr<impl>>();
        //})
    //);

    //auto i_ = all.create<aux::shared_ptr<i>>();

    //BOOST_CHECK(dynamic_cast<fake*>(i_.get()));
//}

//BOOST_AUTO_TEST_CASE(modules_injector) {
    //const int i = 42;
    //injector<module1, module2> injector_{module2(i)};

    //BOOST_CHECK(dynamic_cast<c0if0*>(injector_.create<aux::auto_ptr<if0>>().get()));
    //BOOST_CHECK_EQUAL(i, injector_.create<int>());
//}

//BOOST_AUTO_TEST_CASE(modules_make_injector) {
    //const int i = 42;
    //auto injector_ = make_injector(module1(), module2(i));

    //BOOST_CHECK(dynamic_cast<c0if0*>(injector_.create<aux::auto_ptr<if0>>().get()));
    //BOOST_CHECK_EQUAL(i, injector_.create<int>());
//}

//BOOST_AUTO_TEST_CASE(modules_mix_make_injector) {
    //const int i = 42;
    //const double d = 87.0;
    //const float f = 123.0;
    //const std::string s = "string";

    //auto injector_string = make_injector(
        //bind<std::string>::to(s)
    //);

    //auto injector_ = make_injector(
        //empty_module()
      //, bind<double>::to(d)
      //, module1()
      //, make_injector(
            //bind<float>::to(f)
        //)
      //, injector_string
      //, module2(i)
    //);

    //BOOST_CHECK(dynamic_cast<c0if0*>(injector_.create<aux::auto_ptr<if0>>().get()));
    //BOOST_CHECK_EQUAL(i, injector_.create<int>());
    //BOOST_CHECK_EQUAL(d, injector_.create<double>());
    //BOOST_CHECK_EQUAL(f, injector_.create<float>());
    //BOOST_CHECK_EQUAL(s, injector_.create<std::string>());
//}

//BOOST_AUTO_TEST_CASE(wrappers_types_mix) {
    //const int i1 = 42;
    //const int i2 = 43;
    //const float f1 = 123.0;
    //const float f2 = 124.0;
    //const double d1 = 87.0;
    //const double d2 = 88.0;
    //const short s1 = 1;
    //const short s2 = 2;
    //const char ch1 = '0';
    //const char ch2 = '1';
    //const long l = 77;

    //auto injector_ = make_injector(
        //bind_int<i1>()
      //, bind_int<i2>::named<a>()
      //, bind<float>::to(f1)
      //, bind<float>::named<b>::to(f2)
      //, bind<double>::to(d1)
      //, bind<double>::named<c>::to(d2)
      //, bind<short>::to(s1)
      //, bind<short>::named<d>::to(s2)
      //, bind<char>::to(ch1)
      //, bind<char>::named<e>::to(ch2)
      //, bind<long>::to(l)
    //);

    //auto wrappers_ = injector_.create<aux::unique_ptr<wrappers_types>>();

    //BOOST_CHECK_EQUAL(i1, wrappers_->i_);
    //BOOST_CHECK_EQUAL(i2, wrappers_->ii_);
    //BOOST_CHECK_EQUAL(f1, *wrappers_->f_);
    //BOOST_CHECK_EQUAL(f2, *wrappers_->ff_);
    //BOOST_CHECK_EQUAL(d1, *wrappers_->d_);
    //BOOST_CHECK_EQUAL(d2, *wrappers_->dd_);
    //BOOST_CHECK_EQUAL(s1, *wrappers_->s_);
    //BOOST_CHECK_EQUAL(s2, *wrappers_->ss_);
    //BOOST_CHECK_EQUAL(ch1, wrappers_->c_);
    //BOOST_CHECK_EQUAL(ch2, wrappers_->cc_);
    //BOOST_CHECK_EQUAL(l, wrappers_->l_);
    //BOOST_CHECK_EQUAL(l, wrappers_->ll_);
//}

//BOOST_AUTO_TEST_CASE(externals_ref_cref) {
    //int i = 42;
    //const double d = 87.0;
    //short s = 0;
    //const char c = 'a';

    //auto injector = make_injector(
        //bind<int>::to(boost::ref(i))
      //, bind<double>::to(boost::cref(d))
      //, bind<short>::to(std::ref(s))
      //, bind<char>::to(std::cref(c))
    //);

    //auto refs_ = injector.create<refs>();

    //BOOST_CHECK_EQUAL(i, refs_.i_);
    //BOOST_CHECK_EQUAL(d, refs_.d_);
    //BOOST_CHECK_EQUAL(s, refs_.s_);
    //BOOST_CHECK_EQUAL(c, refs_.c_);
//}

//namespace {

//double return_double(double d) { return d; }
//long return_long(long l) { return l; }

//} // namespace

//BOOST_AUTO_TEST_CASE(bind_to_function_ptr) {
    //const int i = 42;
    //const double d = 87.0;
    //const long l = 0;

    //auto injector = make_injector(
        //bind<function<int()>>::to([&]{ return i; })
      //, bind<function<double()>>::to(boost::bind(&return_double, d))
      //, bind<function<long()>>::to(std::bind(&return_long, l))
    //);

    //auto functions_ = injector.create<functions>();

    //BOOST_CHECK_EQUAL(i, functions_.fi_());
    //BOOST_CHECK_EQUAL(d, functions_.fd_());
    //BOOST_CHECK_EQUAL(l, functions_.fl_());
//}

//BOOST_AUTO_TEST_CASE(named_parameters_with_unique_scope) {
    //auto injector = make_injector(
        //unique<impl>()
      //, bind<i>::named<b>::to(aux::shared_ptr<impl>(new impl()))
    //);

    //auto nameds_ = injector.create<nameds>();
    //BOOST_CHECK(nameds_.n1_ != nameds_.n2_);

    //BOOST_CHECK(nameds_.n1_ != injector.create<aux::shared_ptr<i>>());
    //BOOST_CHECK(nameds_.n2_ != injector.create<aux::shared_ptr<i>>());
//}

//BOOST_AUTO_TEST_CASE(create_with_default_values) {
    //const int i = 42;

    //auto injector = make_injector(
        //di::bind_int<i>()
    //);

    //auto default_values_ = injector.create<default_values>();

    //BOOST_CHECK_EQUAL(i, default_values_.i_);
    //BOOST_CHECK_EQUAL(42.0, default_values_.f_);
    //BOOST_CHECK_EQUAL(87.0, default_values_.d_);
//}

//BOOST_AUTO_TEST_CASE(dynamic_binding_using_polymorphic_lambdas) {
    //auto b = false;
    //auto injector = di::make_injector(
       //di::bind<int>::to(42)
     //, di::bind<if0>::to([&](auto& injector) -> aux::shared_ptr<if0> {
          //if (b) {
            //return injector.template create<aux::shared_ptr<c0if0>>();
          //}

          //return injector.template create<aux::shared_ptr<c3if0>>();
       //})
    //);

    //{
    //auto object = injector.create<aux::shared_ptr<if0>>();
    //BOOST_CHECK(dynamic_cast<c3if0*>(object.get()));
    //BOOST_CHECK_EQUAL(42, dynamic_cast<c3if0*>(object.get())->i);
    //}

    //b = true;
    //{
    //auto object = injector.create<aux::shared_ptr<if0>>();
    //BOOST_CHECK(dynamic_cast<c0if0*>(object.get()));
    //}
//}

//BOOST_AUTO_TEST_CASE(dynamic_binding_using_polymorphic_lambdas_with_interfaces) {
    //auto b = false;
    //auto module = di::make_injector(
        //di::bind<if0>::to([&](auto& injector) -> aux::shared_ptr<if0> {
            //BOOST_CHECK(dynamic_cast<cif0if1*>(injector.template create<aux::unique_ptr<if1>>().get()));

            //if (b) {
              //return injector.template create<aux::shared_ptr<c0if0>>();
            //}

            //return injector.template create<aux::shared_ptr<c3if0>>();
        //})
    //);

    //auto injector = di::make_injector(
        //module
      //, di::bind<if1, cif0if1>{}
      //, di::bind<int>::to(42)
    //);

    //{
    //auto object = injector.create<aux::shared_ptr<if0>>();
    //BOOST_CHECK(dynamic_cast<c3if0*>(object.get()));
    //BOOST_CHECK_EQUAL(42, dynamic_cast<c3if0*>(object.get())->i);
    //}

    //b = true;
    //{
    //auto object = injector.create<aux::shared_ptr<if0>>();
    //BOOST_CHECK(dynamic_cast<c0if0*>(object.get()));
    //}
//}

//BOOST_AUTO_TEST_CASE(dynamic_binding_using_polymorphic_lambdas_with_dependend_interfaces) {
    //auto b = false;
    //auto module1 = di::make_injector(
        //di::bind<if0>::to([&](auto& injector) -> aux::shared_ptr<if0> {
            //if (b) {
              //return injector.template create<aux::shared_ptr<c3if0>>();
            //}

            //return injector.template create<aux::shared_ptr<c4if0>>();
        //})
    //);

    //auto module2 = di::make_injector(
        //di::bind<if1>::to([&](auto& injector) -> aux::shared_ptr<if1> {
            //if (b) {
              //return injector.template create<aux::shared_ptr<c1if1>>();
            //}

            //return injector.template create<aux::shared_ptr<cif0if1>>();
        //})
    //);

    //auto injector = di::make_injector(
        //module1
      //, module2
    //);

    //{
    //auto object1 = dynamic_cast<c4if0*>(injector.create<aux::shared_ptr<if0>>().get());
    //auto object2 = dynamic_cast<cif0if1*>(injector.create<aux::shared_ptr<if1>>().get());
    //BOOST_CHECK(object1 && object2);
    //BOOST_CHECK_EQUAL(dynamic_cast<cif0if1*>(object1->if1_.get()), object2);
    //}

    //b = true;
    //{
    //BOOST_CHECK(dynamic_cast<c3if0*>(injector.create<aux::shared_ptr<if0>>().get()));
    //BOOST_CHECK(dynamic_cast<c1if1*>(injector.create<aux::shared_ptr<if1>>().get()));
    //}
//}

//struct call_operator {
    //bool& b;

    //template<typename TInjector>
    //aux::shared_ptr<if0> operator()(TInjector& injector) const {
        //if (b) {
            //return injector.template create<aux::shared_ptr<c0if0>>();
        //}

        //return injector.template create<aux::shared_ptr<c3if0>>();
    //}
//};

//BOOST_AUTO_TEST_CASE(dynamic_binding_using_template_call_operator) {
    //auto b = false;
    //auto injector = di::make_injector(
       //di::bind<int>::to(42)
     //, di::bind<if0>::to(call_operator{b})
    //);

    //{
    //auto object = injector.create<aux::shared_ptr<if0>>();
    //BOOST_CHECK(dynamic_cast<c3if0*>(object.get()));
    //BOOST_CHECK_EQUAL(42, dynamic_cast<c3if0*>(object.get())->i);
    //}

    //b = true;
    //{
    //auto object = injector.create<aux::shared_ptr<if0>>();
    //BOOST_CHECK(dynamic_cast<c0if0*>(object.get()));
    //}
//}

//namespace boost {
//namespace di {

//using injector_1_t = injector<
    //shared<
        //c3
    //>
  //, unique<
        //c0if0
      //, bind<c1if0>::when<call_stack<c6, c5>>
      //, bind<c2if0>::when<call_stack<c7>>
      //, bind_int<1>
      //, bind_int<2>::when<call_stack<c8>>
      //, bind_int<3>::named<mpl::string<'1'>>::when<call_stack<c7, c6, c4>>
      //, bind_int<4>::named<mpl::string<'2'>>::when<call_stack<c7, c6, c4>>
      //, bind_int<5>::when<call_stack<c2>>
      //, bind_bool<true>
    //>
//>;

//using injector_2_t = injector<
    //shared<
        //c3
    //>
  //, scope<scopes::unique<>>::bind<
        //bind_int<0>::named<mpl::string<'1'>>
      //, bind_int<1>
      //, bind_bool<true>
    //>
//>;

//using injector_3_t = injector<
    //shared<
        //c0if0
    //>
  //, unique<
        //bind_int<2>::when<call_stack<c8>>
      //, bind_int<3>::named<mpl::string<'2'>>
    //>
//>;

//using injector_custom_scope_t = injector<
    //scope<fake_scope<>>::bind<
        //c3
      //, int
    //>
//>;

//using injector_provider_t = injector<
    //unique<
        //transaction_provider
    //>
//>;

//using injector_c0if0_t = injector<
    //c0if0
//>;

//auto injector_1 = make_injector(
    //shared<
        //c3
    //>()
  //, unique<
        //c0if0
      //, bind<c1if0>::when<call_stack<c6, c5>>
      //, bind<c2if0>::when<call_stack<c7>>
      //, bind_int<1>
      //, bind_int<2>::when<call_stack<c8>>
      //, bind_int<3>::named<mpl::string<'1'>>::when<call_stack<c7, c6, c4>>
      //, bind_int<4>::named<mpl::string<'2'>>::when<call_stack<c7, c6, c4>>
      //, bind_int<5>::when<call_stack<c2>>
      //, bind_bool<true>
    //>()
//);

//auto injector_2 = make_injector(
    //shared<
        //c0if0
    //>()
  //, unique<
        //bind_int<2>::when<call_stack<c8>>
      //, bind_int<3>::named<mpl::string<'2'>>
    //>()
//);

//auto injector_3 = make_injector(
    //shared<
        //c3
    //>()
  //, unique<
        //bind_int<0>::named<mpl::string<'1'>>
      //, bind_int<1>
      //, bind_bool<true>
    //>()
//);

//auto injector_custom_scope = make_injector(
    //scope<fake_scope<>>::bind<c3, int>()
//);

//auto injector_provider = make_injector(
    //unique<
        //transaction_provider, mpl::int_<0>
    //>()
//);

//auto injector_externals = make_injector(
    //bind<double>::to(7.0)
  //, bind<if0>::to(aux::shared_ptr<c3if0>(new c3if0(67, 78.0)))
  //, bind<int>::named<mpl::string<'1'>>::when<call_stack<c7, c6, c4>>::to(3)
  //, bind<int>::when<call_stack<c8>>::to(4)
//);

//auto injector_externals_1 = make_injector(
    //bind<if0>::to(aux::shared_ptr<c3if0>(new c3if0(67, 78.0)))
  //, bind<int>::named<mpl::string<'1'>>::when<call_stack<c7, c6, c4>>::to(3)
  //, bind<int>::when<call_stack<c8>>::to(4)
//);

//auto injector_externals_2 = make_injector(
    //shared<
        //c0if0
    //>()
  //, bind<int>::to(42)
  //, bind<double>::to(87.0)
//);

//auto injector_c0if0 = make_injector(
    //deduce<c0if0>()
//);

//void check(const c8& c8_) {
    //BOOST_CHECK(c8_.c1_ == c8_.c7_->c6_->c5_.c1_);
    //BOOST_CHECK(c8_.c7_->c6_->c4_->c3_ == c8_.c7_->c6_->c3_);
    //BOOST_CHECK(c8_.c7_->if0_ != c8_.c7_->c6_->c5_.if0_);

    //BOOST_CHECK(dynamic_cast<c1if0*>(c8_.c7_->c6_->c5_.if0_.get()));
    //BOOST_CHECK(dynamic_cast<c2if0*>(c8_.c7_->if0_.get()));

    //BOOST_CHECK_EQUAL(2, c8_.i);
    //BOOST_CHECK_EQUAL(3, c8_.c7_->c6_->c4_->i1);
    //BOOST_CHECK_EQUAL(4, c8_.c7_->c6_->c4_->i2);
    //BOOST_CHECK_EQUAL(1, c8_.c7_->c6_->c3_->i);
    //BOOST_CHECK_EQUAL(5, c8_.c7_->c6_->c5_.c2_->i);
    //BOOST_CHECK_EQUAL(0.0, c8_.c7_->c6_->c5_.c2_->d);
    //BOOST_CHECK_EQUAL(0, c8_.c7_->c6_->c5_.c2_->c);
    //BOOST_CHECK_EQUAL(true, c8_.c7_->c6_->c5_.c2_->b);
//}

//void check(const aux::shared_ptr<c8>& c8_) {
    //BOOST_CHECK(c8_->c1_ == c8_->c7_->c6_->c5_.c1_);
    //BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    //BOOST_CHECK(c8_->c7_->if0_ == c8_->c7_->c6_->c5_.if0_);

    //BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->c6_->c5_.if0_.get()));
    //BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0_.get()));

    //BOOST_CHECK_EQUAL(2, c8_->i);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    //BOOST_CHECK_EQUAL(3, c8_->c7_->c6_->c4_->i2);
    //BOOST_CHECK_EQUAL(1, c8_->c7_->c6_->c3_->i);
    //BOOST_CHECK_EQUAL(1, c8_->c7_->c6_->c5_.c2_->i);
    //BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
    //BOOST_CHECK_EQUAL(true, c8_->c7_->c6_->c5_.c2_->b);
//}

//using one_injector_types = mpl::vector<
    //injector<injector_1_t>
  //, injector<decltype(injector_1)>
//>;

//BOOST_AUTO_TEST_CASE_TEMPLATE(one_injector, TInjector, one_injector_types) {
    //TInjector injector;
    //check(injector.template create<c8>());
//}

//using multiple_injectors_types = mpl::vector<
    //injector<injector_2_t, injector_3_t>
  //, injector<injector_3_t, injector_2_t>
  //, injector<decltype(injector_2), decltype(injector_3)>
  //, injector<decltype(injector_3), decltype(injector_2)>
//>;

//BOOST_AUTO_TEST_CASE_TEMPLATE(multiple_injectors, TInjector, multiple_injectors_types) {
    //TInjector injector;
    //check(injector.template create<aux::shared_ptr<c8>>());
//}

//using mix_injectors_types = mpl::vector<
    //injector<injector_2_t, decltype(injector_2)>
  //, injector<decltype(injector_2), injector_2_t>
//>;

//BOOST_AUTO_TEST_CASE_TEMPLATE(mix_injectors, TInjector, mix_injectors_types) {
    //TInjector injector;
    //check(injector.template create<aux::shared_ptr<c8>>());
//}

//using basic_provider_types = mpl::vector<
    //injector<injector_provider_t>
  //, injector<decltype(injector_provider)>
//>;

//BOOST_AUTO_TEST_CASE_TEMPLATE(basic_provider, TInjector, basic_provider_types) {
    //TInjector injector;
    //transaction_usage obj = injector.template create<transaction_usage>();
    //BOOST_CHECK(obj.p->get().get() != obj.p->get().get());
//}

//using basic_visitor_types = mpl::vector<
    //injector<injector_provider_t>
  //, injector<decltype(injector_provider)>
//>;

//BOOST_AUTO_TEST_CASE_TEMPLATE(basic_visitor, TInjector, basic_visitor_types) {
    //TInjector injector;

    //fake_visitor<
        //mpl::vector<
            //transaction_usage
          //, aux::shared_ptr<provider<aux::shared_ptr<transaction>>>
          //, aux::shared_ptr<c3>
          //, int
        //>
    //> visitor;

    //injector.template visit<transaction_usage>(visitor);
//}

//using basic_call_types = mpl::vector<
    //injector<injector_custom_scope_t>
  //, injector<decltype(injector_custom_scope)>
//>;

//BOOST_AUTO_TEST_CASE_TEMPLATE(basic_call, TInjector, basic_call_types) {
    //fake_scope<>::entry_calls() = 0;
    //fake_scope<>::exit_calls() = 0;

    //TInjector injector;

    //BOOST_CHECK(!injector.template create<aux::shared_ptr<c3>>());

    //injector.call(fake_scope_entry());
    //BOOST_CHECK(injector.template create<aux::shared_ptr<c3>>());

    //injector.call(fake_scope_exit());
    //BOOST_CHECK(!injector.template create<aux::shared_ptr<c3>>());
//}

//BOOST_AUTO_TEST_CASE(basic_injector_externals) {
    //injector<decltype(injector_externals_2)> injector_(injector_externals_2);
    //auto c9_ = injector_.create<aux::shared_ptr<c9>>();

    //BOOST_CHECK_EQUAL(42, c9_->i);
    //BOOST_CHECK_EQUAL(87.0, c9_->d);
//}

//BOOST_AUTO_TEST_CASE(externals_priority) {
    //const int i = 87;

    //auto injector = make_injector(
        //bind_int<42>()
      //, bind<int>::to(i)
    //);

    //BOOST_CHECK_EQUAL(i, injector.create<c3>().i);
//}

//BOOST_AUTO_TEST_CASE(externals_mix) {
    //injector<
        //decltype(injector_externals_1)
      //, decltype(injector_externals_2)
    //> injector_(injector_externals_2, injector_externals_1);

    //auto c8_ = injector_.create<aux::shared_ptr<c8>>();

    //BOOST_CHECK_EQUAL(4, c8_->i);
    //BOOST_CHECK_EQUAL(3, c8_->c7_->c6_->c4_->i1);
    //BOOST_CHECK_EQUAL(42, c8_->c7_->c6_->c4_->i2);
    //BOOST_CHECK_EQUAL(42, c8_->c7_->c6_->c3_->i);
    //BOOST_CHECK_EQUAL(42, c8_->c7_->c6_->c5_.c2_->i);
    //BOOST_CHECK_EQUAL(87.0, c8_->c7_->c6_->c5_.c2_->d);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
    //BOOST_CHECK_EQUAL(67, dynamic_cast<c3if0&>(*c8_->c7_->c6_->c5_.if0_).i);
    //BOOST_CHECK_EQUAL(78.0, dynamic_cast<c3if0&>(*c8_->c7_->c6_->c5_.if0_).d);
    //BOOST_CHECK_EQUAL(67, dynamic_cast<c3if0&>(*c8_->c7_->if0_).i);
//}

//BOOST_AUTO_TEST_CASE(ctor) {
    //injector<decltype(injector_1)> injector(injector_1);
    //check(injector.create<c8>());
//}

//BOOST_AUTO_TEST_CASE(ctor_mix) {
    //injector<injector_2_t, decltype(injector_2)> injector(injector_2);
    //check(injector.create<aux::shared_ptr<c8>>());
//}

//BOOST_AUTO_TEST_CASE(ctor_mix_order) {
    //injector<decltype(injector_2), injector_2_t> injector(injector_2);
    //check(injector.create<aux::shared_ptr<c8>>());
//}

//BOOST_AUTO_TEST_CASE(ctor_mix_explicit) {
    //injector<
        //injector_2_t
      //, decltype(injector_2)
    //> injector(injector_2_t(), injector_2);
    //check(injector.create<aux::shared_ptr<c8>>());
//}

//BOOST_AUTO_TEST_CASE(ctor_mix_explicit_order) {
    //injector<
        //decltype(injector_2)
      //, injector_2_t
    //> injector(injector_2, injector_2_t());
    //check(injector.create<aux::shared_ptr<c8>>());
//}

//BOOST_AUTO_TEST_CASE(install) {
    //injector<injector_2_t, injector_3_t> injector_((injector_2_t(), injector_3_t()));
    //check(injector_.create<aux::shared_ptr<c8>>());
//}

//BOOST_AUTO_TEST_CASE(install_mix) {
    //injector<injector_2_t, decltype(injector_2)> injector_((injector_2_t(), injector_2));
    //check(injector_.create<aux::shared_ptr<c8>>());
//}

//BOOST_AUTO_TEST_CASE(scope_deduction) {
    //auto c19_ = injector<>().create<aux::shared_ptr<c19>>();
    //BOOST_CHECK(c19_->c1_ == c19_->c1__);
//}

//BOOST_AUTO_TEST_CASE(scope_deduction_named_shared_ptr) {
    //auto c22_ = injector<>().create<aux::shared_ptr<c22>>();
    //BOOST_CHECK(c22_->i == c22_->c11_->i);
//}

//using deduce_injectors_types = mpl::vector<
    //injector<injector_c0if0_t>
  //, injector<decltype(injector_c0if0)>
//>;

//BOOST_AUTO_TEST_CASE_TEMPLATE(scope_deduction_if, TInjector, deduce_injectors_types) {
    //TInjector injector;
    //auto c20_ = injector.template create<aux::shared_ptr<c20>>();
    //BOOST_CHECK(c20_->if0_ == c20_->if0__);
//}

//BOOST_AUTO_TEST_CASE_TEMPLATE(shared_ptr_unique_ptr, TInjector, deduce_injectors_types) {
    //TInjector injector;
    //auto c21_ = injector.template create<aux::shared_ptr<c21>>();
    //BOOST_CHECK(c21_->if0__ == c21_->if0__);
//}

//BOOST_AUTO_TEST_CASE(smart_ptr_weak_ptr) {
    //auto c25_ = injector<>().create<c25>();
    //BOOST_CHECK(nullptr != c25_.w1_.lock());
//}

//using creation_special_cases_types = mpl::vector<
    //noncopyable_const_ref
//#if !defined(BOOST_MSVC)
  //, rvalue
//#endif
//>;

//BOOST_AUTO_TEST_CASE_TEMPLATE(creation_special_cases, T, creation_special_cases_types) {
    //const int i = 42;
    //auto obj_ = injector<bind_int<i>>().create<T>();
    //BOOST_CHECK_EQUAL(i, obj_.i_);
//}

//BOOST_AUTO_TEST_CASE(stored_ref_created_by_injector) {
    //const int i = 42;
    //auto ref_sp_int_ = injector<bind_int<i>>().create<ref_sp_int>();
    //BOOST_CHECK(ref_sp_int_.i_);
//}

//BOOST_AUTO_TEST_CASE(smart_ptr_auto_ptr) {
    //const int i = 42;
    //auto auto_ptr_int_ = injector<bind_int<i>>().create<aux::auto_ptr<auto_ptr_int>>();
    //BOOST_CHECK_EQUAL(i, *auto_ptr_int_->i_);
//}

//using bind_multiple_interfaces_to_the_same_impl_types = mpl::vector<
    //cif0if1
  //, bind<any_of<if0, if1>, cif0if1>
  //, deduce<cif0if1>
//>;

//BOOST_AUTO_TEST_CASE_TEMPLATE(bind_multiple_interfaces_to_the_same_impl, T, bind_multiple_interfaces_to_the_same_impl_types) {
    //auto multiple_interfaces_ = injector<T>().template create<multiple_interfaces>();

    //BOOST_CHECK(dynamic_cast<cif0if1*>(multiple_interfaces_.if0_.get()));
    //BOOST_CHECK(dynamic_cast<cif0if1*>(multiple_interfaces_.if1_.get()));
    //BOOST_CHECK(
        //static_cast<cif0if1*>(multiple_interfaces_.if0_.get())
        //==
        //static_cast<cif0if1*>(multiple_interfaces_.if1_.get())
    //);
//}

//BOOST_AUTO_TEST_CASE(bind_multiple_interfaces_to_the_same_impl_without_any_of) {
    //auto multiple_interfaces_ = injector<
        //bind<if0, cif0if1>
      //, bind<if1, cif0if1>
    //>().create<multiple_interfaces>();

    //BOOST_CHECK(dynamic_cast<cif0if1*>(multiple_interfaces_.if0_.get()));
    //BOOST_CHECK(dynamic_cast<cif0if1*>(multiple_interfaces_.if1_.get()));
    //BOOST_CHECK(
        //static_cast<cif0if1*>(multiple_interfaces_.if0_.get())
        //!=
        //static_cast<cif0if1*>(multiple_interfaces_.if1_.get())
    //);
//}

//BOOST_AUTO_TEST_CASE(inject_priority) {
    //const int i = 42;
    //const double d = 87.0;
    //const float f = 12.0;

    //auto c28_ = make_injector(
        //bind<int>::to(i)
      //, bind<double>::to(d)
      //, bind<float>::to(f)
    //).create<c28>();

    //BOOST_CHECK_EQUAL(i, c28_.i_);
    //BOOST_CHECK_EQUAL(0.0, c28_.d_);
    //BOOST_CHECK_EQUAL(0.0, c28_.f_);
//}

//BOOST_AUTO_TEST_CASE(session_scope) {
    //injector<
        //session<c0if0>
    //> injector_;

    //{
    //auto c20_ = injector_.create<c20>();
    //BOOST_CHECK(nullptr == c20_.if0_.get());
    //BOOST_CHECK(nullptr == c20_.if0__.get());
    //}

    //{
    //injector_.call(scopes::session_entry());
    //auto c20_ = injector_.create<c20>();
    //BOOST_CHECK(dynamic_cast<c0if0*>(c20_.if0_.get()));
    //BOOST_CHECK(dynamic_cast<c0if0*>(c20_.if0__.get()));
    //BOOST_CHECK(c20_.if0_ == c20_.if0__);
    //}

    //{
    //injector_.call(scopes::session_exit());
    //auto c20_ = injector_.create<c20>();
    //BOOST_CHECK(nullptr == c20_.if0_.get());
    //BOOST_CHECK(nullptr == c20_.if0__.get());
    //}
//}

//BOOST_AUTO_TEST_CASE(scoped_injector_create) {
    //aux::shared_ptr<int> i1;
    //aux::shared_ptr<int> i2;
    //aux::shared_ptr<int> i3;

    //{
    //injector<shared<int>> i;

    //i1 = i.create<aux::shared_ptr<int>>();
    //i2 = i.create<aux::shared_ptr<int>>();

    //BOOST_CHECK(i1 == i2);
    //}

    //{
    //injector<shared<int>> i;

    //i3 = i.create<aux::shared_ptr<int>>();

    //BOOST_CHECK(i3 != i1);
    //BOOST_CHECK(i3 != i2);
    //}
//}

//BOOST_AUTO_TEST_CASE(scoped_injector_create_with_deduced_scope) {
    //aux::shared_ptr<c27> i1;
    //aux::shared_ptr<c27> i2;
    //aux::shared_ptr<c27> i3;

    //{
    //injector<> i;

    //i1 = i.create<aux::shared_ptr<c27>>();
    //i2 = i.create<aux::shared_ptr<c27>>();

    //BOOST_CHECK(i1->d_ == i2->d_);
    //}

    //{
    //injector<> i;

    //i3 = i.create<aux::shared_ptr<c27>>();

    //BOOST_CHECK(i3->d_ != i1->d_);
    //BOOST_CHECK(i3->d_ != i2->d_);
    //}
//}

//BOOST_AUTO_TEST_CASE(named_parameters_all_externals) {
    //const int i1 = 42;
    //const int i2 = 43;

    //auto injector = make_injector(
         //bind<i>::named<a>::to(aux::shared_ptr<impl>(new impl(i1)))
       //, bind<i>::named<b>::to(aux::shared_ptr<impl>(new impl(i2)))
    //);

    //auto nameds_ = injector.create<nameds>();

    //BOOST_CHECK(nameds_.n1_ != nameds_.n2_);
    //BOOST_CHECK(dynamic_cast<impl*>(nameds_.n1_.get()));
    //BOOST_CHECK(dynamic_cast<impl*>(nameds_.n2_.get()));
    //BOOST_CHECK_EQUAL(i1, dynamic_cast<impl*>(nameds_.n1_.get())->i);
    //BOOST_CHECK_EQUAL(i2, dynamic_cast<impl*>(nameds_.n2_.get())->i);
//}

//BOOST_AUTO_TEST_CASE(create_with_policies) {
    //using namespace di::policies;

    //injector<>().create<int>(
        //circular_dependencies()
      //, creation_permission<allow_type<int>>()
        //BOOST_DI_WKND(MSVC)()
        //BOOST_DI_WKND(NO_MSVC)(, parameters_permission<allow_copies>())
      //, scopes_permission<allow_scope<scopes::unique<>>>()
    //);

    //di::make_injector(di::bind_int<42>()).create<aux::shared_ptr<int>>(
        //scopes_permission<allow_scope<scopes::unique<>>, allow_scope<scopes::shared<>>>()
      //, creation_permission<>()
      //, circular_dependencies()
        //BOOST_DI_WKND(MSVC)()
        //BOOST_DI_WKND(NO_MSVC)(, parameters_permission<allow_smart_ptrs>())
    //);
//}

//BOOST_AUTO_TEST_CASE(allocate_with_policies) {
    //using namespace di::policies;

    //injector<>().allocate<int>(core::provider()
      //, circular_dependencies()
      //, creation_permission<allow_type<int>>()
        //BOOST_DI_WKND(MSVC)()
        //BOOST_DI_WKND(NO_MSVC)(, parameters_permission<allow_copies>())
      //, scopes_permission<allow_scope<scopes::unique<>>>()
    //);

    //di::make_injector(di::bind_int<42>()).allocate<aux::shared_ptr<int>>(core::provider()
      //, scopes_permission<allow_scope<scopes::unique<>>, allow_scope<scopes::shared<>>>()
      //, circular_dependencies()
        //BOOST_DI_WKND(MSVC)()
        //BOOST_DI_WKND(NO_MSVC)(, parameters_permission<allow_copies, allow_ptrs, allow_smart_ptrs, allow_const_refs>())
      //, creation_permission<>()
    //);
//}

//BOOST_AUTO_TEST_CASE(create_using_copy) {
    //module<> module_;
    //auto obj = module_.create<c0>();
    //(void)(obj);
//}

//BOOST_AUTO_TEST_CASE(create_using_ptr) {
    //module<> module_;
    //aux::auto_ptr<c0> obj(module_.create<c0*>());
    //BOOST_CHECK(obj.get());
//}

//BOOST_AUTO_TEST_CASE(create_using_const_ptr) {
    //module<> module_;
    //aux::auto_ptr<const c0> obj(module_.create<const c0*>());
    //BOOST_CHECK(obj.get());
//}

//BOOST_AUTO_TEST_CASE(create_using_shared_ptr) {
    //module<> module_;
    //auto obj = module_.create<aux::shared_ptr<c0>>();
    //BOOST_CHECK(obj);
//}

//BOOST_AUTO_TEST_CASE(create_default_ctor) {
    //module<> module_;
    //module_.create<c0>();
//}

//BOOST_AUTO_TEST_CASE(create_noncopyable) {
    //module<>().create<aux::shared_ptr<c1>>();
//}

//BOOST_AUTO_TEST_CASE(empty) {
    //module<> module_;
    //BOOST_CHECK_EQUAL(0, module_.create<int>());
//}

//BOOST_AUTO_TEST_CASE(create_unique) {
    //module<
        //mpl::vector<
            //fake_dependency<scopes::unique<>, if0, c0if0>::type
        //>
    //> module_;

    //aux::shared_ptr<c8> c8_ = module_.create<aux::shared_ptr<c8>>();

    //BOOST_CHECK(c8_->c1_ == c8_->c7_->c6_->c5_.c1_);
    //BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    //BOOST_CHECK(c8_->c7_->if0_ != c8_->c7_->c6_->c5_.if0_);

    //BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->c6_->c5_.if0_.get()));
    //BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0_.get()));

    //BOOST_CHECK_EQUAL(0, c8_->i);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i2);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c3_->i);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->i);
    //BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
//}

//BOOST_AUTO_TEST_CASE(create_unique_shared) {
    //module<
        //mpl::vector<
            //fake_dependency<scopes::unique<>, if0, c0if0>::type
          //, fake_dependency<scopes::shared<>, c3>::type
        //>
    //> module_;

    //aux::shared_ptr<c8> c8_ = module_.create<aux::shared_ptr<c8>>();

    //BOOST_CHECK(c8_->c1_ == c8_->c7_->c6_->c5_.c1_);
    //BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    //BOOST_CHECK(c8_->c7_->if0_ != c8_->c7_->c6_->c5_.if0_);

    //BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->c6_->c5_.if0_.get()));
    //BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0_.get()));

    //BOOST_CHECK_EQUAL(0, c8_->i);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i2);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c3_->i);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->i);
    //BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
//}

//BOOST_AUTO_TEST_CASE(create_shared_context) {
    //module<
        //mpl::vector<
            //fake_dependency<scopes::unique<>, if0, c0if0>::type
          //, fake_dependency<scopes::unique<>, if0, c1if0, no_name, mpl::vector<call_stack<c6, c5>>>::type
          //, fake_dependency<scopes::shared<>, c3>::type
        //>
    //>
    //module_;

    //aux::shared_ptr<c8> c8_ = module_.create<aux::shared_ptr<c8>>();

    //BOOST_CHECK(c8_->c1_ == c8_->c7_->c6_->c5_.c1_);
    //BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    //BOOST_CHECK(c8_->c7_->if0_ != c8_->c7_->c6_->c5_.if0_);

    //BOOST_CHECK(dynamic_cast<c1if0*>(c8_->c7_->c6_->c5_.if0_.get()));
    //BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0_.get()));

    //BOOST_CHECK_EQUAL(0, c8_->i);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i2);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c3_->i);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->i);
    //BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
//}

//BOOST_AUTO_TEST_CASE(create_unique_shared_context_order) {
    //module<
        //mpl::vector<
            //fake_dependency<scopes::unique<>, if0, c0if0>::type
          //, fake_dependency<scopes::unique<>, if0, c1if0, no_name, mpl::vector<call_stack<c6, c5>>>::type
          //, fake_dependency<scopes::unique<>, if0, c2if0, no_name, mpl::vector<call_stack<c7>>>::type
          //, fake_dependency<scopes::shared<>, c3>::type
        //>
    //> module_;

    //auto c8_ = module_.create<aux::shared_ptr<c8>>();

    //BOOST_CHECK(c8_->c1_ == c8_->c7_->c6_->c5_.c1_);
    //BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    //BOOST_CHECK(c8_->c7_->if0_ != c8_->c7_->c6_->c5_.if0_);

    //BOOST_CHECK(dynamic_cast<c1if0*>(c8_->c7_->c6_->c5_.if0_.get()));
    //BOOST_CHECK(dynamic_cast<c2if0*>(c8_->c7_->if0_.get()));

    //BOOST_CHECK_EQUAL(0, c8_->i);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i2);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c3_->i);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->i);
    //BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
//}

//BOOST_AUTO_TEST_CASE(create_unique_shared_context_mix) {
    //module<
        //mpl::vector<
            //fake_dependency<scopes::unique<>, if0, c0if0>::type
          //, fake_dependency<scopes::unique<>, if0, c1if0, no_name, mpl::vector<call_stack<c6, c5>>>::type
          //, fake_dependency<scopes::unique<>, if0, c2if0, no_name, mpl::vector<call_stack<c7>>>::type
          //, fake_dependency<scopes::shared<>, c3>::type
          //, fake_dependency<scopes::unique<>, int, mpl::int_<1>>::type
          //, fake_dependency<scopes::unique<>, int, mpl::int_<2>, no_name, mpl::vector<call_stack<c8>>>::type
          //, fake_dependency<scopes::unique<>, int, mpl::int_<3>, mpl::string<'1'>, mpl::vector<call_stack<c7, c6, c4>>>::type
          //, fake_dependency<scopes::unique<>, int, mpl::int_<4>, mpl::string<'2'>, mpl::vector<call_stack<c7, c6, c4>>>::type
          //, fake_dependency<scopes::unique<>, int, mpl::int_<5>, no_name, mpl::vector<call_stack<c2>>>::type
        //>
    //> module_;

    //auto c8_ = module_.create<aux::shared_ptr<c8>>();

    //BOOST_CHECK(c8_->c1_ == c8_->c7_->c6_->c5_.c1_);
    //BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    //BOOST_CHECK(c8_->c7_->if0_ != c8_->c7_->c6_->c5_.if0_);

    //BOOST_CHECK(dynamic_cast<c1if0*>(c8_->c7_->c6_->c5_.if0_.get()));
    //BOOST_CHECK(dynamic_cast<c2if0*>(c8_->c7_->if0_.get()));

    //BOOST_CHECK_EQUAL(2, c8_->i);
    //BOOST_CHECK_EQUAL(3, c8_->c7_->c6_->c4_->i1);
    //BOOST_CHECK_EQUAL(4, c8_->c7_->c6_->c4_->i2);
    //BOOST_CHECK_EQUAL(1, c8_->c7_->c6_->c3_->i);
    //BOOST_CHECK_EQUAL(5, c8_->c7_->c6_->c5_.c2_->i);
    //BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
//}

//BOOST_AUTO_TEST_CASE(create_shared_impl) {
    //module<
        //mpl::vector<
            //fake_dependency<scopes::shared<>, if0, c0if0>::type
        //>
    //> module_;

    //auto c8_ = module_.create<aux::shared_ptr<c8>>();

    //BOOST_CHECK(c8_->c1_ == c8_->c7_->c6_->c5_.c1_);
    //BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    //BOOST_CHECK(c8_->c7_->if0_ == c8_->c7_->c6_->c5_.if0_);

    //BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->c6_->c5_.if0_.get()));
    //BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0_.get()));

    //BOOST_CHECK_EQUAL(0, c8_->i);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i2);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c3_->i);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->i);
    //BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
//}

//BOOST_AUTO_TEST_CASE(create_shared_many) {
    //module<
        //mpl::vector<
            //fake_dependency<scopes::shared<>, if0, c0if0>::type
          //, fake_dependency<scopes::shared<>, c3>::type
          //, fake_dependency<scopes::shared<>, c1>::type
        //>
    //> module_;

    //auto c8_ = module_.create<aux::shared_ptr<c8>>();

    //BOOST_CHECK(c8_->c1_ == c8_->c7_->c6_->c5_.c1_);
    //BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    //BOOST_CHECK(c8_->c7_->if0_ == c8_->c7_->c6_->c5_.if0_);

    //BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->c6_->c5_.if0_.get()));
    //BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0_.get()));

    //BOOST_CHECK_EQUAL(0, c8_->i);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i2);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c3_->i);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->i);
    //BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
//}

//BOOST_AUTO_TEST_CASE(ctor_trait) {
    //const int i = 42;

    //module<
        //mpl::vector<
            //fake_dependency<scopes::unique<>, int, mpl::int_<i>>::type
        //>
    //> module_;

    //auto obj = module_.create<c14>();

    //BOOST_CHECK_EQUAL(i, obj.i);
    //BOOST_CHECK_EQUAL(0.0, obj.d);
//}

//BOOST_AUTO_TEST_CASE(class_ctor_traits) {
    //const int i1 = 42;
    //const int i2 = 87;

    //module<
        //mpl::vector<
            //fake_dependency<scopes::unique<>, int, mpl::int_<i1>, mpl::string<'1'>>::type
          //, fake_dependency<scopes::unique<>, int, mpl::int_<i2>, mpl::string<'2'>>::type
        //>
    //> module_;

    //auto obj = module_.create<c10>();

    //BOOST_CHECK_EQUAL(i1, obj.i1);
    //BOOST_CHECK_EQUAL(i2, obj.i2);
//}

//BOOST_AUTO_TEST_CASE(base_of) {
    //module<
        //mpl::vector<
            //fake_dependency<scopes::unique<>, int, mpl::int_<1>>::type
          //, fake_dependency<scopes::unique<>, int, mpl::int_<4>, mpl::string<'2'>, mpl::vector<call_stack<c7, c6, c4>>>::type
          //, fake_dependency<scopes::unique<>, int, mpl::int_<5>, no_name, mpl::vector<call_stack<c2>>>::type
          //, fake_dependency<scopes::unique<>, c0if0, c0if0>::type
          //, fake_dependency<scopes::unique<>, int, mpl::int_<3>, mpl::string<'1'>, mpl::vector<call_stack<c7, c6, c4>>>::type
        //>
    //> module_;

    //auto c8_ = module_.create<aux::shared_ptr<c8>>();

    //BOOST_CHECK(c8_->c1_ == c8_->c7_->c6_->c5_.c1_);
    //BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    //BOOST_CHECK(c8_->c7_->if0_ != c8_->c7_->c6_->c5_.if0_);

    //BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->c6_->c5_.if0_.get()));
    //BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0_.get()));

    //BOOST_CHECK_EQUAL(1, c8_->i);
    //BOOST_CHECK_EQUAL(3, c8_->c7_->c6_->c4_->i1);
    //BOOST_CHECK_EQUAL(4, c8_->c7_->c6_->c4_->i2);
    //BOOST_CHECK_EQUAL(1, c8_->c7_->c6_->c3_->i);
    //BOOST_CHECK_EQUAL(5, c8_->c7_->c6_->c5_.c2_->i);
    //BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    //BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
//}

//BOOST_AUTO_TEST_CASE(multiple_calls) {
    //module<
        //mpl::vector<
            //fake_dependency<scopes::unique<>, c0if0, c0if0>::type
          //, fake_dependency<scopes::shared<>, c3, c3, no_name, mpl::vector<call_stack<c15>, call_stack<c6, c4>>>::type
          //, fake_dependency<scopes::shared<>, c3, c3, no_name, mpl::vector<call_stack<c6>>>::type
        //>
    //> module_;

    //auto c15_ = module_.create<aux::shared_ptr<c15>>();

    //BOOST_CHECK(c15_->c3_ == c15_->c6_.c4_->c3_); BOOST_CHECK(c15_->c3_ != c15_->c6_.c3_);
//}

//BOOST_AUTO_TEST_CASE(base_of_interface_not_trivial_ctor) {
    //module<
        //mpl::vector<
            //fake_dependency<scopes::unique<>, transaction_provider, transaction_provider>::type
        //>
    //> module_;

    //auto obj = module_.create<transaction_usage>();

    //BOOST_CHECK(obj.p->get().get() != obj.p->get().get());
//}

//BOOST_AUTO_TEST_CASE(named_shared_ptr_explicit_value) {
    //const int i = 42;

    //module<
        //mpl::vector<
            //fake_dependency<scopes::unique<>, int, mpl::int_<i>, mpl::string<'1'>>::type
        //>
    //> module_;

    //auto obj = module_.create<c11>();

    //BOOST_CHECK_EQUAL(i, *obj.i);
//}

//BOOST_AUTO_TEST_CASE(named_shared_ptr_base_of_explicit_value) {
    //const int i = 42;

    //module<
        //mpl::vector<
            //fake_dependency<scopes::unique<>, int, mpl::int_<i>, mpl::string<'1'>>::type
        //>
    //> module_;

    //auto obj = module_.create<c11>();

    //BOOST_CHECK_EQUAL(i, *obj.i);
//}

//BOOST_AUTO_TEST_CASE(named_shared_ptr_if) {
    //module<
        //mpl::vector<
            //fake_dependency<scopes::unique<>, if0, c0if0, mpl::string<'1'>>::type
        //>
    //> module_;

    //auto obj = module_.create<c12>();

    //BOOST_CHECK(dynamic_cast<c0if0*>(obj.if0_.get()));
    //BOOST_CHECK_EQUAL(0, obj.c2_->i);
    //BOOST_CHECK_EQUAL(0.0, obj.c2_->d);
    //BOOST_CHECK_EQUAL(0, obj.c2_->c);
//}

//BOOST_AUTO_TEST_CASE(named_shared_ptr_if_with_not_trivial_ctor) {
    //const int i = 42;

    //module<
        //mpl::vector<
            //fake_dependency<scopes::unique<>, if0, c3if0>::type
          //, fake_dependency<scopes::unique<>, int, mpl::int_<i>>::type
        //>
    //> module_;

    //auto obj = module_.create<c13>();

    //c3if0* if0_ = dynamic_cast<c3if0*>(obj.if0_.get());
    //BOOST_CHECK(if0_);
    //BOOST_CHECK_EQUAL(i, obj.c3_.i);
    //BOOST_CHECK_EQUAL(i, if0_->i);
    //BOOST_CHECK_EQUAL(0.0, if0_->d);
//}

//BOOST_AUTO_TEST_CASE(externals_create_by_explicit_value) {
    //const int i = 42;
    //const double d = 21.0;
    //const char c = 'x';

    //fake_dependency<scopes::external<>, int>::type i_(i);
    //fake_dependency<scopes::external<>, double>::type d_(d);
    //fake_dependency<scopes::external<>, char>::type c_(c);

    //module<
        //mpl::vector<
            //fake_dependency<scopes::unique<>, std::string, mpl::string<'s'>>::type
          //, fake_dependency<scopes::external<>, int>::type
          //, fake_dependency<scopes::external<>, double>::type
          //, fake_dependency<scopes::external<>, char>::type
        //>
    //> module_(i_, d_, c_);

    //auto obj = module_.create<c9>();

    //BOOST_CHECK_EQUAL(i, obj.i);
    //BOOST_CHECK_EQUAL(d, obj.d);
    //BOOST_CHECK_EQUAL(c, obj.c);
    //BOOST_CHECK_EQUAL("s", obj.s);
//}

//BOOST_AUTO_TEST_CASE(externals_create_with_non_trivial_ctor) {
    //const int i = 42;
    //const double d = 21.0;
    //const char c = 'x';
    //const bool b = true;

    //using external_shared = scopes::external<wrappers::shared>;
    //fake_dependency<external_shared, c2>::type c2_(aux::shared_ptr<c2>(new c2(i, d, c, b)));

    //module<
        //mpl::vector<
            //fake_dependency<external_shared, c2>::type
        //>
    //> module_(c2_);

    //auto obj = module_.create<aux::shared_ptr<c2>>();

    //BOOST_CHECK_EQUAL(i, obj->i);
    //BOOST_CHECK_EQUAL(d, obj->d);
    //BOOST_CHECK_EQUAL(c, obj->c);
    //BOOST_CHECK_EQUAL(b, obj->b);
//}

//using externals_priority_t = mpl::vector<
    //module<
        //mpl::vector<
            //fake_dependency<scopes::unique<>, int, int, mpl::string<'1'>>::type
          //, fake_dependency<scopes::unique<>, int, int, mpl::string<'2'>>::type
          //, fake_dependency<scopes::external<>, int, int, mpl::string<'1'>>::type
          //, fake_dependency<scopes::external<>, int, int, mpl::string<'2'>>::type
        //>
    //>
  //, module<
        //mpl::vector<
            //fake_dependency<scopes::external<>, int, int, mpl::string<'1'>>::type
          //, fake_dependency<scopes::external<>, int, int, mpl::string<'2'>>::type
          //, fake_dependency<scopes::unique<>, int, int, mpl::string<'1'>>::type
          //, fake_dependency<scopes::unique<>, int, int, mpl::string<'2'>>::type
        //>
    //>
  //, module<
        //mpl::vector<
            //fake_dependency<scopes::unique<>, int, int, mpl::string<'1'>>::type
          //, fake_dependency<scopes::unique<>, int, int, mpl::string<'2'>>::type
          //, fake_dependency<scopes::external<>, int, int, mpl::string<'1'>>::type
          //, fake_dependency<scopes::external<>, int, int, mpl::string<'2'>>::type
        //>
    //>
  //, module<
        //mpl::vector<
            //fake_dependency<scopes::external<>, int, int, mpl::string<'1'>>::type
          //, fake_dependency<scopes::external<>, int, int, mpl::string<'2'>>::type
          //, fake_dependency<scopes::unique<>, int, int, mpl::string<'1'>>::type
          //, fake_dependency<scopes::unique<>, int, int, mpl::string<'2'>>::type
        //>
    //>
//>;

//BOOST_AUTO_TEST_CASE_TEMPLATE(externals_create_priority, TModule, externals_priority_t) {
    //const int i1 = 42;
    //const int i2 = 87;

    //fake_dependency<scopes::external<>, int, int, mpl::string<'1'>>::type i1_(i1);
    //fake_dependency<scopes::external<>, int, int, mpl::string<'2'>>::type i2_(i2);

    //TModule module_(i1_, i2_);

    //auto obj = module_.template create<c4>();

    //BOOST_CHECK_EQUAL(i1, obj.i1);
    //BOOST_CHECK_EQUAL(i2, obj.i2);
//}

//BOOST_AUTO_TEST_CASE(visit) {
    //module<
        //mpl::vector<
            //fake_dependency<scopes::unique<>, transaction_provider, transaction_provider>::type
          //, fake_dependency<scopes::unique<>, int, mpl::int_<0>>::type
        //>
    //> module_;

    //fake_visitor<
        //mpl::vector<
            //transaction_usage
          //, aux::shared_ptr<di::provider<aux::shared_ptr<transaction>>>
          //, aux::shared_ptr<c3>
          //, int
        //>
    //> visitor;

    //module_.visit<transaction_usage>(visitor);
//}

//BOOST_AUTO_TEST_CASE(visit_external) {
    //c3 c3_;
    //fake_dependency<scopes::external<wrappers::reference>, c3>::type e3(boost::ref(c3_));

    //module<
        //mpl::vector<
            //fake_dependency<scopes::external<wrappers::reference>, c3>::type
        //>
    //> module_(e3);

    //fake_visitor<
        //mpl::vector<
            //mpl::pair<c18, scopes::unique<>>
          //, mpl::pair<c0, scopes::unique<>>
          //, mpl::pair<aux::shared_ptr<c1>, scopes::shared<>>
          //, mpl::pair<int, scopes::unique<>>
          //, mpl::pair<c3&, scopes::external<wrappers::reference>>
        //>
    //> visitor;

    //module_.visit<c18>(visitor);
//}

//BOOST_AUTO_TEST_CASE(call) {
    //fake_scope<>::entry_calls() = 0;
    //fake_scope<>::exit_calls() = 0;

    //module<
        //mpl::vector<
            //fake_dependency<fake_scope<>, c0if0, c0if0>::type
        //>
    //> module_;

    //BOOST_CHECK_EQUAL(0, fake_scope<>::entry_calls());
    //BOOST_CHECK_EQUAL(0, fake_scope<>::exit_calls());

    //module_.call(fake_scope_entry());
    //BOOST_CHECK_EQUAL(1, fake_scope<>::entry_calls());
    //BOOST_CHECK_EQUAL(0, fake_scope<>::exit_calls());

    //fake_scope<>::entry_calls() = 0;
    //fake_scope<>::exit_calls() = 0;

    //module_.call(fake_scope_exit());
    //BOOST_CHECK_EQUAL(0, fake_scope<>::entry_calls());
    //BOOST_CHECK_EQUAL(1, fake_scope<>::exit_calls());
//}

//BOOST_AUTO_TEST_CASE(create_with_policies) {
    //module<> module_;
    //fake_policy<1>::assert_calls() = 0;
    //fake_policy<2>::assert_calls() = 0;

    //BOOST_CHECK_EQUAL(0, module_.create<int>(fake_policy<1>(), fake_policy<2>()));
    //BOOST_CHECK_EQUAL(1, fake_policy<1>::assert_calls());
    //BOOST_CHECK_EQUAL(1, fake_policy<1>::assert_calls());
//}

//BOOST_AUTO_TEST_CASE(allocate) {
    //module<> module_;
    //fake_provider::provide_calls() = 0;
    //BOOST_CHECK_EQUAL(0, module_.allocate<int>(fake_provider()));
    //BOOST_CHECK_EQUAL(1, fake_provider::provide_calls());
//}

//BOOST_AUTO_TEST_CASE(allocate_with_policies) {
    //module<> module_;
    //fake_provider::provide_calls() = 0;
    //fake_policy<1>::assert_calls() = 0;
    //fake_policy<2>::assert_calls() = 0;

    //BOOST_CHECK_EQUAL(0, module_.allocate<int>(fake_provider(), fake_policy<1>(), fake_policy<2>()));
    //BOOST_CHECK_EQUAL(1, fake_provider::provide_calls());
    //BOOST_CHECK_EQUAL(1, fake_policy<1>::assert_calls());
    //BOOST_CHECK_EQUAL(1, fake_policy<1>::assert_calls());
//}
