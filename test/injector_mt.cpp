//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/injector.hpp"
#include "boost/di/make_injector.hpp"
#include "boost/di/aux_/thread.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/vector.hpp>

#include "fake_visitor.hpp"
#include "fake_scope.hpp"
#include "data.hpp"

namespace boost {
namespace di {

#if 0
using injector_1_t = injector<
    scoped<
        c3
    >
  , per_request<
        c0if0
      , bind<c1if0>::in_call<call_stack<c6, c5>>
      , bind<c2if0>::in_call<c7>
      , bind_int<1>
      , bind_int<2>::in_call<c8>
      , bind_int<3>::in_name<mpl::string<'1'>>::in_call<call_stack<c7, c6, c4>>
      , bind_int<4>::in_name<mpl::string<'2'>>::in_call<call_stack<c7, c6, c4>>
      , bind_int<5>::in_call<c2>
    >
>;

using injector_2_t = injector<
    scoped<
        c3
    >
  , per_request<
        bind_int<0>::in_name<mpl::string<'1'>>
      , bind_int<1>
    >
>;

using injector_3_t = injector<
    scoped<
        c0if0
    >
  , per_request<
        bind_int<2>::in_call<c8>
      , bind_int<3>::in_name<mpl::string<'2'>>
    >
>;

using injector_custom_scope_t = injector<
    scope<fake_scope<>>::bind<c3, int>
>;

using injector_provider_t = injector<
    per_request<
        transaction_provider
    >
>;

using injector_c0if0_t = injector<
    c0if0
>;

auto injector_1 = make_injector(
    scoped<
        c3
    >()
  , per_request<
        c0if0
      , bind<c1if0>::in_call<call_stack<c6, c5>>
      , bind<c2if0>::in_call<c7>
      , bind_int<1>
      , bind_int<2>::in_call<c8>
      , bind_int<3>::in_name<mpl::string<'1'>>::in_call<call_stack<c7, c6, c4>>
      , bind_int<4>::in_name<mpl::string<'2'>>::in_call<call_stack<c7, c6, c4>>
      , bind_int<5>::in_call<c2>
    >()
);

auto injector_2 = make_injector(
    scoped<
        c0if0
    >()
  , per_request<
        bind_int<2>::in_call<c8>
      , bind_int<3>::in_name<mpl::string<'2'>>
    >()
);

auto injector_3 = make_injector(
    scoped<
        c3
    >()
  , per_request<
        bind_int<0>::in_name<mpl::string<'1'>>
      , bind_int<1>
    >()
);

auto injector_custom_scope = make_injector(
    scope<fake_scope<>>::bind<c3, int>()
);

auto injector_provider = make_injector(
    per_request<
        transaction_provider, mpl::int_<0>
    >()
);

auto injector_externals = make_injector(
    bind<double>::to(7.0)
  , bind<if0>::to(shared_ptr<c3if0>(new c3if0(67, 78.0)))
  , bind<int>::in_name<mpl::string<'1'>>::in_call<call_stack<c7, c6, c4>>::to(3)
  , bind<int>::in_call<c8>::to(4)
);

auto injector_externals_1 = make_injector(
    bind<if0>::to(shared_ptr<c3if0>(new c3if0(67, 78.0)))
  , bind<int>::in_name<mpl::string<'1'>>::in_call<call_stack<c7, c6, c4>>::to(3)
  , bind<int>::in_call<c8>::to(4)
);

auto injector_externals_2 = make_injector(
    scoped<
        c0if0
    >()
  , bind<int>::to(42)
  , bind<double>::to(87.0)
);

auto injector_c0if0 = make_injector(
    deduce<c0if0>()
);

void check(const c8& c8_) {
    BOOST_CHECK(c8_.c1_ == c8_.c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_.c7_->c6_->c4_->c3_ == c8_.c7_->c6_->c3_);
    BOOST_CHECK(c8_.c7_->if0_ != c8_.c7_->c6_->c5_.if0_);

    BOOST_CHECK(dynamic_cast<c1if0*>(c8_.c7_->c6_->c5_.if0_.get()));
    BOOST_CHECK(dynamic_cast<c2if0*>(c8_.c7_->if0_.get()));

    BOOST_CHECK_EQUAL(2, c8_.i);
    BOOST_CHECK_EQUAL(3, c8_.c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(4, c8_.c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(1, c8_.c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(5, c8_.c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_.c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_.c7_->c6_->c5_.c2_->c);
}

void check(const shared_ptr<c8>& c8_) {
    BOOST_CHECK(c8_->c1_ == c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0_ == c8_->c7_->c6_->c5_.if0_);

    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->c6_->c5_.if0_.get()));
    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0_.get()));

    BOOST_CHECK_EQUAL(2, c8_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(3, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(1, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(1, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
}

using one_injector_types = mpl::vector<
    injector<injector_1_t>
  , injector<decltype(injector_1)>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(one_injector, TInjector, one_injector_types) {
    TInjector injector;
    check(injector.template create<c8>());
}

using many_injectors_types = mpl::vector<
    injector<injector_2_t, injector_3_t>
  , injector<injector_3_t, injector_2_t>
  , injector<decltype(injector_2), decltype(injector_3)>
  , injector<decltype(injector_3), decltype(injector_2)>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(many_injectors, TInjector, many_injectors_types) {
    TInjector injector;
    check(injector.template create<shared_ptr<c8>>());
}

using mix_injectors_types = mpl::vector<
    injector<injector_2_t, decltype(injector_2)>
  , injector<decltype(injector_2), injector_2_t>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(mix_injectors, TInjector, mix_injectors_types) {
    TInjector injector;
    check(injector.template create<shared_ptr<c8>>());
}

using basic_provider_types = mpl::vector<
    injector<injector_provider_t>
  , injector<decltype(injector_provider)>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(basic_provider, TInjector, basic_provider_types) {
    TInjector injector;
    transaction_usage obj = injector.template create<transaction_usage>();
    BOOST_CHECK(obj.p->get().get() != obj.p->get().get());
}

using basic_visitor_types = mpl::vector<
    injector<injector_provider_t>
  , injector<decltype(injector_provider)>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(basic_visitor, TInjector, basic_visitor_types) {
    TInjector injector;

    fake_visitor<
        mpl::vector<
            transaction_usage
          , shared_ptr<provider<shared_ptr<transaction>>>
          , shared_ptr<c3>
          , int
        >
    > visitor;

    injector.template visit<transaction_usage>(visitor);
}

using basic_call_types = mpl::vector<
    injector<injector_custom_scope_t>
  , injector<decltype(injector_custom_scope)>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(basic_call, TInjector, basic_call_types) {
    fake_scope<>::entry_calls() = 0;
    fake_scope<>::exit_calls() = 0;

    TInjector injector;

    BOOST_CHECK(!injector.template create<shared_ptr<c3>>());

    injector.call(fake_scope_entry());
    BOOST_CHECK(injector.template create<shared_ptr<c3>>());

    injector.call(fake_scope_exit());
    BOOST_CHECK(!injector.template create<shared_ptr<c3>>());
}

BOOST_AUTO_TEST_CASE(basic_injector_externals) {
    injector<decltype(injector_externals_2)> injector_(injector_externals_2);
    auto c9_ = injector_.create<shared_ptr<c9>>();

    BOOST_CHECK_EQUAL(42, c9_->i);
    BOOST_CHECK_EQUAL(87.0, c9_->d);
}

BOOST_AUTO_TEST_CASE(externals_mix) {
    injector<
        decltype(injector_externals_1)
      , decltype(injector_externals_2)
    > injector_(injector_externals_2, injector_externals_1);

    auto c8_ = injector_.create<shared_ptr<c8>>();

    BOOST_CHECK_EQUAL(4, c8_->i);
    BOOST_CHECK_EQUAL(3, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(42, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(42, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(87.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
    BOOST_CHECK_EQUAL(67, dynamic_cast<c3if0&>(*c8_->c7_->c6_->c5_.if0_).i);
    BOOST_CHECK_EQUAL(78.0, dynamic_cast<c3if0&>(*c8_->c7_->c6_->c5_.if0_).d);
    BOOST_CHECK_EQUAL(67, dynamic_cast<c3if0&>(*c8_->c7_->if0_).i);
}

BOOST_AUTO_TEST_CASE(ctor) {
    injector<decltype(injector_1)> injector(injector_1);
    check(injector.create<c8>());
}

BOOST_AUTO_TEST_CASE(ctor_mix) {
    injector<injector_2_t, decltype(injector_2)> injector(injector_2);
    check(injector.create<shared_ptr<c8>>());
}

BOOST_AUTO_TEST_CASE(ctor_mix_order) {
    injector<decltype(injector_2), injector_2_t> injector(injector_2);
    check(injector.create<shared_ptr<c8>>());
}

BOOST_AUTO_TEST_CASE(ctor_mix_explicit) {
    injector<
        injector_2_t
      , decltype(injector_2)
    > injector(injector_2_t(), injector_2);
    check(injector.create<shared_ptr<c8>>());
}

BOOST_AUTO_TEST_CASE(ctor_mix_explicit_order) {
    injector<
        decltype(injector_2)
      , injector_2_t
    > injector(injector_2, injector_2_t());
    check(injector.create<shared_ptr<c8>>());
}

BOOST_AUTO_TEST_CASE(install) {
    injector<> injector_;
    check(injector_(injector_2_t(), injector_3_t()).create<shared_ptr<c8>>());
}

BOOST_AUTO_TEST_CASE(install_mix) {
    injector<> injector_;
    check(injector_(injector_2_t(), injector_2).create<shared_ptr<c8>>());
}

BOOST_AUTO_TEST_CASE(pre_installed_injector_install_injector_type) {
    injector<decltype(injector_2)> injector_(injector_2);
    check(injector_(injector_2_t()).create<shared_ptr<c8>>());
}

BOOST_AUTO_TEST_CASE(pre_installed_injector_install_injector) {
    injector<injector_2_t> injector_;
    check(injector_(injector_2).create<shared_ptr<c8>>());
}

BOOST_AUTO_TEST_CASE(scope_deduction) {
    auto c19_ = injector<>().create<shared_ptr<c19>>();
    BOOST_CHECK(c19_->c1_ == c19_->c1__);
}

BOOST_AUTO_TEST_CASE(scope_deduction_named_shared_ptr) {
    auto c22_ = injector<>().create<shared_ptr<c22>>();
    BOOST_CHECK(c22_->i == c22_->c11_->i);
}

using deduce_injectors_types = mpl::vector<
    injector<injector_c0if0_t>
  , injector<decltype(injector_c0if0)>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(scope_deduction_if, TInjector, deduce_injectors_types) {
    TInjector injector;
    auto c20_ = injector.template create<shared_ptr<c20>>();
    BOOST_CHECK(c20_->if0_ == c20_->if0__);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(shared_ptr_unique_ptr, TInjector, deduce_injectors_types) {
    TInjector injector;
    auto c21_ = injector.template create<shared_ptr<c21>>();
    BOOST_CHECK(c21_->if0__ == c21_->if0__);
}

BOOST_AUTO_TEST_CASE(smart_ptr_weak_ptr) {
    auto c25_ = injector<>().create<c25>();
    BOOST_CHECK(nullptr != c25_.w1_.lock());
}

BOOST_AUTO_TEST_CASE(session_scope) {
    injector<
        session<c0if0>
    > injector_;

    {
    auto c20_ = injector_.create<c20>();
    BOOST_CHECK(nullptr == c20_.if0_.get());
    BOOST_CHECK(nullptr == c20_.if0__.get());
    }

    {
    injector_.call(scopes::session_entry());
    auto c20_ = injector_.create<c20>();
    BOOST_CHECK(dynamic_cast<c0if0*>(c20_.if0_.get()));
    BOOST_CHECK(dynamic_cast<c0if0*>(c20_.if0__.get()));
    BOOST_CHECK(c20_.if0_ == c20_.if0__);
    }

    {
    injector_.call(scopes::session_exit());
    auto c20_ = injector_.create<c20>();
    BOOST_CHECK(nullptr == c20_.if0_.get());
    BOOST_CHECK(nullptr == c20_.if0__.get());
    }
}

BOOST_AUTO_TEST_CASE(per_thread_scope_singleton) {
    std::vector<shared_ptr<c20>> v;
    mutex m;

    injector<
        per_thread<
            scoped<c0if0>
        >
    > injector_;

    thread t1([&]{
        scoped_lock l(m);
        v.push_back(injector_.create<shared_ptr<c20>>());
    });

    thread t2([&]{
        scoped_lock l(m);
        v.push_back(injector_.create<shared_ptr<c20>>());
    });
    t1.join();
    t2.join();

    BOOST_CHECK_EQUAL(2, v.size());

    BOOST_CHECK(dynamic_cast<c0if0*>(v[0]->if0_.get()));
    BOOST_CHECK(dynamic_cast<c0if0*>(v[0]->if0__.get()));
    BOOST_CHECK(v[0]->if0_ == v[0]->if0__);

    BOOST_CHECK(dynamic_cast<c0if0*>(v[1]->if0_.get()));
    BOOST_CHECK(dynamic_cast<c0if0*>(v[1]->if0__.get()));
    BOOST_CHECK(v[1]->if0_ == v[1]->if0__);

    BOOST_CHECK(v[0]->if0_ != v[1]->if0_);
    BOOST_CHECK(v[0]->if0__ != v[1]->if0__);
}
#endif

BOOST_AUTO_TEST_CASE(bind_vector_value_and_smart_ptr) {
    using injector_t = injector<
        bind_vector<if0, c0if0, c1if0, c2if0>
      , bind_vector<int, int_<4>, int_<2>>
    >;

    auto c26_ = injector_t().create<c26>();

/*    BOOST_CHECK_EQUAL(3, c26_.v1_.size());*/
    //BOOST_CHECK_EQUAL(2, c26_.v2_.size());
    //BOOST_CHECK_EQUAL(4, c26_.v2_[0]);
    /*BOOST_CHECK_EQUAL(2, c26_.v2_[1]);*/
}

} // namespace di
} // namespace boost

