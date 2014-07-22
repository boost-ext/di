//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/injector.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/vector.hpp>

#include "boost/di/make_injector.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/config.hpp"

#include "common/fakes/fake_visitor.hpp"
#include "common/fakes/fake_scope.hpp"
#include "common/data.hpp"

namespace boost {
namespace di {

using injector_1_t = injector<
    shared<
        c3
    >
  , unique<
        c0if0
      , bind<c1if0>::when<call_stack<c6, c5>>
      , bind<c2if0>::when<call_stack<c7>>
      , bind_int<1>
      , bind_int<2>::when<call_stack<c8>>
      , bind_int<3>::named<mpl::string<'1'>>::when<call_stack<c7, c6, c4>>
      , bind_int<4>::named<mpl::string<'2'>>::when<call_stack<c7, c6, c4>>
      , bind_int<5>::when<call_stack<c2>>
      , bind_bool<true>
    >
>;

using injector_2_t = injector<
    shared<
        c3
    >
  , scope<scopes::unique<>>::bind<
        bind_int<0>::named<mpl::string<'1'>>
      , bind_int<1>
      , bind_bool<true>
    >
>;

using injector_3_t = injector<
    shared<
        c0if0
    >
  , unique<
        bind_int<2>::when<call_stack<c8>>
      , bind_int<3>::named<mpl::string<'2'>>
    >
>;

using injector_custom_scope_t = injector<
    scope<fake_scope<>>::bind<
        c3
      , int
    >
>;

using injector_provider_t = injector<
    unique<
        transaction_provider
    >
>;

using injector_c0if0_t = injector<
    c0if0
>;

auto injector_1 = make_injector(
    shared<
        c3
    >()
  , unique<
        c0if0
      , bind<c1if0>::when<call_stack<c6, c5>>
      , bind<c2if0>::when<call_stack<c7>>
      , bind_int<1>
      , bind_int<2>::when<call_stack<c8>>
      , bind_int<3>::named<mpl::string<'1'>>::when<call_stack<c7, c6, c4>>
      , bind_int<4>::named<mpl::string<'2'>>::when<call_stack<c7, c6, c4>>
      , bind_int<5>::when<call_stack<c2>>
      , bind_bool<true>
    >()
);

auto injector_2 = make_injector(
    shared<
        c0if0
    >()
  , unique<
        bind_int<2>::when<call_stack<c8>>
      , bind_int<3>::named<mpl::string<'2'>>
    >()
);

auto injector_3 = make_injector(
    shared<
        c3
    >()
  , unique<
        bind_int<0>::named<mpl::string<'1'>>
      , bind_int<1>
      , bind_bool<true>
    >()
);

auto injector_custom_scope = make_injector(
    scope<fake_scope<>>::bind<c3, int>()
);

auto injector_provider = make_injector(
    unique<
        transaction_provider, mpl::int_<0>
    >()
);

auto injector_externals = make_injector(
    bind<double>::to(7.0)
  , bind<if0>::to(aux::shared_ptr<c3if0>(new c3if0(67, 78.0)))
  , bind<int>::named<mpl::string<'1'>>::when<call_stack<c7, c6, c4>>::to(3)
  , bind<int>::when<call_stack<c8>>::to(4)
);

auto injector_externals_1 = make_injector(
    bind<if0>::to(aux::shared_ptr<c3if0>(new c3if0(67, 78.0)))
  , bind<int>::named<mpl::string<'1'>>::when<call_stack<c7, c6, c4>>::to(3)
  , bind<int>::when<call_stack<c8>>::to(4)
);

auto injector_externals_2 = make_injector(
    shared<
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
    BOOST_CHECK_EQUAL(true, c8_.c7_->c6_->c5_.c2_->b);
}

void check(const aux::shared_ptr<c8>& c8_) {
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
    BOOST_CHECK_EQUAL(true, c8_->c7_->c6_->c5_.c2_->b);
}

using one_injector_types = mpl::vector<
    injector<injector_1_t>
  , injector<decltype(injector_1)>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(one_injector, TInjector, one_injector_types) {
    TInjector injector;
    check(injector.template create<c8>());
}

using multiple_injectors_types = mpl::vector<
    injector<injector_2_t, injector_3_t>
  , injector<injector_3_t, injector_2_t>
  , injector<decltype(injector_2), decltype(injector_3)>
  , injector<decltype(injector_3), decltype(injector_2)>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(multiple_injectors, TInjector, multiple_injectors_types) {
    TInjector injector;
    check(injector.template create<aux::shared_ptr<c8>>());
}

using mix_injectors_types = mpl::vector<
    injector<injector_2_t, decltype(injector_2)>
  , injector<decltype(injector_2), injector_2_t>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(mix_injectors, TInjector, mix_injectors_types) {
    TInjector injector;
    check(injector.template create<aux::shared_ptr<c8>>());
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
          , aux::shared_ptr<provider<aux::shared_ptr<transaction>>>
          , aux::shared_ptr<c3>
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

    BOOST_CHECK(!injector.template create<aux::shared_ptr<c3>>());

    injector.call(fake_scope_entry());
    BOOST_CHECK(injector.template create<aux::shared_ptr<c3>>());

    injector.call(fake_scope_exit());
    BOOST_CHECK(!injector.template create<aux::shared_ptr<c3>>());
}

BOOST_AUTO_TEST_CASE(basic_injector_externals) {
    injector<decltype(injector_externals_2)> injector_(injector_externals_2);
    auto c9_ = injector_.create<aux::shared_ptr<c9>>();

    BOOST_CHECK_EQUAL(42, c9_->i);
    BOOST_CHECK_EQUAL(87.0, c9_->d);
}

BOOST_AUTO_TEST_CASE(externals_priority) {
    const int i = 87;

    auto injector = make_injector(
        bind_int<42>()
      , bind<int>::to(i)
    );

    BOOST_CHECK_EQUAL(i, injector.create<c3>().i);
}

BOOST_AUTO_TEST_CASE(externals_mix) {
    injector<
        decltype(injector_externals_1)
      , decltype(injector_externals_2)
    > injector_(injector_externals_2, injector_externals_1);

    auto c8_ = injector_.create<aux::shared_ptr<c8>>();

    BOOST_CHECK_EQUAL(4, c8_->i);
    BOOST_CHECK_EQUAL(3, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(42, c8_->c7_->c6_->c4_->i2);
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
    check(injector.create<aux::shared_ptr<c8>>());
}

BOOST_AUTO_TEST_CASE(ctor_mix_order) {
    injector<decltype(injector_2), injector_2_t> injector(injector_2);
    check(injector.create<aux::shared_ptr<c8>>());
}

BOOST_AUTO_TEST_CASE(ctor_mix_explicit) {
    injector<
        injector_2_t
      , decltype(injector_2)
    > injector(injector_2_t(), injector_2);
    check(injector.create<aux::shared_ptr<c8>>());
}

BOOST_AUTO_TEST_CASE(ctor_mix_explicit_order) {
    injector<
        decltype(injector_2)
      , injector_2_t
    > injector(injector_2, injector_2_t());
    check(injector.create<aux::shared_ptr<c8>>());
}

BOOST_AUTO_TEST_CASE(install) {
    injector<injector_2_t, injector_3_t> injector_((injector_2_t(), injector_3_t()));
    check(injector_.create<aux::shared_ptr<c8>>());
}

BOOST_AUTO_TEST_CASE(install_mix) {
    injector<injector_2_t, decltype(injector_2)> injector_((injector_2_t(), injector_2));
    check(injector_.create<aux::shared_ptr<c8>>());
}

BOOST_AUTO_TEST_CASE(scope_deduction) {
    auto c19_ = injector<>().create<aux::shared_ptr<c19>>();
    BOOST_CHECK(c19_->c1_ == c19_->c1__);
}

BOOST_AUTO_TEST_CASE(scope_deduction_named_shared_ptr) {
    auto c22_ = injector<>().create<aux::shared_ptr<c22>>();
    BOOST_CHECK(c22_->i == c22_->c11_->i);
}

using deduce_injectors_types = mpl::vector<
    injector<injector_c0if0_t>
  , injector<decltype(injector_c0if0)>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(scope_deduction_if, TInjector, deduce_injectors_types) {
    TInjector injector;
    auto c20_ = injector.template create<aux::shared_ptr<c20>>();
    BOOST_CHECK(c20_->if0_ == c20_->if0__);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(shared_ptr_unique_ptr, TInjector, deduce_injectors_types) {
    TInjector injector;
    auto c21_ = injector.template create<aux::shared_ptr<c21>>();
    BOOST_CHECK(c21_->if0__ == c21_->if0__);
}

BOOST_AUTO_TEST_CASE(smart_ptr_weak_ptr) {
    auto c25_ = injector<>().create<c25>();
    BOOST_CHECK(nullptr != c25_.w1_.lock());
}

using creation_special_cases_types = mpl::vector<
    noncopyable_const_ref
#if !defined(BOOST_MSVC)
  , rvalue
#endif
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(creation_special_cases, T, creation_special_cases_types) {
    const int i = 42;
    auto obj_ = injector<bind_int<i>>().create<T>();
    BOOST_CHECK_EQUAL(i, obj_.i_);
}

BOOST_AUTO_TEST_CASE(stored_ref_created_by_injector) {
    const int i = 42;
    auto ref_sp_int_ = injector<bind_int<i>>().create<ref_sp_int>();
    BOOST_CHECK(ref_sp_int_.i_);
}

BOOST_AUTO_TEST_CASE(smart_ptr_auto_ptr) {
    const int i = 42;
    auto auto_ptr_int_ = injector<bind_int<i>>().create<aux::auto_ptr<auto_ptr_int>>();
    BOOST_CHECK_EQUAL(i, *auto_ptr_int_->i_);
}

using bind_multiple_interfaces_to_the_same_impl_types = mpl::vector<
    cif0if1
  , bind<any_of<if0, if1>, cif0if1>
  , deduce<cif0if1>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(bind_multiple_interfaces_to_the_same_impl, T, bind_multiple_interfaces_to_the_same_impl_types) {
    auto multiple_interfaces_ = injector<T>().template create<multiple_interfaces>();

    BOOST_CHECK(dynamic_cast<cif0if1*>(multiple_interfaces_.if0_.get()));
    BOOST_CHECK(dynamic_cast<cif0if1*>(multiple_interfaces_.if1_.get()));
    BOOST_CHECK(
        static_cast<cif0if1*>(multiple_interfaces_.if0_.get())
        ==
        static_cast<cif0if1*>(multiple_interfaces_.if1_.get())
    );
}

BOOST_AUTO_TEST_CASE(bind_multiple_interfaces_to_the_same_impl_without_any_of) {
    auto multiple_interfaces_ = injector<
        bind<if0, cif0if1>
      , bind<if1, cif0if1>
    >().create<multiple_interfaces>();

    BOOST_CHECK(dynamic_cast<cif0if1*>(multiple_interfaces_.if0_.get()));
    BOOST_CHECK(dynamic_cast<cif0if1*>(multiple_interfaces_.if1_.get()));
    BOOST_CHECK(
        static_cast<cif0if1*>(multiple_interfaces_.if0_.get())
        !=
        static_cast<cif0if1*>(multiple_interfaces_.if1_.get())
    );
}

BOOST_AUTO_TEST_CASE(inject_priority) {
    const int i = 42;
    const double d = 87.0;
    const float f = 12.0;

    auto c28_ = make_injector(
        bind<int>::to(i)
      , bind<double>::to(d)
      , bind<float>::to(f)
    ).create<c28>();

    BOOST_CHECK_EQUAL(i, c28_.i_);
    BOOST_CHECK_EQUAL(0.0, c28_.d_);
    BOOST_CHECK_EQUAL(0.0, c28_.f_);
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

BOOST_AUTO_TEST_CASE(scoped_injector_create) {
    aux::shared_ptr<int> i1;
    aux::shared_ptr<int> i2;
    aux::shared_ptr<int> i3;

    {
    injector<shared<int>> i;

    i1 = i.create<aux::shared_ptr<int>>();
    i2 = i.create<aux::shared_ptr<int>>();

    BOOST_CHECK(i1 == i2);
    }

    {
    injector<shared<int>> i;

    i3 = i.create<aux::shared_ptr<int>>();

    BOOST_CHECK(i3 != i1);
    BOOST_CHECK(i3 != i2);
    }
}

BOOST_AUTO_TEST_CASE(scoped_injector_create_with_deduced_scope) {
    aux::shared_ptr<c27> i1;
    aux::shared_ptr<c27> i2;
    aux::shared_ptr<c27> i3;

    {
    injector<> i;

    i1 = i.create<aux::shared_ptr<c27>>();
    i2 = i.create<aux::shared_ptr<c27>>();

    BOOST_CHECK(i1->d_ == i2->d_);
    }

    {
    injector<> i;

    i3 = i.create<aux::shared_ptr<c27>>();

    BOOST_CHECK(i3->d_ != i1->d_);
    BOOST_CHECK(i3->d_ != i2->d_);
    }
}

BOOST_AUTO_TEST_CASE(named_parameters_all_externals) {
    const int i1 = 42;
    const int i2 = 43;

    auto injector = make_injector(
         bind<i>::named<a>::to(aux::shared_ptr<impl>(new impl(i1)))
       , bind<i>::named<b>::to(aux::shared_ptr<impl>(new impl(i2)))
    );

    auto nameds_ = injector.create<nameds>();

    BOOST_CHECK(nameds_.n1_ != nameds_.n2_);
    BOOST_CHECK(dynamic_cast<impl*>(nameds_.n1_.get()));
    BOOST_CHECK(dynamic_cast<impl*>(nameds_.n2_.get()));
    BOOST_CHECK_EQUAL(i1, dynamic_cast<impl*>(nameds_.n1_.get())->i);
    BOOST_CHECK_EQUAL(i2, dynamic_cast<impl*>(nameds_.n2_.get())->i);

}

BOOST_AUTO_TEST_CASE(named_parameters_with_shared) {
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

} // namespace di
} // namespace boost

