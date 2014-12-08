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

