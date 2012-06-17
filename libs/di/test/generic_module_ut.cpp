//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/generic_module.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "boost/di/aux_/convertible.hpp"
#include "boost/di/named.hpp"
#include "boost/di/concepts.hpp"

#include "fake_dependency.hpp"
#include "fake_dependency_base_of.hpp"
#include "fake_visitor.hpp"
#include "fake_scope.hpp"
#include "contains_all.hpp"
#include "data.hpp"

namespace boost {
namespace di {

struct int_value
{
    explicit int_value(int i)
        : i(i)
    { }

    int i;
};

struct double_value
{
    static double value;
};
double double_value::value = 0;

BOOST_AUTO_TEST_CASE(create) {
    struct module
        : generic_module<
              per_requests<
                  c0if0
              >
          >
    { } module_;

    shared_ptr<c8> c8_ = module_.create<shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ != c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0_ != c8_->c7_->c6_->c5_.if0_);

    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->c6_->c5_.if0_.get()));
    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0_.get()));

    BOOST_CHECK_EQUAL(0, c8_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
}

BOOST_AUTO_TEST_CASE(visit) {
    struct module
        : generic_module<
              per_requests<
                  transaction_provider
                , mpl::int_<0>
              >
          >
    { } module_;

    fake_visitor<
        mpl::vector<
            transaction_usage
          , shared_ptr<provider<shared_ptr<transaction> > >
          , shared_ptr<c3>
          , int
        >
    > visitor;

    module_.visit<transaction_usage>(visitor);
}

BOOST_AUTO_TEST_CASE(call) {
    fake_scope::entry_calls() = 0;
    fake_scope::exit_calls() = 0;

    struct module
        : generic_module<
              scope<fake_scope>::bind<c0if0>
          >
    { } module_;

    BOOST_CHECK_EQUAL(0, fake_scope::entry_calls());
    BOOST_CHECK_EQUAL(0, fake_scope::exit_calls());

    module_.call<fake_scope>(fake_scope::entry());
    BOOST_CHECK_EQUAL(1, fake_scope::entry_calls());
    BOOST_CHECK_EQUAL(0, fake_scope::exit_calls());

    module_.call<fake_scope>(fake_scope::exit());
    BOOST_CHECK_EQUAL(1, fake_scope::entry_calls());
    BOOST_CHECK_EQUAL(1, fake_scope::exit_calls());
}

BOOST_AUTO_TEST_CASE(empty) {
    struct module
       : generic_module<>
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector0<>
          , module::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(default_scope) {
    struct module
        : generic_module<c1>
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::per_request<>, c1, c1>::type
            >
          , module::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(default_scope_many) {
    struct module
        : generic_module<c1, c2, c3>
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::per_request<>, c1, c1>::type
              , fake_dependency_base_of<scopes::per_request<>, c2, c2>::type
              , fake_dependency_base_of<scopes::per_request<>, c3, c3>::type
            >
          , module::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(default_scope_bind) {
    struct module
        : generic_module<
              bind<if0, c0if0>
            , c1
            , bind<c2>::in_name<int>
            , bind<c3>::in_call<call_stack<c4, c5> >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::per_request<>, if0, c0if0>::type
              , fake_dependency_base_of<scopes::per_request<>, c1, c1>::type
              , fake_dependency_base_of<scopes::per_request<>, named<c2, int>, c2>::type
              , fake_dependency_base_of<scopes::per_request<>, c3, c3, call_stack<c4, c5> >::type
            >
          , module::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(custom_scope) {
    struct module
        : generic_module<
              scope<fake_scope>::bind<
                  c0if0
              >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<fake_scope, c0if0, c0if0>::type
            >
          , module::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(one_scope) {
    struct module
        : generic_module<
              scope<scopes::singleton<> >::bind<
                  c0if0
              >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton<>, c0if0, c0if0>::type
            >
          , module::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(one_scope_alias) {
    struct module
        : generic_module<
              singletons<
                  c0if0
              >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton<>, c0if0, c0if0>::type
            >
          , module::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(one_scope_direct) {
    struct module
        : generic_module<
              singleton<c0if0>
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton<>, c0if0, c0if0>::type
            >
          , module::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(many_singletons) {
    struct module
        : generic_module<
              singletons<
                c1, c2, c3
              >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton<>, c1, c1>::type
              , fake_dependency_base_of<scopes::singleton<>, c2, c2>::type
              , fake_dependency_base_of<scopes::singleton<>, c3, c3>::type
            >
          , module::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(many_scopes) {
    struct module
        : generic_module<
              singletons<
                  c1, c2
              >
            , per_requests<
                  c3, c4
              >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton<>, c1, c1>::type
              , fake_dependency_base_of<scopes::singleton<>, c2, c2>::type
              , fake_dependency_base_of<scopes::per_request<>, c3, c3>::type
              , fake_dependency_base_of<scopes::per_request<>, c4, c4>::type
            >,
            module::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(in_call) {
    struct module
        : generic_module<
              per_request<c1>::in_call<c2>
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::per_request<>, c1, c1, c2>::type
            >
          , module::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(in_name) {
    struct module
        : generic_module<
              singleton<c1>::in_name<int>
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton<>, named<c1, int>, c1>::type
            >
          , module::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(in_name_in_call) {
    struct module
        : generic_module<
              singletons<
                  bind<c1>::in_name<int>::in_call<double>
                , bind<c2>::in_name<double>::in_call<int>
              >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton<>, named<c1, int>, c1, double>::type
              , fake_dependency_base_of<scopes::singleton<>, named<c2, double>, c2, int>::type
            >,
            module::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(in_call_in_name) {
    struct module
        : generic_module<
              singletons<
                  bind<c1>::in_call<double>::in_name<int>
                , bind<c2>::in_call<int>::in_name<double>
              >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton<>, named<c1, int>, c1, double>::type
              , fake_dependency_base_of<scopes::singleton<>, named<c2, double>, c2, int>::type
            >
          , module::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_if) {
    struct module
        : generic_module<
              singletons<
                  bind<if0, c0if0>
              >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::singleton<>, if0, c0if0>::type
            >
          , module::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(mix) {
    struct module
        : generic_module<
              singletons<
                  bind<if0, c0if0>
                , c1
                , bind<c2>::in_name<int>
                , bind<c3>::in_call<call_stack<c4, c5> >
              >
            , per_requests<
                c6
              >
            , singleton<c7>::in_name<double>::in_call<c1>
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency<scopes::singleton<>, if0, c0if0>::type
              , fake_dependency_base_of<scopes::singleton<>, c1, c1>::type
              , fake_dependency_base_of<scopes::singleton<>, named<c2, int>, c2>::type
              , fake_dependency_base_of<scopes::singleton<>, c3, c3, call_stack<c4, c5> >::type
              , fake_dependency_base_of<scopes::per_request<>, c6, c6>::type
              , fake_dependency_base_of<scopes::singleton<>, named<c7, double>, c7, c1>::type
            >
          , module::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(named_in_call) {
    struct module
        : generic_module<
              per_requests<
                  bind<int, mpl::int_<1> >
                , bind<int, mpl::int_<4> >::in_name<mpl::string<'2'> >::in_call<call_stack<c7, c6, c4> >
                , bind<int, mpl::int_<5> >::in_call<c2>
              >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::per_request<>, int, mpl::int_<1> >::type
              , fake_dependency_base_of<scopes::per_request<>, named<int, mpl::string<'2'> >, mpl::int_<4>, call_stack<c7, c6, c4> >::type
              , fake_dependency_base_of<scopes::per_request<>, int, mpl::int_<5>, c2>::type
            >
          , module::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(multiple_calls) {
    struct module
        : generic_module<
              singletons<
                  bind<c0>::in_call<c1, call_stack<c2, c3>, c4 >
              >
            , bind<c5>::in_call<int, double>
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton<>, c0, c0, c1, call_stack<c2, c3>, c4>::type
              , fake_dependency_base_of<scopes::per_request<>, c5, c5, int, double>::type
            >
          , module::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(externals_base) {
    struct module
        : generic_module<
              externals<
                  c1
              >
          >
    { };

    BOOST_CHECK((contains_all<mpl::vector0<>, module::deps>::value));

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                aux_::convertible<c1>
            >
          , module::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(externals_mix) {
    struct module
        : generic_module<
              externals<
                  c1, c2
              >
            , external<c3>
          >
    { };

    BOOST_CHECK((contains_all<mpl::vector0<>, module::deps>::value));

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                aux_::convertible<c1>
              , aux_::convertible<c2>
              , aux_::convertible<c3>
            >
          , module::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(externals_bind) {
    struct module
        : generic_module<
              externals<
                  int
                , bind<c1>::in_name<int>
                , bind<c2>::in_call<c1>
                , bind<c3>::in_name<double>::in_call<c4, c5>
              >
          >
    { };

    BOOST_CHECK((contains_all<mpl::vector0<>, module::deps>::value));

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                aux_::convertible<int>
              , aux_::convertible<named<c1, int> >
              , aux_::convertible<c2, mpl::vector<c1> >
              , aux_::convertible<named<c3, double>, mpl::vector<c4, c5> >
            >
          , module::pool::externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(set_instance_int) {
    const int i = 42;

    struct module
        : generic_module<
              externals<
                  int
              >
          >
    { };

    BOOST_CHECK_EQUAL(i, static_cast<int>(module::set<int>(i)));
}

BOOST_AUTO_TEST_CASE(set_instance_annotate_in_in_call) {
    const int i1 = 42;
    const int i2 = 43;

    struct module
        : generic_module<
              externals<
                  annotate<bind<int>::in_call<c1, c2> >::with<a>
                , annotate<bind<int>::in_call<c3, c4> >::with<b>
              >
          >
    { };

    BOOST_CHECK_EQUAL(i1, static_cast<int>(module::set<a>(i1)));
    BOOST_CHECK_EQUAL(i2, static_cast<int>(module::set<b>(i2)));
}

BOOST_AUTO_TEST_CASE(set_instance_annotate_in_name) {
    const int i1 = 42;
    const int i2 = 43;

    struct module
        : generic_module<
              externals<
                  annotate<bind<int>::in_name<float> >::with<a>
                , annotate<bind<int>::in_name<double> >::with<b>
              >
          >
    { };

    BOOST_CHECK_EQUAL(i1, static_cast<int>(module::set<a>(i1)));
    BOOST_CHECK_EQUAL(i2, static_cast<int>(module::set<b>(i2)));
}

BOOST_AUTO_TEST_CASE(set_instance_annotatein_name_in_call) {
    const int i1 = 42;
    const int i2 = 43;

    struct module
        : generic_module<
              externals<
                  annotate<bind<int>::in_call<c1, c2>::in_name<float> >::with<a>
                , annotate<bind<int>::in_name<double>::in_call<c3, c4> >::with<b>
              >
          >
    { };

    BOOST_CHECK_EQUAL(i1, static_cast<int>(module::set<a>(i1)));
    BOOST_CHECK_EQUAL(i2, static_cast<int>(module::set<b>(i2)));
}

BOOST_AUTO_TEST_CASE(set_instance_mix) {
    const int i1 = 42;
    const int i2 = 43;
    const int i3 = 44;

    struct module
        : generic_module<
              externals<
                  int
                , annotate<bind<int>::in_name<float> >::with<a>
                , annotate<bind<int>::in_call<c1, c2>::in_name<float> >::with<b>
              >
          >
    { };

    BOOST_CHECK_EQUAL(i1, static_cast<int>(module::set<int>(i1)));
    BOOST_CHECK_EQUAL(i2, static_cast<int>(module::set<a>(i2)));
    BOOST_CHECK_EQUAL(i3, static_cast<int>(module::set<b>(i3)));
}

BOOST_AUTO_TEST_CASE(set_if) {
    shared_ptr<if0> c0if0_(new c0if0);

    struct module
        : generic_module<
              externals<
                  if0
              >
          >
    { };

    BOOST_CHECK_EQUAL(
        c0if0_
      , static_cast<shared_ptr<if0> >(module::set<if0>(c0if0_))
    );
}

BOOST_AUTO_TEST_CASE(set_variant_shared_ptr) {
    shared_ptr<c3> c3_(new c3);

    typedef generic_module<
        external<c3>
    > module;

    module module_(
        module::set<c3>(c3_)
    );

    c4 c4_ = module_.create<c4>();

    BOOST_CHECK_EQUAL(c3_, c4_.c3_);
}

BOOST_AUTO_TEST_CASE(set_variant_ref) {
    const int i = 42;
    const double d = 87.0;
    c3 c3_(i);
    c14 c14_(i, d);

    const c3& c3_const_ref = c3_;
    c14& c14_ref = c14_;

    typedef generic_module<
        externals<
            c3, c14
        >
    > module;

    module module_(
        module::set<c3>(c3_const_ref)
      , module::set<c14>(c14_ref)
    );

    shared_ptr<c16> c16_ = module_.create<shared_ptr<c16> >();

    BOOST_CHECK(&c3_const_ref == &c16_->c3_);
    BOOST_CHECK(&c14_ref == &c16_->c14_);

    BOOST_CHECK_EQUAL(c3_.i, c16_->c3_.i);
    BOOST_CHECK_EQUAL(c14_.i, c16_->c14_.i);
    BOOST_CHECK_EQUAL(c14_.d, c16_->c14_.d);
}

BOOST_AUTO_TEST_CASE(set_variant_no_copy) {
    const int i = 42;
    const double d = 87.0;
    c3 c3_(i);
    c14 c14_(i, d);

    typedef generic_module<
        externals<
            c3, c14
        >
    > module;

    module module_(
        module::set<c3>(c3_)
      , module::set<c14>(c14_)
    );

    shared_ptr<c16> c16_ = module_.create<shared_ptr<c16> >();

    BOOST_CHECK(&c3_ == &c16_->c3_);
    BOOST_CHECK(&c14_ == &c16_->c14_);

    BOOST_CHECK_EQUAL(c3_.i, c16_->c3_.i);
    BOOST_CHECK_EQUAL(c14_.i, c16_->c14_.i);
    BOOST_CHECK_EQUAL(c14_.d, c16_->c14_.d);
}

BOOST_AUTO_TEST_CASE(explicit_value) {
    double_value::value = 42.0;

    struct module
        : generic_module<
              bind<double, double_value>
          >
    { } module_;

    BOOST_CHECK_EQUAL(double_value::value, module_.create<double>());
}

BOOST_AUTO_TEST_CASE(ctor_with_externals) {
    const int i = 42;
    const double d = 87.0;

    typedef generic_module<
        externals<
            int
          , double
        >
    > module;

    module module_(
        module::set<int>(i),
        module::set<double>(d)
    );

    BOOST_CHECK_EQUAL(i, module_.create<int>());
    BOOST_CHECK_EQUAL(d, module_.create<double>());
}

BOOST_AUTO_TEST_CASE(ctor_with_externals_shared_ptr) {
    const int i = 42;
    shared_ptr<int_value> v(new int_value(i));

    typedef generic_module<
        external<
            int_value
        >
    > module;

    module module_(
        module::set<int_value>(v)
    );

    BOOST_CHECK_EQUAL(i, module_.create<int_value>().i);
}

} // namespace di
} // namespace boot

