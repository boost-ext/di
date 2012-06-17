//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/policy.hpp"

namespace boost {
namespace  di {

template<>
struct defaults<policy<>, specialized>
{
    typedef policy<> type;
};

} // namespace di
} // namespace boost

#include "boost/di/aux_/module.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/string.hpp>

#include "boost/di/aux_/convertible.hpp"
#include "boost/di/concepts/call_stack.hpp"
#include "boost/di/scopes/per_request.hpp"
#include "boost/di/scopes/singleton.hpp"
#include "boost/di/named.hpp"
#include "fake_dependency.hpp"
#include "fake_dependency_base_of.hpp"
#include "fake_visitor.hpp"
#include "fake_scope.hpp"
#include "fake_policy.hpp"
#include "contains_all.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace aux_ {

BOOST_AUTO_TEST_CASE(create_using_copy) {
    module<> module_;
    c0 obj = module_.create<c0>();
    (void)(obj);
}

BOOST_AUTO_TEST_CASE(create_using_ptr) {
    module<> module_;
    c0* obj = module_.create<c0*>();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(create_using_const_ptr) {
    module<> module_;
    const c0* obj = module_.create<const c0*>();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(create_using_shared_ptr) {
    module<> module_;
    shared_ptr<c0> obj = module_.create<shared_ptr<c0> >();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(create_default_ctor) {
    module<> module_;
    module_.create<c0>();
}

BOOST_AUTO_TEST_CASE(create_noncopyable) {
    module<>().create<shared_ptr<c1> >();
}

BOOST_AUTO_TEST_CASE(empty) {
    module<> module_;
    BOOST_CHECK_EQUAL(0, module_.create<int>());
}

BOOST_AUTO_TEST_CASE(create_per_request) {
    module<
        mpl::vector<
            fake_dependency<scopes::per_request<>, if0, c0if0>::type
        >
    > module_;

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

BOOST_AUTO_TEST_CASE(create_per_request_singleton) {
    module<
        mpl::vector<
            fake_dependency<scopes::per_request<>, if0, c0if0>::type
          , fake_dependency<scopes::singleton<>, c3>::type
        >
    > module_;

    shared_ptr<c8> c8_ = module_.create<shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
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

BOOST_AUTO_TEST_CASE(create_singleton_context) {
    module<
        mpl::vector<
            fake_dependency<scopes::per_request<>, if0, c0if0>::type
          , fake_dependency<scopes::per_request<>, if0, c1if0, concepts::call_stack<c6, c5> >::type
          , fake_dependency<scopes::singleton<>, c3>::type
        >
    >
    module_;

    shared_ptr<c8> c8_ = module_.create<shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0_ != c8_->c7_->c6_->c5_.if0_);

    BOOST_CHECK(dynamic_cast<c1if0*>(c8_->c7_->c6_->c5_.if0_.get()));
    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0_.get()));

    BOOST_CHECK_EQUAL(0, c8_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
}

BOOST_AUTO_TEST_CASE(create_per_request_singleton_context_order) {
    module<
        mpl::vector<
            fake_dependency<scopes::per_request<>, if0, c0if0>::type
          , fake_dependency<scopes::per_request<>, if0, c1if0, concepts::call_stack<c6, c5> >::type
          , fake_dependency<scopes::per_request<>, if0, c2if0, c7>::type
          , fake_dependency<scopes::singleton<>, c3>::type
        >
    > module_;

    shared_ptr<c8> c8_ = module_.create<shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0_ != c8_->c7_->c6_->c5_.if0_);

    BOOST_CHECK(dynamic_cast<c1if0*>(c8_->c7_->c6_->c5_.if0_.get()));
    BOOST_CHECK(dynamic_cast<c2if0*>(c8_->c7_->if0_.get()));

    BOOST_CHECK_EQUAL(0, c8_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
}

BOOST_AUTO_TEST_CASE(create_per_request_singleton_context_mix) {
    module<
        mpl::vector<
            fake_dependency<scopes::per_request<>, if0, c0if0>::type
          , fake_dependency<scopes::per_request<>, if0, c1if0, concepts::call_stack<c6, c5> >::type
          , fake_dependency<scopes::per_request<>, if0, c2if0, c7>::type
          , fake_dependency<scopes::singleton<>, c3>::type
          , fake_dependency<scopes::per_request<>, int, mpl::int_<1> >::type
          , fake_dependency<scopes::per_request<>, int, mpl::int_<2>, c8>::type
          , fake_dependency<scopes::per_request<>, named<int, mpl::string<'1'> >, mpl::int_<3>, concepts::call_stack<c7, c6, c4> >::type
          , fake_dependency<scopes::per_request<>, named<int, mpl::string<'2'> >, mpl::int_<4>, concepts::call_stack<c7, c6, c4> >::type
          , fake_dependency<scopes::per_request<>, int, mpl::int_<5>, c2>::type
        >
    > module_;

    shared_ptr<c8> c8_ = module_.create<shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0_ != c8_->c7_->c6_->c5_.if0_);

    BOOST_CHECK(dynamic_cast<c1if0*>(c8_->c7_->c6_->c5_.if0_.get()));
    BOOST_CHECK(dynamic_cast<c2if0*>(c8_->c7_->if0_.get()));

    BOOST_CHECK_EQUAL(2, c8_->i);
    BOOST_CHECK_EQUAL(3, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(4, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(1, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(5, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
}

BOOST_AUTO_TEST_CASE(create_singleton_impl) {
    module<
        mpl::vector<
            fake_dependency<scopes::singleton<>, if0, c0if0>::type
        >
    > module_;

    shared_ptr<c8> c8_ = module_.create<shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ != c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0_ == c8_->c7_->c6_->c5_.if0_);

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

BOOST_AUTO_TEST_CASE(create_singleton_many) {
    module<
        mpl::vector<
            fake_dependency<scopes::singleton<>, if0, c0if0>::type
          , fake_dependency<scopes::singleton<>, c3>::type
          , fake_dependency<scopes::singleton<>, c1>::type
        >
    > module_;

    shared_ptr<c8> c8_ = module_.create<shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ == c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0_ == c8_->c7_->c6_->c5_.if0_);

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

BOOST_AUTO_TEST_CASE(ctor_trait) {
    const int i = 42;

    module<
        mpl::vector<
            fake_dependency_base_of<scopes::per_request<>, int, mpl::int_<i> >::type
        >
    > module_;

    c14 obj = module_.create<c14>();

    BOOST_CHECK_EQUAL(i, obj.i);
    BOOST_CHECK_EQUAL(0.0, obj.d);
}

BOOST_AUTO_TEST_CASE(class_ctor_traits) {
    const int i1 = 42;
    const int i2 = 87;

    module<
        mpl::vector<
            fake_dependency<scopes::per_request<>, named<int, mpl::string<'1'> >, mpl::int_<i1> >::type
          , fake_dependency<scopes::per_request<>, named<int, mpl::string<'2'> >, mpl::int_<i2> >::type
        >
    > module_;

    c10 obj = module_.create<c10>();

    BOOST_CHECK_EQUAL(i1, obj.i1);
    BOOST_CHECK_EQUAL(i2, obj.i2);
}

BOOST_AUTO_TEST_CASE(base_of) {
    module<
        mpl::vector<
            fake_dependency_base_of<scopes::per_request<>, int, mpl::int_<1> >::type
          , fake_dependency_base_of<scopes::per_request<>, named<int, mpl::string<'2'> >, mpl::int_<4>, concepts::call_stack<c7, c6, c4> >::type
          , fake_dependency_base_of<scopes::per_request<>, int, mpl::int_<5>, c2>::type
          , fake_dependency_base_of<scopes::per_request<>, c0if0, c0if0>::type
          , fake_dependency_base_of<scopes::per_request<>, named<int, mpl::string<'1'> >, mpl::int_<3>, concepts::call_stack<c7, c6, c4> >::type
        >
    > module_;

    shared_ptr<c8> c8_ = module_.create<shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ != c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0_ != c8_->c7_->c6_->c5_.if0_);

    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->c6_->c5_.if0_.get()));
    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0_.get()));

    BOOST_CHECK_EQUAL(1, c8_->i);
    BOOST_CHECK_EQUAL(3, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(4, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(1, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(5, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
}

BOOST_AUTO_TEST_CASE(multiple_calls) {
    module<
        mpl::vector<
            fake_dependency_base_of<scopes::per_request<>, c0if0, c0if0>::type
          , fake_dependency_base_of<scopes::singleton<>, c3, c3, c15, concepts::call_stack<c6, c4> >::type
          , fake_dependency_base_of<scopes::singleton<>, c3, c3, c6>::type
        >
    > module_;

    shared_ptr<c15> c15_ = module_.create<shared_ptr<c15> >();

    BOOST_CHECK(c15_->c3_ == c15_->c6_.c4_->c3_); BOOST_CHECK(c15_->c3_ != c15_->c6_.c3_);
}

BOOST_AUTO_TEST_CASE(base_of_interface_not_trivial_ctor) {
    module<
        mpl::vector<
            fake_dependency_base_of<scopes::per_request<>, transaction_provider, transaction_provider>::type
        >
    > module_;

    transaction_usage obj = module_.create<transaction_usage>();

    BOOST_CHECK(obj.p->get().get() != obj.p->get().get());
}

BOOST_AUTO_TEST_CASE(named_shared_ptr_explicit_value) {
    const int i = 42;

    module<
        mpl::vector<
            fake_dependency<scopes::per_request<>, named<int, mpl::string<'1'> >, mpl::int_<i> >::type
        >
    > module_;

    c11 obj = module_.create<c11>();

    BOOST_CHECK_EQUAL(i, *obj.i);
}

BOOST_AUTO_TEST_CASE(named_shared_ptr_base_of_explicit_value) {
    const int i = 42;

    module<
        mpl::vector<
            fake_dependency_base_of<scopes::per_request<>, named<int, mpl::string<'1'> >, mpl::int_<i> >::type
        >
    > module_;

    c11 obj = module_.create<c11>();

    BOOST_CHECK_EQUAL(i, *obj.i);
}

BOOST_AUTO_TEST_CASE(named_shared_ptr_if) {
    module<
        mpl::vector<
            fake_dependency_base_of<scopes::per_request<>, named<if0, mpl::string<'1'> >, c0if0>::type
        >
    > module_;

    c12 obj = module_.create<c12>();

    BOOST_CHECK(dynamic_cast<c0if0*>(obj.if0_.get()));
    BOOST_CHECK_EQUAL(0, obj.c2_->i);
    BOOST_CHECK_EQUAL(0.0, obj.c2_->d);
    BOOST_CHECK_EQUAL(0, obj.c2_->c);
}

BOOST_AUTO_TEST_CASE(named_shared_ptr_if_with_not_trivial_ctor) {
    const int i = 42;

    module<
        mpl::vector<
            fake_dependency_base_of<scopes::per_request<>, named<if0>, c3if0>::type
          , fake_dependency_base_of<scopes::per_request<>, int, mpl::int_<i> >::type
        >
    > module_;

    c13 obj = module_.create<c13>();

    c3if0* if0_ = dynamic_cast<c3if0*>(obj.if0_.get());
    BOOST_CHECK(if0_);
    BOOST_CHECK_EQUAL(i, obj.c3_.i);
    BOOST_CHECK_EQUAL(i, if0_->i);
    BOOST_CHECK_EQUAL(0.0, if0_->d);
}

BOOST_AUTO_TEST_CASE(externals_create_by_explicit_value) {
    const int i = 42;
    const double d = 21.0;
    const char c = 'x';

    convertible<int> i_(i);
    convertible<double> d_(d);
    convertible<char> c_(c);

    module<
        mpl::vector<
            fake_dependency<scopes::per_request<>, std::string, mpl::string<'s'> >::type
        >
      , mpl::vector<
            convertible<int>
          , convertible<double>
          , convertible<char>
        >
    > module_(i_, d_, c_);

    c9 obj = module_.create<c9>();

    BOOST_CHECK_EQUAL(i, obj.i);
    BOOST_CHECK_EQUAL(d, obj.d);
    BOOST_CHECK_EQUAL(c, obj.c);
    BOOST_CHECK_EQUAL("s", obj.s);
}

BOOST_AUTO_TEST_CASE(externals_create_with_non_trivial_ctor) {
    const int i = 42;
    const double d = 21.0;
    const char c = 'x';

    convertible<c2> c2_(make_shared<c2>(i, d, c));

    module<
        mpl::vector<
            fake_dependency<scopes::per_request<>, c2>::type
        >
      , mpl::vector<
            convertible<c2>
        >
    > module_(c2_);

    c2 obj = module_.create<c2>();

    BOOST_CHECK_EQUAL(i, obj.i);
    BOOST_CHECK_EQUAL(d, obj.d);
    BOOST_CHECK_EQUAL(c, obj.c);
}

BOOST_AUTO_TEST_CASE(externals_create_with_attributes) {
    const int i1 = 42;
    const int i2 = 87;

    typedef named<int, mpl::string<'1'> > named1;
    typedef named<int, mpl::string<'2'> > named2;

    convertible<named1> i1_(i1);
    convertible<named2> i2_(i2);

    module<
        mpl::vector<
            fake_dependency<scopes::per_request<>, named1, int>::type
          , fake_dependency<scopes::per_request<>, named2, int>::type
        >
      , mpl::vector<
            convertible<named1>
          , convertible<named2>
        >
    > module_(i1_, i2_);

    c4 obj = module_.create<c4>();

    BOOST_CHECK_EQUAL(i1, obj.i1);
    BOOST_CHECK_EQUAL(i2, obj.i2);
}

BOOST_AUTO_TEST_CASE(create_string_boost_function_ptr) {
    const int i1 = 42;
    const int i2 = 87;
    struct c
    {
        static int f() { return i1; }
    };

    convertible<function<int()> > i_(&c::f);

    module<
        mpl::vector<
            fake_dependency_base_of<scopes::per_request<>, int, mpl::int_<i2> >::type
          , fake_dependency_base_of<scopes::per_request<>, std::string, mpl::string<'s'> >::type
        >
      , mpl::vector<
            convertible<function<int()> >
        >
    > module_(i_);

    c17 obj = module_.create<c17>();

    BOOST_CHECK_EQUAL(i1, obj.f_());
    BOOST_CHECK_EQUAL(i2, obj.c3_.i);
    BOOST_CHECK_EQUAL("s", obj.s_);
    BOOST_CHECK_EQUAL(i2, obj.i_);
}

BOOST_AUTO_TEST_CASE(visit) {
    module<
        mpl::vector<
            fake_dependency_base_of<scopes::per_request<>, transaction_provider, transaction_provider>::type
          , fake_dependency_base_of<scopes::per_request<>, int, mpl::int_<0> >::type
        >
    > module_;

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

    module<
        mpl::vector<
            fake_dependency_base_of<fake_scope, c0if0, c0if0>::type
        >
    > module_;

    BOOST_CHECK_EQUAL(0, fake_scope::entry_calls());
    BOOST_CHECK_EQUAL(0, fake_scope::exit_calls());

    module_.call<fake_scope>(fake_scope::entry());
    BOOST_CHECK_EQUAL(1, fake_scope::entry_calls());
    BOOST_CHECK_EQUAL(0, fake_scope::exit_calls());

    module_.call<fake_scope>(fake_scope::exit());
    BOOST_CHECK_EQUAL(1, fake_scope::entry_calls());
    BOOST_CHECK_EQUAL(1, fake_scope::exit_calls());
}

BOOST_AUTO_TEST_CASE(policies) {
    BOOST_CHECK((
        contains_all<
            mpl::vector<
                policy<fake_policy<0> >
              , policy<> // default
            >
          , module<
                mpl::vector<
                    policy<fake_policy<0> >
                >
            >::policies::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(policies_mix) {
    BOOST_CHECK((
        contains_all<
            mpl::vector<
                policy<
                    fake_policy<0>
                  , fake_policy<1>
                >
              , policy<> // default
            >
          , module<
                mpl::vector<
                    fake_dependency_base_of<scopes::per_request<>, int, mpl::int_<0> >::type
                  , policy<fake_policy<0>, fake_policy<1> >
                >
            >::policies::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(policies_mix_join_many) {
    BOOST_CHECK((
        contains_all<
            mpl::vector<
                policy<fake_policy<0>, fake_policy<1> >
              , policy<fake_policy<2> >
              , policy<fake_policy<3> >
              , policy<> // default
            >
          , module<
                mpl::vector<
                    policy<fake_policy<0>, fake_policy<1> >
                  , policy<fake_policy<2> >
                  , fake_dependency_base_of<scopes::per_request<>, int, mpl::int_<0> >::type
                  , policy<fake_policy<3> >
                >
            >::policies::type
        >::value
    ));
}

} // namespace aux_
} // namespace di
} // namespace boost

