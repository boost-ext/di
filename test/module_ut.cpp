//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/module.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/scopes/per_request.hpp"
#include "boost/di/scopes/singleton.hpp"
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
    struct module_type
        : module<
              per_request<
                  c0if0
              >
          >
    { } module_;

    shared_ptr<c8> c8_ = module_.create<shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ == c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
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
    struct module_type
        : module<
              per_request<
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

    struct module_type
        : module<
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
    struct module_type
       : module<>
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector0<>
          , module_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(default_scope_deduce) {
    struct module_type
        : module<c1>
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::deduce, c1, c1>::type
            >
          , module_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(default_scope_many) {
    struct module_type
        : module<c1, c2, c3>
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::deduce, c1, c1>::type
              , fake_dependency_base_of<scopes::deduce, c2, c2>::type
              , fake_dependency_base_of<scopes::deduce, c3, c3>::type
            >
          , module_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(default_scope_bind) {
    struct module_type
        : module<
              bind<if0, c0if0>
            , c1
            , bind<c2>::in_name<int>
            , bind<c3>::in_call<call_stack<c4, c5> >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::deduce, if0, c0if0>::type
              , fake_dependency_base_of<scopes::deduce, c1, c1>::type
              , fake_dependency_base_of<scopes::deduce, named<c2, int>, c2>::type
              , fake_dependency_base_of<scopes::deduce, c3, c3, call_stack<c4, c5> >::type
            >
          , module_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(custom_scope) {
    struct module_type
        : module<
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
          , module_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(one_scope) {
    struct module_type
        : module<
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
          , module_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(one_scope_alias) {
    struct module_type
        : module<
              singleton<
                  c0if0
              >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton<>, c0if0, c0if0>::type
            >
          , module_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(one_scope_direct) {
    struct module_type
        : module<
              singleton<c0if0>
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton<>, c0if0, c0if0>::type
            >
          , module_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(many_singleton) {
    struct module_type
        : module<
              singleton<
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
          , module_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(many_scopes) {
    struct module_type
        : module<
              singleton<
                  c1, c2
              >
            , per_request<
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
            module_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(in_call) {
    struct module_type
        : module<
              per_request<c1>::in_call<c2>
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::per_request<>, c1, c1, c2>::type
            >
          , module_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(in_name) {
    struct module_type
        : module<
              singleton<c1>::in_name<int>
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton<>, named<c1, int>, c1>::type
            >
          , module_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(in_name_in_call) {
    struct module_type
        : module<
              singleton<
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
            module_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(in_call_in_name) {
    struct module_type
        : module<
              singleton<
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
          , module_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_if) {
    struct module_type
        : module<
              singleton<
                  bind<if0, c0if0>
              >
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton<>, if0, c0if0>::type
            >
          , module_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(mix) {
    struct module_type
        : module<
              singleton<
                  bind<if0, c0if0>
                , c1
                , bind<c2>::in_name<int>
                , bind<c3>::in_call<call_stack<c4, c5> >
              >
            , per_request<
                c6
              >
            , singleton<c7>::in_name<double>::in_call<c1>
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton<>, if0, c0if0>::type
              , fake_dependency_base_of<scopes::singleton<>, c1, c1>::type
              , fake_dependency_base_of<scopes::singleton<>, named<c2, int>, c2>::type
              , fake_dependency_base_of<scopes::singleton<>, c3, c3, call_stack<c4, c5> >::type
              , fake_dependency_base_of<scopes::per_request<>, c6, c6>::type
              , fake_dependency_base_of<scopes::singleton<>, named<c7, double>, c7, c1>::type
            >
          , module_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(named_in_call) {
    struct module_type
        : module<
              per_request<
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
          , module_type::deps
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(multiple_calls) {
    struct module_type
        : module<
              singleton<
                  bind<c0>::in_call<c1, call_stack<c2, c3>, c4 >
              >
            , bind<c5>::in_call<int, double>
          >
    { };

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_dependency_base_of<scopes::singleton<>, c0, c0, c1, call_stack<c2, c3>, c4>::type
              , fake_dependency_base_of<scopes::deduce, c5, c5, int, double>::type
            >
          , module_type::deps
        >::value
    ));
}

} // namespace di
} // namespace boot

