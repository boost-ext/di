//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/injector.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/make_shared.hpp>
#include <boost/mpl/vector.hpp>

#include "boost/di/make_injector.hpp"

#include "fake_visitor.hpp"
#include "fake_scope.hpp"
#include "data.hpp"

namespace boost {
namespace di {

using injector_1 = injector<
    singleton<
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

using injector_2 = injector<
    singleton<
        c3
    >
  , per_request<
        bind_int<0>::in_name<mpl::string<'1'>>
      , bind_int<1>
    >
>;

using injector_3 = injector<
    singleton<
        c0if0
    >
  , per_request<
        bind_int<2>::in_call<c8>
      , bind_int<3>::in_name<mpl::string<'2'>>
    >
>;

using injector_custom_scope = injector<
    scope<fake_scope>::bind<c3>
>;

using injector_provider = injector<
    per_request<
        transaction_provider
      , mpl::int_<0>
    >
>;

using injector_c0if0 = injector<
    c0if0
> ;

auto injector2_1 = make_injector(
    singleton<
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

auto injector2_2 = make_injector(
    singleton<
        c0if0
    >()
  , per_request<
        bind_int<2>::in_call<c8>
      , bind_int<3>::in_name<mpl::string<'2'>>
    >()
);

auto injector2_3 = make_injector(
    singleton<
        c3
    >()
  , per_request<
        bind_int<0>::in_name<mpl::string<'1'>>
      , bind_int<1>
    >()
);

auto injector2_custom_scope = make_injector(
    scope<fake_scope>::bind<c3>()
);

auto injector2_provider = make_injector(
    per_request<
        transaction_provider, mpl::int_<0>
    >()
);

auto injector_externals = make_injector(
    bind<double>::to(7.0)
  , bind<if0>::to(boost::make_shared<c3if0>(67, 78.0))
  , bind<int>::in_name<mpl::string<'1'>>::in_call<call_stack<c7, c6, c4>>::to(3)
  , bind<int>::in_call<c8>::to(4)
);

auto injector2_externals = make_injector(
    singleton<
        c0if0
    >()
  , bind<int>::to(42)
  , bind<double>::to(87.0)
);

auto injector2_externals_1 = make_injector(
    bind<int>::to(42)
  , bind<int>::in_call<c2>::to(87)
);

auto injector2_c0if0 = make_injector(
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
    injector<injector_1>
  , injector<decltype(injector2_1)>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(one_injector, TInjector, one_injector_types) {
    TInjector injector;
    check(injector.template create<c8>());
}

using many_injectors_types = mpl::vector<
    injector<injector_2, injector_3>
  , injector<injector_3, injector_2>
  , injector<decltype(injector2_2), decltype(injector2_3)>
  , injector<decltype(injector2_3), decltype(injector2_2)>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(many_injectors, TInjector, many_injectors_types) {
    TInjector injector;
    check(injector.template create<shared_ptr<c8>>());
}

using mix_injectors_types = mpl::vector<
    injector<injector_2, decltype(injector2_2)>
  , injector<decltype(injector2_2), injector_2>
> ;

BOOST_AUTO_TEST_CASE_TEMPLATE(mix_injectors, TInjector, mix_injectors_types) {
    TInjector injector;
    check(injector.template create<shared_ptr<c8>>());
}

using basic_provider_types = mpl::vector<
    injector<injector_provider>
  , injector<decltype(injector2_provider)>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(basic_provider, TInjector, basic_provider_types) {
    TInjector injector;
    transaction_usage obj = injector.template create<transaction_usage>();
    BOOST_CHECK(obj.p->get().get() != obj.p->get().get());
}

using basic_visitor_types = mpl::vector<
    injector<injector_provider>
  , injector<decltype(injector2_provider)>
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
    injector<injector_custom_scope>
  , injector<decltype(injector2_custom_scope)>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(basic_call, TInjector, basic_call_types) {
    fake_scope::entry_calls() = 0;
    fake_scope::exit_calls() = 0;

    TInjector injector;

    BOOST_CHECK(!injector.template create<shared_ptr<c3>>());

    injector.template call<fake_scope>(fake_scope::entry());
    BOOST_CHECK(injector.template create<shared_ptr<c3>>());

    injector.template call<fake_scope>(fake_scope::exit());
    BOOST_CHECK(!injector.template create<shared_ptr<c3>>());
}

BOOST_AUTO_TEST_CASE(basic_injector2_externals) {
    injector<decltype(injector2_externals)> injector_(injector2_externals);
    shared_ptr<c9> c9_ = injector_.create<shared_ptr<c9>>();

    BOOST_CHECK_EQUAL(42, c9_->i);
    BOOST_CHECK_EQUAL(87.0, c9_->d);
}

#if 0
BOOST_AUTO_TEST_CASE(externals_mix) {
    injector<
        decltype(injector_externals)
      , decltype(injector2_externals)
    > injector_(injector2_externals, injector_externals);

    shared_ptr<c8> c8_ = injector_.create<shared_ptr<c8>>();

    BOOST_CHECK_EQUAL(4, c8_->i);
    BOOST_CHECK_EQUAL(3, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(42, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(87, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(7.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
    BOOST_CHECK_EQUAL(67, dynamic_cast<c3if0&>(*c8_->c7_->c6_->c5_.if0_).i);
    BOOST_CHECK_EQUAL(78.0, dynamic_cast<c3if0&>(*c8_->c7_->c6_->c5_.if0_).d);
    BOOST_CHECK_EQUAL(67, dynamic_cast<c3if0&>(*c8_->c7_->if0_).i);
}
#endif

BOOST_AUTO_TEST_CASE(ctor) {
    injector<decltype(injector2_1)> injector(injector2_1);
    check(injector.create<c8>());
}

/*BOOST_AUTO_TEST_CASE(ctor_mix) {*/
    //injector<injector_2, decltype(injector2_2)> injector(injector2_2);
    //check(injector.create<shared_ptr<c8>>());
//}

//BOOST_AUTO_TEST_CASE(ctor_mix_order) {
    //injector<decltype(injector2_2), injector_2> injector(injector2_2);
    //check(injector.create<shared_ptr<c8>>());
/*}*/

BOOST_AUTO_TEST_CASE(ctor_mix_explicit) {
    injector<
        injector_2
      , decltype(injector2_2)
    > injector(injector_2(), injector2_2);
    check(injector.create<shared_ptr<c8>>());
}

BOOST_AUTO_TEST_CASE(ctor_mix_explicit_order) {
    injector<
        decltype(injector2_2)
      , injector_2
    > injector(injector2_2, injector_2());
    check(injector.create<shared_ptr<c8>>());
}

BOOST_AUTO_TEST_CASE(install) {
    injector<> injector_;
    check(injector_(injector_2(), injector_3()).create<shared_ptr<c8>>());
}

/*BOOST_AUTO_TEST_CASE(install_mix) {*/
    //injector<> injector_;
    //check(injector_(injector_2(), injector2_2).create<shared_ptr<c8>>());
//}

//BOOST_AUTO_TEST_CASE(pre_installed_injector_install_injector2) {
    //injector<injector_2> injector_;
    //check(injector_(injector2_2).create<shared_ptr<c8>>());
/*}*/

//BOOST_AUTO_TEST_CASE(pre_installed_injector2_install_injector) {
    //injector<decltype(injector2_2)> injector_(injector2_2);
    //check(injector_.install<injector_2>().create<shared_ptr<c8>>());
//}

BOOST_AUTO_TEST_CASE(scope_deduction) {
    shared_ptr<c19> c19_ = injector<>().create<shared_ptr<c19>>();
    BOOST_CHECK(c19_->c1_ == c19_->c1__);
}

using deduce_injectors_types = mpl::vector<
    injector<injector_c0if0>
  , injector<decltype(injector2_c0if0)>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(scope_deduction_if, TInjector, deduce_injectors_types) {
    TInjector injector;
    shared_ptr<c20> c20_ = injector.template create<shared_ptr<c20>>();
    BOOST_CHECK(c20_->if0_ == c20_->if0__);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(std_shared_ptr_std_unique_ptr, TInjector, deduce_injectors_types) {
    TInjector injector;
    std::shared_ptr<c21> c21_ = injector.template create<std::shared_ptr<c21>>();
    BOOST_CHECK(c21_->if0__ == c21_->if0__);
}

} // namespace di
} // namespace boost

