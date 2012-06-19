//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "fake_config.hpp"

#include "boost/di/injector.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/vector.hpp>

#include "boost/di/generic_module.hpp"
#include "boost/di/fusion_module.hpp"
#include "fake_visitor.hpp"
#include "fake_scope.hpp"
#include "data.hpp"

namespace boost {
namespace di {

typedef generic_module<
    singletons<
        c3
    >
  , per_requests<
        c0if0
      , bind<c1if0>::in_call<call_stack<c6, c5> >
      , bind<c2if0>::in_call<c7>
      , bind<int, mpl::int_<1> >
      , bind<int, mpl::int_<2> >::in_call<c8>
      , bind<int, mpl::int_<3> >::in_name<mpl::string<'1'> >::in_call<call_stack<c7, c6, c4> >
      , bind<int, mpl::int_<4> >::in_name<mpl::string<'2'> >::in_call<call_stack<c7, c6, c4> >
      , bind<int, mpl::int_<5> >::in_call<c2>
    >
> generic_module_1;

typedef generic_module<
    singletons<
        c3
    >
  , per_requests<
        bind<int, mpl::int_<0> >::in_name<mpl::string<'1'> >
      , bind<int, mpl::int_<1> >
    >
> generic_module_2;

typedef generic_module<
    singletons<
        c0if0
    >
  , per_requests<
        bind<int, mpl::int_<2> >::in_call<c8>
      , bind<int, mpl::int_<3> >::in_name<mpl::string<'2'> >
    >
> generic_module_3;

typedef generic_module<
    scope<fake_scope>::bind<c3>
> generic_module_custom_scope;

typedef generic_module<
    per_requests<
        transaction_provider
      , mpl::int_<0>
    >
> generic_module_provider;

typedef generic_module<
    singletons<
        c0if0
    >
  , externals<
        int
      , double
    >
> generic_module_externals;

struct generic_module_externals_ctor : generic_module<
    singletons<
        c0if0
    >
  , externals<
        int
      , double
    >
>
{
    generic_module_externals_ctor(int i, double d)
        : generic_module(set<int>(i), set<double>(d))
    { }
};

typedef generic_module<
    bind<if0, c3if0>
  , externals<
        double
      , if0
      , annotate<bind<int>::in_name<mpl::string<'1'> >::in_call<call_stack<c7, c6, c4> > >::with<a>
      , annotate<bind<int>::in_call<c8> >::with<b>
    >
> generic_module_externals_1;

BOOST_AUTO(fusion_module_1, fusion_module<>()(
    singletons<
        c3
    >()
  , per_requests<
        c0if0
      , bind<c1if0>::in_call<call_stack<c6, c5> >
      , bind<c2if0>::in_call<c7>
      , bind<int, mpl::int_<1> >
      , bind<int, mpl::int_<2> >::in_call<c8>
      , bind<int, mpl::int_<3> >::in_name<mpl::string<'1'> >::in_call<call_stack<c7, c6, c4> >
      , bind<int, mpl::int_<4> >::in_name<mpl::string<'2'> >::in_call<call_stack<c7, c6, c4> >
      , bind<int, mpl::int_<5> >::in_call<c2>
    >()
));

BOOST_AUTO(fusion_module_2, fusion_module<>()(
    singletons<
        c0if0
    >()
  , per_requests<
        bind<int, mpl::int_<2> >::in_call<c8>
      , bind<int, mpl::int_<3> >::in_name<mpl::string<'2'> >
    >()
));

BOOST_AUTO(fusion_module_3, fusion_module<>()(
    singletons<
        c3
    >()
  , per_requests<
        bind<int, mpl::int_<0> >::in_name<mpl::string<'1'> >
      , bind<int, mpl::int_<1> >
    >()
));

BOOST_AUTO(fusion_module_custom_scope, fusion_module<>()(
    scope<fake_scope>::bind<c3>()
));

BOOST_AUTO(fusion_module_provider, fusion_module<>()(
    per_requests<
        transaction_provider, mpl::int_<0>
    >()
));

BOOST_AUTO(fusion_module_externals, fusion_module<>()(
    singletons<
        c0if0
    >()
  , bind<int>::to(42)
  , bind<double>::to(87.0)
));

BOOST_AUTO(fusion_module_externals_1, fusion_module<>()(
    bind<int>::to(42)
  , bind<int>::in_call<c2>::to(87)
));

void check(const c8& c8_) {
    BOOST_CHECK(c8_.c1_ != c8_.c7_->c6_->c5_.c1_);
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
    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
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

typedef mpl::vector<
    injector<generic_module_1>
  , injector<BOOST_TYPEOF(fusion_module_1)>
> one_module_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(one_module, TInjector, one_module_types) {
    TInjector injector;
    check(injector.template create<c8>());
}

typedef mpl::vector<
    injector<generic_module_2, generic_module_3>
  , injector<generic_module_3, generic_module_2>
  , injector<BOOST_TYPEOF(fusion_module_2), BOOST_TYPEOF(fusion_module_3)>
  , injector<BOOST_TYPEOF(fusion_module_3), BOOST_TYPEOF(fusion_module_2)>
> many_modules_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(many_modules, TInjector, many_modules_types) {
    TInjector injector;
    check(injector.template create<shared_ptr<c8> >());
}

typedef mpl::vector<
    injector<generic_module_2, BOOST_TYPEOF(fusion_module_2)>
  , injector<BOOST_TYPEOF(fusion_module_2), generic_module_2>
> mix_modules_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(mix_modules, TInjector, mix_modules_types) {
    TInjector injector;

    check(injector.template create<shared_ptr<c8> >());
}

typedef mpl::vector<
    injector<generic_module_provider>
  , injector<BOOST_TYPEOF(fusion_module_provider)>
> basic_provider_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(basic_provider, TInjector, basic_provider_types) {
    TInjector injector;
    transaction_usage obj = injector.template create<transaction_usage>();
    BOOST_CHECK(obj.p->get().get() != obj.p->get().get());
}

typedef mpl::vector<
    injector<generic_module_provider>
  , injector<BOOST_TYPEOF(fusion_module_provider)>
> basic_visitor_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(basic_visitor, TInjector, basic_visitor_types) {
    TInjector injector;

    fake_visitor<
        mpl::vector<
            transaction_usage
          , shared_ptr<provider<shared_ptr<transaction> > >
          , shared_ptr<c3>
          , int
        >
    > visitor;

    injector.template visit<transaction_usage>(visitor);
}

typedef mpl::vector<
    injector<generic_module_custom_scope>
  , injector<BOOST_TYPEOF(fusion_module_custom_scope)>
> basic_call_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(basic_call, TInjector, basic_call_types) {
    fake_scope::entry_calls() = 0;
    fake_scope::exit_calls() = 0;

    TInjector injector;

    BOOST_CHECK(!injector.template create<shared_ptr<c3> >());

    injector.template call<fake_scope>(fake_scope::entry());
    BOOST_CHECK(injector.template create<shared_ptr<c3> >());

    injector.template call<fake_scope>(fake_scope::exit());
    BOOST_CHECK(!injector.template create<shared_ptr<c3> >());
}

BOOST_AUTO_TEST_CASE(basic_generic_module_externals) {
    injector<generic_module_externals> injector_(
        generic_module_externals(
            generic_module_externals::set<int>(42)
          , generic_module_externals::set<double>(87.0)
        )
    );

    shared_ptr<c9> c9_ = injector_.create<shared_ptr<c9> >();

    BOOST_CHECK_EQUAL(42, c9_->i);
    BOOST_CHECK_EQUAL(87.0, c9_->d);
}

BOOST_AUTO_TEST_CASE(ctor_generic_module_externals) {
    injector<generic_module_externals_ctor> injector_(
        generic_module_externals_ctor(42, 87.0)
    );

    shared_ptr<c9> c9_ = injector_.create<shared_ptr<c9> >();

    BOOST_CHECK_EQUAL(42, c9_->i);
    BOOST_CHECK_EQUAL(87.0, c9_->d);
}

BOOST_AUTO_TEST_CASE(basic_fusion_module_externals) {
    injector<BOOST_TYPEOF(fusion_module_externals)> injector_(fusion_module_externals);
    shared_ptr<c9> c9_ = injector_.create<shared_ptr<c9> >();

    BOOST_CHECK_EQUAL(42, c9_->i);
    BOOST_CHECK_EQUAL(87.0, c9_->d);
}

BOOST_AUTO_TEST_CASE(externals_mix) {
    shared_ptr<c3if0> c3if0_(new c3if0(67, 78.0));

    typedef generic_module_externals_1 generic_module;

    injector<
        BOOST_TYPEOF(fusion_module_externals_1)
      , generic_module_externals_1
    > injector_(
        generic_module(
            generic_module::set<a>(3.0)
          , generic_module::set<b>(4.0)
          , generic_module::set<if0>(c3if0_)
          , generic_module::set<double>(7.0)
        )
      , fusion_module_externals_1
    );

    shared_ptr<c8> c8_ = injector_.create<shared_ptr<c8> >();

    //TODO
    //BOOST_CHECK_EQUAL(4, c8_->i);
    //BOOST_CHECK_EQUAL(3, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(42, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(42, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(7.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
    BOOST_CHECK_EQUAL(67, dynamic_cast<c3if0&>(*c8_->c7_->c6_->c5_.if0_).i);
    BOOST_CHECK_EQUAL(78.0, dynamic_cast<c3if0&>(*c8_->c7_->c6_->c5_.if0_).d);
    BOOST_CHECK_EQUAL(67, dynamic_cast<c3if0&>(*c8_->c7_->if0_).i);
}

BOOST_AUTO_TEST_CASE(ctor) {
    injector<BOOST_TYPEOF(fusion_module_1)> injector(fusion_module_1);
    check(injector.create<c8>());
}

BOOST_AUTO_TEST_CASE(ctor_mix) {
    injector<generic_module_2, BOOST_TYPEOF(fusion_module_2)> injector(fusion_module_2);
    check(injector.create<shared_ptr<c8> >());
}

BOOST_AUTO_TEST_CASE(ctor_mix_order) {
    injector<BOOST_TYPEOF(fusion_module_2), generic_module_2> injector(fusion_module_2);
    check(injector.create<shared_ptr<c8> >());
}

BOOST_AUTO_TEST_CASE(ctor_mix_explicit) {
    injector<
        generic_module_2
      , BOOST_TYPEOF(fusion_module_2)
    > injector(generic_module_2(), fusion_module_2);
    check(injector.create<shared_ptr<c8> >());
}

BOOST_AUTO_TEST_CASE(ctor_mix_explicit_order) {
    injector<
        BOOST_TYPEOF(fusion_module_2)
      , generic_module_2
    > injector(fusion_module_2, generic_module_2());
    check(injector.create<shared_ptr<c8> >());
}

BOOST_AUTO_TEST_CASE(install) {
    injector<> injector_;
    check(injector_.install(generic_module_2(), generic_module_3()).create<shared_ptr<c8> >());
}

BOOST_AUTO_TEST_CASE(install_mix) {
    injector<> injector_;
    check(injector_.install(generic_module_2(), fusion_module_2).create<shared_ptr<c8> >());
}

BOOST_AUTO_TEST_CASE(pre_installed_generic_module_install_fusion_module) {
    injector<generic_module_2> injector_;
    check(injector_.install(fusion_module_2).create<shared_ptr<c8> >());
}

BOOST_AUTO_TEST_CASE(pre_installed_fusion_module_install_generic_module) {
    injector<BOOST_TYPEOF(fusion_module_2)> injector_(fusion_module_2);
    check(injector_.install<generic_module_2>().create<shared_ptr<c8> >());
}

} // namespace di
} // namespace boost

