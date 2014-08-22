//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/make_injector.hpp"

#include <boost/test/unit_test.hpp>
#include <functional>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/typeof/typeof.hpp>

#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/config.hpp"
#include "boost/di/bindings.hpp"
//#include "boost/di/policies/circular_dependencies.hpp"

#include "common/fakes/fake_provider.hpp"
#include "common/data.hpp"

namespace boost {
namespace di {

struct empty_module {
    auto configure() const -> decltype(make_injector()) {
        return make_injector();
    }
};

struct module1 {
    using injector_t = injector<c0if0>;

    injector_t configure() const {
        return injector_t();
    }
};

struct module2 {
    using injector_t = injector<decltype(bind<int>::to(int()))>;

    explicit module2(int i)
        : i_(i)
    { }

    injector_t configure() const {
        return injector_t(bind<int>::to(i_));
    }

    int i_ = 0;
};

BOOST_AUTO_TEST_CASE(ctor) {
    using injector_c0 = injector<
        c0if0
    >;

    aux::shared_ptr<c5> c5_ = injector_c0().create<aux::shared_ptr<c5>>();
    BOOST_CHECK_EQUAL(0, c5_->c2_->i);
}

BOOST_AUTO_TEST_CASE(empty) {
    auto injector_empty = make_injector();
    BOOST_CHECK_EQUAL(0, injector_empty.create<c3>().i);
}

BOOST_AUTO_TEST_CASE(by_value) {
    const int i = 42;
    using injector_c0 = injector<
        c0if0
    >;

    auto injector_c0_int_ = make_injector(
        injector_c0()
      , bind<int>::to(i)
    );

    aux::shared_ptr<c5> c5_ = injector_c0_int_.create<aux::shared_ptr<c5>>();
    BOOST_CHECK_EQUAL(i, c5_->c2_->i);
}

BOOST_AUTO_TEST_CASE(with_policy) {
    const int i = 42;

    auto injector_ = make_injector(
        bind_int<i>()
    );

    BOOST_CHECK_EQUAL(i, injector_.create<c3>(
        //policies::circular_dependencies()
    ).i);
}

BOOST_AUTO_TEST_CASE(with_policy_seperate) {
    const int i = 42;

    auto injector_ = make_injector(
        bind_int<i>()
    );

    BOOST_CHECK_EQUAL(i, injector_.create<c3>(
        //policies::circular_dependencies()
    ).i);
}

BOOST_AUTO_TEST_CASE(create_mix) {
    const int i = 42;
    const double d = 42.0;

    using injector_c0 = injector<
        c0if0
      , bind_int<i>
    >;

    auto injector_c1 = make_injector(
        shared<
            c1
        >()
    );

    auto injector_ = make_injector(
        injector_c0()
      , unique<
            c2
        >()
      , injector_c1
      , bind<double>::to(d)
    );

    auto c5_ = injector_.create<aux::shared_ptr<c5>>(
        //policies::circular_dependencies()
    );

    BOOST_CHECK(dynamic_cast<c0if0*>(c5_->if0_.get()));
    BOOST_CHECK_EQUAL(i, c5_->c2_->i);
    BOOST_CHECK_EQUAL(d, c5_->c2_->d);
    BOOST_CHECK_EQUAL(0, c5_->c2_->c);
}

BOOST_AUTO_TEST_CASE(allocate) {
    auto injector = make_injector();
    fake_provider::get_calls() = 0;
    auto i = injector.allocate<int>(fake_provider());
    BOOST_CHECK_EQUAL(0, i);
    BOOST_CHECK_EQUAL(1, fake_provider::get_calls());
}

BOOST_AUTO_TEST_CASE(allocate_with_policies) {
    auto injector = make_injector();
    fake_provider::get_calls() = 0;
    auto i = injector.allocate<int>(
        fake_provider()
      //, policies::circular_dependencies()
    );
    BOOST_CHECK_EQUAL(0, i);
    BOOST_CHECK_EQUAL(1, fake_provider::get_calls());
}

BOOST_AUTO_TEST_CASE(externals_return_from_function) {
    const int i = 42;
    const double d = 87.0;

    using externals = injector<
        c0if0
      , decltype(bind<int>::to(int()))
      , BOOST_TYPEOF(bind<double>::to(double()))
    >;

    externals injector = externals(
        bind<int>::to(i)
      , bind<double>::to(d)
    );

    auto c5_ = injector.create<c5>();

    BOOST_CHECK(dynamic_cast<c0if0*>(c5_.if0_.get()));
    BOOST_CHECK_EQUAL(i, c5_.c2_->i);
    BOOST_CHECK_EQUAL(d, c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c5_.c2_->c);
}

BOOST_AUTO_TEST_CASE(runtime_factory_impl) {
    const auto debug_property = false;
    const auto value = 42;

    auto common = make_injector(
        bind<int>::to(value)
    );

    auto all = make_injector(
        common
      , bind<i>::to([&]() -> aux::shared_ptr<i> {
            if (debug_property) {
                return aux::shared_ptr<i>(new fake());
            }

            return common.create<aux::shared_ptr<impl>>();
        })
    );

    auto i_ = all.create<aux::shared_ptr<i>>();

    BOOST_CHECK(dynamic_cast<impl*>(i_.get()));
    BOOST_CHECK_EQUAL(value, dynamic_cast<impl*>(i_.get())->i);
}

BOOST_AUTO_TEST_CASE(runtime_factory_fake) {
    const auto debug_property = true;
    const auto value = 42;

    auto common = make_injector(
        bind<int>::to(value)
    );

    auto all = make_injector(
        common
      , bind<i>::to([&]() -> aux::shared_ptr<i> {
            if (debug_property) {
                return aux::shared_ptr<i>(new fake());
            }

            return common.create<aux::shared_ptr<impl>>();
        })
    );

    auto i_ = all.create<aux::shared_ptr<i>>();

    BOOST_CHECK(dynamic_cast<fake*>(i_.get()));
}

BOOST_AUTO_TEST_CASE(modules_injector) {
    const int i = 42;
    injector<module1, module2> injector_{module2(i)};

    BOOST_CHECK(dynamic_cast<c0if0*>(injector_.create<aux::auto_ptr<if0>>().get()));
    BOOST_CHECK_EQUAL(i, injector_.create<int>());
}

BOOST_AUTO_TEST_CASE(modules_make_injector) {
    const int i = 42;
    auto injector_ = make_injector(module1(), module2(i));

    BOOST_CHECK(dynamic_cast<c0if0*>(injector_.create<aux::auto_ptr<if0>>().get()));
    BOOST_CHECK_EQUAL(i, injector_.create<int>());
}

BOOST_AUTO_TEST_CASE(modules_mix_make_injector) {
    const int i = 42;
    const double d = 87.0;
    const float f = 123.0;
    const std::string s = "string";

    auto injector_string = make_injector(
        bind<std::string>::to(s)
    );

    auto injector_ = make_injector(
        empty_module()
      , bind<double>::to(d)
      , module1()
      , make_injector(
            bind<float>::to(f)
        )
      , injector_string
      , module2(i)
    );

    BOOST_CHECK(dynamic_cast<c0if0*>(injector_.create<aux::auto_ptr<if0>>().get()));
    BOOST_CHECK_EQUAL(i, injector_.create<int>());
    BOOST_CHECK_EQUAL(d, injector_.create<double>());
    BOOST_CHECK_EQUAL(f, injector_.create<float>());
    BOOST_CHECK_EQUAL(s, injector_.create<std::string>());
}

#if !defined(_MSC_VER)
BOOST_AUTO_TEST_CASE(wrappers_types_mix) {
    const int i1 = 42;
    const int i2 = 43;
    const float f1 = 123.0;
    const float f2 = 124.0;
    const double d1 = 87.0;
    const double d2 = 88.0;
    const short s1 = 1;
    const short s2 = 2;
    const char ch1 = '0';
    const char ch2 = '1';
    const long l = 77;

    auto injector_ = make_injector(
        bind_int<i1>()
      , bind_int<i2>::named<a>()
      , bind<float>::to(f1)
      , bind<float>::named<b>::to(f2)
      , bind<double>::to(d1)
      , bind<double>::named<c>::to(d2)
      , bind<short>::to(s1)
      , bind<short>::named<d>::to(s2)
      , bind<char>::to(ch1)
      , bind<char>::named<e>::to(ch2)
      , bind<long>::to(l)
    );

    auto wrappers_ = injector_.create<aux::unique_ptr<wrappers_types>>();

    BOOST_CHECK_EQUAL(i1, wrappers_->i_);
    BOOST_CHECK_EQUAL(i2, wrappers_->ii_);
    BOOST_CHECK_EQUAL(f1, *wrappers_->f_);
    BOOST_CHECK_EQUAL(f2, *wrappers_->ff_);
    BOOST_CHECK_EQUAL(d1, *wrappers_->d_);
    BOOST_CHECK_EQUAL(d2, *wrappers_->dd_);
    BOOST_CHECK_EQUAL(s1, *wrappers_->s_);
    BOOST_CHECK_EQUAL(s2, *wrappers_->ss_);
    BOOST_CHECK_EQUAL(ch1, wrappers_->c_);
    BOOST_CHECK_EQUAL(ch2, wrappers_->cc_);
    BOOST_CHECK_EQUAL(l, wrappers_->l_);
    BOOST_CHECK_EQUAL(l, wrappers_->ll_);
}
#endif

BOOST_AUTO_TEST_CASE(externals_ref_cref) {
    int i = 42;
    const double d = 87.0;
    short s = 0;
    const char c = 'a';

    auto injector = make_injector(
        bind<int>::to(boost::ref(i))
      , bind<double>::to(boost::cref(d))
      , bind<short>::to(std::ref(s))
      , bind<char>::to(std::cref(c))
    );

    auto refs_ = injector.create<refs>();

    BOOST_CHECK_EQUAL(i, refs_.i_);
    BOOST_CHECK_EQUAL(d, refs_.d_);
    BOOST_CHECK_EQUAL(s, refs_.s_);
    BOOST_CHECK_EQUAL(c, refs_.c_);
}

namespace {

double return_double(double d) { return d; }
long return_long(long l) { return l; }

} // namespace

BOOST_AUTO_TEST_CASE(bind_to_function_ptr) {
    const int i = 42;
    const double d = 87.0;
    const long l = 0;

    auto injector = make_injector(
        bind<function<int()>>::to([&]{ return i; })
      , bind<function<double()>>::to(boost::bind(&return_double, d))
      , bind<function<long()>>::to(std::bind(&return_long, l))
    );

    auto functions_ = injector.create<functions>();

    BOOST_CHECK_EQUAL(i, functions_.fi_());
    BOOST_CHECK_EQUAL(d, functions_.fd_());
    BOOST_CHECK_EQUAL(l, functions_.fl_());
}

BOOST_AUTO_TEST_CASE(named_parameters_with_shared_scope) {
    auto injector = make_injector(
        shared<impl>()
      , bind<i>::named<b>::to(aux::shared_ptr<impl>(new impl()))
    );

    auto nameds_ = injector.create<nameds>();
    BOOST_CHECK(nameds_.n1_ != nameds_.n2_);

    BOOST_CHECK(nameds_.n1_ == injector.create<aux::shared_ptr<i>>());
    BOOST_CHECK(nameds_.n2_ != injector.create<aux::shared_ptr<i>>());
}

BOOST_AUTO_TEST_CASE(named_parameters_with_unique_scope) {
    auto injector = make_injector(
        unique<impl>()
      , bind<i>::named<b>::to(aux::shared_ptr<impl>(new impl()))
    );

    auto nameds_ = injector.create<nameds>();
    BOOST_CHECK(nameds_.n1_ != nameds_.n2_);

    BOOST_CHECK(nameds_.n1_ != injector.create<aux::shared_ptr<i>>());
    BOOST_CHECK(nameds_.n2_ != injector.create<aux::shared_ptr<i>>());
}

BOOST_AUTO_TEST_CASE(context_with_shared_ptr_type) {
    const int i1 = 42;
    const int i2 = 43;

    auto injector = make_injector(
        bind_int<i1>::when<context<context_with_shared_ptr, context_nested>>()
      , bind<int>::when<context<context_with_shared_ptr, context_nested>>::named<b>::to(i2)
    );

    auto context_ = injector.create<context_with_shared_ptr>();

    BOOST_CHECK_EQUAL(0, context_.i_);
    BOOST_CHECK_EQUAL(0, context_.ia_);
    BOOST_CHECK_EQUAL(0, context_.ib_);
    BOOST_CHECK_EQUAL(i1, context_.nested_->i_);
    BOOST_CHECK_EQUAL(i1, context_.nested_->ia_);
    BOOST_CHECK_EQUAL(i2, context_.nested_->ib_);
}

BOOST_AUTO_TEST_CASE(create_with_default_values) {
    const int i = 42;

    auto injector = make_injector(
        di::bind_int<i>()
    );

    auto default_values_ = injector.create<default_values>();

    BOOST_CHECK_EQUAL(i, default_values_.i_);
    BOOST_CHECK_EQUAL(42.0, default_values_.f_);
    BOOST_CHECK_EQUAL(87.0, default_values_.d_);
}

} // namespace di
} // namespace boost

