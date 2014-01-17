//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/detail/module.hpp"
#include <memory>

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/string.hpp>
#include <boost/mpl/pair.hpp>

#include "boost/di/scopes/unique.hpp"
#include "boost/di/scopes/shared.hpp"
#include "boost/di/scopes/external.hpp"
#include "boost/di/concepts.hpp"
#include "fake_dependency.hpp"
#include "fake_dependency_base_of.hpp"
#include "fake_visitor.hpp"
#include "fake_scope.hpp"
#include "fake_policy.hpp"
#include "contains_all.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace detail {

BOOST_AUTO_TEST_CASE(create_using_copy) {
    module<> module_;
    c0 obj = module_.create<c0>();
    (void)(obj);
}

BOOST_AUTO_TEST_CASE(create_using_ptr) {
    module<> module_;
    aux::auto_ptr<c0> obj(module_.create<c0*>());
    BOOST_CHECK(obj.get());
}

BOOST_AUTO_TEST_CASE(create_using_const_ptr) {
    module<> module_;
    aux::auto_ptr<const c0> obj(module_.create<const c0*>());
    BOOST_CHECK(obj.get());
}

BOOST_AUTO_TEST_CASE(create_using_shared_ptr) {
    module<> module_;
    aux::shared_ptr<c0> obj = module_.create<aux::shared_ptr<c0>>();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(create_default_ctor) {
    module<> module_;
    module_.create<c0>();
}

BOOST_AUTO_TEST_CASE(create_noncopyable) {
    module<>().create<aux::shared_ptr<c1>>();
}

BOOST_AUTO_TEST_CASE(empty) {
    module<> module_;
    BOOST_CHECK_EQUAL(0, module_.create<int>());
}

BOOST_AUTO_TEST_CASE(create_unique) {
    module<
        mpl::vector<
            fake_dependency<scopes::unique<>, if0, c0if0>::type
        >
    > module_;

    aux::shared_ptr<c8> c8_ = module_.create<aux::shared_ptr<c8>>();

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

BOOST_AUTO_TEST_CASE(create_unique_shared) {
    module<
        mpl::vector<
            fake_dependency<scopes::unique<>, if0, c0if0>::type
          , fake_dependency<scopes::shared<>, c3>::type
        >
    > module_;

    aux::shared_ptr<c8> c8_ = module_.create<aux::shared_ptr<c8>>();

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

BOOST_AUTO_TEST_CASE(create_shared_context) {
    module<
        mpl::vector<
            fake_dependency<scopes::unique<>, if0, c0if0>::type
          , fake_dependency<scopes::unique<>, if0, c1if0, void, call_stack<c6, c5>>::type
          , fake_dependency<scopes::shared<>, c3>::type
        >
    >
    module_;

    aux::shared_ptr<c8> c8_ = module_.create<aux::shared_ptr<c8>>();

    BOOST_CHECK(c8_->c1_ == c8_->c7_->c6_->c5_.c1_);
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

BOOST_AUTO_TEST_CASE(create_unique_shared_context_order) {
    module<
        mpl::vector<
            fake_dependency<scopes::unique<>, if0, c0if0>::type
          , fake_dependency<scopes::unique<>, if0, c1if0, void, call_stack<c6, c5>>::type
          , fake_dependency<scopes::unique<>, if0, c2if0, void, c7>::type
          , fake_dependency<scopes::shared<>, c3>::type
        >
    > module_;

    aux::shared_ptr<c8> c8_ = module_.create<aux::shared_ptr<c8>>();

    BOOST_CHECK(c8_->c1_ == c8_->c7_->c6_->c5_.c1_);
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

BOOST_AUTO_TEST_CASE(create_unique_shared_context_mix) {
    module<
        mpl::vector<
            fake_dependency<scopes::unique<>, if0, c0if0>::type
          , fake_dependency<scopes::unique<>, if0, c1if0, void, call_stack<c6, c5>>::type
          , fake_dependency<scopes::unique<>, if0, c2if0, void, c7>::type
          , fake_dependency<scopes::shared<>, c3>::type
          , fake_dependency<scopes::unique<>, int, mpl::int_<1>>::type
          , fake_dependency<scopes::unique<>, int, mpl::int_<2>, void, c8>::type
          , fake_dependency<scopes::unique<>, int, mpl::int_<3>, mpl::string<'1'>, call_stack<c7, c6, c4>>::type
          , fake_dependency<scopes::unique<>, int, mpl::int_<4>, mpl::string<'2'>, call_stack<c7, c6, c4>>::type
          , fake_dependency<scopes::unique<>, int, mpl::int_<5>, void, c2>::type
        >
    > module_;

    aux::shared_ptr<c8> c8_ = module_.create<aux::shared_ptr<c8>>();

    BOOST_CHECK(c8_->c1_ == c8_->c7_->c6_->c5_.c1_);
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

BOOST_AUTO_TEST_CASE(create_shared_impl) {
    module<
        mpl::vector<
            fake_dependency<scopes::shared<>, if0, c0if0>::type
        >
    > module_;

    aux::shared_ptr<c8> c8_ = module_.create<aux::shared_ptr<c8>>();

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

BOOST_AUTO_TEST_CASE(create_shared_many) {
    module<
        mpl::vector<
            fake_dependency<scopes::shared<>, if0, c0if0>::type
          , fake_dependency<scopes::shared<>, c3>::type
          , fake_dependency<scopes::shared<>, c1>::type
        >
    > module_;

    aux::shared_ptr<c8> c8_ = module_.create<aux::shared_ptr<c8>>();

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
            fake_dependency_base_of<scopes::unique<>, int, mpl::int_<i>>::type
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
            fake_dependency<scopes::unique<>, int, mpl::int_<i1>, mpl::string<'1'>>::type
          , fake_dependency<scopes::unique<>, int, mpl::int_<i2>, mpl::string<'2'>>::type
        >
    > module_;

    c10 obj = module_.create<c10>();

    BOOST_CHECK_EQUAL(i1, obj.i1);
    BOOST_CHECK_EQUAL(i2, obj.i2);
}

BOOST_AUTO_TEST_CASE(base_of) {
    module<
        mpl::vector<
            fake_dependency_base_of<scopes::unique<>, int, mpl::int_<1>>::type
          , fake_dependency_base_of<scopes::unique<>, int, mpl::int_<4>, mpl::string<'2'>, call_stack<c7, c6, c4>>::type
          , fake_dependency_base_of<scopes::unique<>, int, mpl::int_<5>, void, c2>::type
          , fake_dependency_base_of<scopes::unique<>, c0if0, c0if0>::type
          , fake_dependency_base_of<scopes::unique<>, int, mpl::int_<3>, mpl::string<'1'>, call_stack<c7, c6, c4>>::type
        >
    > module_;

    aux::shared_ptr<c8> c8_ = module_.create<aux::shared_ptr<c8>>();

    BOOST_CHECK(c8_->c1_ == c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
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
            fake_dependency_base_of<scopes::unique<>, c0if0, c0if0>::type
          , fake_dependency_base_of<scopes::shared<>, c3, c3, void, c15, call_stack<c6, c4>>::type
          , fake_dependency_base_of<scopes::shared<>, c3, c3, void, c6>::type
        >
    > module_;

    aux::shared_ptr<c15> c15_ = module_.create<aux::shared_ptr<c15>>();

    BOOST_CHECK(c15_->c3_ == c15_->c6_.c4_->c3_); BOOST_CHECK(c15_->c3_ != c15_->c6_.c3_);
}

BOOST_AUTO_TEST_CASE(base_of_interface_not_trivial_ctor) {
    module<
        mpl::vector<
            fake_dependency_base_of<scopes::unique<>, transaction_provider, transaction_provider>::type
        >
    > module_;

    transaction_usage obj = module_.create<transaction_usage>();

    BOOST_CHECK(obj.p->get().get() != obj.p->get().get());
}

BOOST_AUTO_TEST_CASE(named_shared_ptr_explicit_value) {
    const int i = 42;

    module<
        mpl::vector<
            fake_dependency<scopes::unique<>, int, mpl::int_<i>, mpl::string<'1'>>::type
        >
    > module_;

    c11 obj = module_.create<c11>();

    BOOST_CHECK_EQUAL(i, *obj.i);
}

BOOST_AUTO_TEST_CASE(named_shared_ptr_base_of_explicit_value) {
    const int i = 42;

    module<
        mpl::vector<
            fake_dependency_base_of<scopes::unique<>, int, mpl::int_<i>, mpl::string<'1'>>::type
        >
    > module_;

    c11 obj = module_.create<c11>();

    BOOST_CHECK_EQUAL(i, *obj.i);
}

BOOST_AUTO_TEST_CASE(named_shared_ptr_if) {
    module<
        mpl::vector<
            fake_dependency_base_of<scopes::unique<>, if0, c0if0, mpl::string<'1'>>::type
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
            fake_dependency_base_of<scopes::unique<>, if0, c3if0>::type
          , fake_dependency_base_of<scopes::unique<>, int, mpl::int_<i>>::type
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

    fake_dependency<scopes::external<>, int>::type i_(i);
    fake_dependency<scopes::external<>, double>::type d_(d);
    fake_dependency<scopes::external<>, char>::type c_(c);

    module<
        mpl::vector<
            fake_dependency<scopes::unique<>, std::string, mpl::string<'s'>>::type
          , fake_dependency<scopes::external<>, int>::type
          , fake_dependency<scopes::external<>, double>::type
          , fake_dependency<scopes::external<>, char>::type
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

    using external_shared = scopes::external<convertibles::shared>;
    fake_dependency<external_shared, c2>::type c2_(aux::shared_ptr<c2>(new c2(i, d, c)));

    module<
        mpl::vector<
            fake_dependency<external_shared, c2>::type
        >
    > module_(c2_);

    auto obj = module_.create<aux::shared_ptr<c2>>();

    BOOST_CHECK_EQUAL(i, obj->i);
    BOOST_CHECK_EQUAL(d, obj->d);
    BOOST_CHECK_EQUAL(c, obj->c);
}

using externals_priority_t = mpl::vector<
    module<
        mpl::vector<
            fake_dependency<scopes::unique<>, int, int, mpl::string<'1'>>::type
          , fake_dependency<scopes::unique<>, int, int, mpl::string<'2'>>::type
          , fake_dependency<scopes::external<>, int, int, mpl::string<'1'>>::type
          , fake_dependency<scopes::external<>, int, int, mpl::string<'2'>>::type
        >
    >
  , module<
        mpl::vector<
            fake_dependency<scopes::external<>, int, int, mpl::string<'1'>>::type
          , fake_dependency<scopes::external<>, int, int, mpl::string<'2'>>::type
          , fake_dependency<scopes::unique<>, int, int, mpl::string<'1'>>::type
          , fake_dependency<scopes::unique<>, int, int, mpl::string<'2'>>::type
        >
    >
  , module<
        mpl::vector<
            fake_dependency<scopes::unique<>, int, int, mpl::string<'1'>>::type
          , fake_dependency<scopes::unique<>, int, int, mpl::string<'2'>>::type
          , fake_dependency<scopes::external<>, int, int, mpl::string<'1'>>::type
          , fake_dependency<scopes::external<>, int, int, mpl::string<'2'>>::type
        >
    >
  , module<
        mpl::vector<
            fake_dependency<scopes::external<>, int, int, mpl::string<'1'>>::type
          , fake_dependency<scopes::external<>, int, int, mpl::string<'2'>>::type
          , fake_dependency<scopes::unique<>, int, int, mpl::string<'1'>>::type
          , fake_dependency<scopes::unique<>, int, int, mpl::string<'2'>>::type
        >
    >
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(externals_create_priority, TModule, externals_priority_t) {
    const int i1 = 42;
    const int i2 = 87;

    fake_dependency<scopes::external<>, int, int, mpl::string<'1'>>::type i1_(i1);
    fake_dependency<scopes::external<>, int, int, mpl::string<'2'>>::type i2_(i2);

    TModule module_(i1_, i2_);

    c4 obj = module_.template create<c4>();

    BOOST_CHECK_EQUAL(i1, obj.i1);
    BOOST_CHECK_EQUAL(i2, obj.i2);
}

BOOST_AUTO_TEST_CASE(visit) {
    module<
        mpl::vector<
            fake_dependency_base_of<scopes::unique<>, transaction_provider, transaction_provider>::type
          , fake_dependency_base_of<scopes::unique<>, int, mpl::int_<0>>::type
        >
    > module_;

    fake_visitor<
        mpl::vector<
            transaction_usage
          , aux::shared_ptr<provider<aux::shared_ptr<transaction>>>
          , aux::shared_ptr<c3>
          , int
        >
    > visitor;

    module_.visit<transaction_usage>(visitor);
}

BOOST_AUTO_TEST_CASE(visit_external) {
    c3 c3_;
    fake_dependency<scopes::external<>, c3>::type e3(c3_);

    module<
        mpl::vector<
            fake_dependency<scopes::external<>, c3>::type
        >
    > module_(e3);

    fake_visitor<
        mpl::vector<
            mpl::pair<c18, scopes::unique<>>
          , mpl::pair<c0, scopes::unique<>>
          , mpl::pair<aux::shared_ptr<c1>, scopes::shared<>>
          , mpl::pair<c3&, scopes::external<>>
        >
    > visitor;

    module_.visit<c18>(visitor);
}

BOOST_AUTO_TEST_CASE(call) {
    fake_scope<>::entry_calls() = 0;
    fake_scope<>::exit_calls() = 0;

    module<
        mpl::vector<
            fake_dependency_base_of<fake_scope<>, c0if0, c0if0>::type
        >
    > module_;

    BOOST_CHECK_EQUAL(0, fake_scope<>::entry_calls());
    BOOST_CHECK_EQUAL(0, fake_scope<>::exit_calls());

    module_.call(fake_scope_entry());
    BOOST_CHECK_EQUAL(1, fake_scope<>::entry_calls());
    BOOST_CHECK_EQUAL(0, fake_scope<>::exit_calls());

    fake_scope<>::entry_calls() = 0;
    fake_scope<>::exit_calls() = 0;

    module_.call(fake_scope_exit());
    BOOST_CHECK_EQUAL(0, fake_scope<>::entry_calls());
    BOOST_CHECK_EQUAL(1, fake_scope<>::exit_calls());
}

using factory_module = module<
    mpl::vector<
       fake_dependency<scopes::deduce, if0, if0_factory>::type
     , fake_dependency<scopes::external<>, eid>::type
    >
>;

BOOST_AUTO_TEST_CASE(create_factory_e0) {
    fake_dependency<scopes::external<>, eid>::type eid_(e0);
    factory_module module_(eid_);

    auto c23_ = module_.create<aux::shared_ptr<c23>>();

    BOOST_CHECK(dynamic_cast<c0if0*>(c23_->if0_.get()));
}

BOOST_AUTO_TEST_CASE(create_factory_e1) {
    fake_dependency<scopes::external<>, eid>::type eid_(e1);
    factory_module module_(eid_);

    auto c23_ = module_.create<aux::shared_ptr<c23>>();

    BOOST_CHECK(dynamic_cast<c1if0*>(c23_->if0_.get()));
}

BOOST_AUTO_TEST_CASE(create_factory_null) {
    fake_dependency<scopes::external<>, eid>::type eid_(static_cast<eid>(0));
    factory_module module_(eid_);

    auto c23_ = module_.create<aux::shared_ptr<c23>>();

    BOOST_CHECK(nullptr == c23_->if0_.get());
}

BOOST_AUTO_TEST_CASE(policies) {
    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_policy<0>
            >
          , module<
                mpl::vector<
                    fake_policy<0>
                >
            >::policies::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(policies_mix) {
    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_policy<0>
              , fake_policy<1>
            >
          , module<
                mpl::vector<
                    fake_dependency_base_of<scopes::unique<>, int, mpl::int_<0>>::type
                  , fake_policy<0>
                  , fake_policy<1>
                >
            >::policies::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(policies_mix_join_many) {
    BOOST_CHECK((
        contains_all<
            mpl::vector<
                fake_policy<0>
              , fake_policy<1>
              , fake_policy<2>
              , fake_policy<3>
            >
          , module<
                mpl::vector<
                    fake_policy<0>
                  , fake_policy<1>
                  , fake_policy<2>
                  , fake_dependency_base_of<scopes::unique<>, int, mpl::int_<0>>::type
                  , fake_policy<3>
                >
            >::policies::type
        >::value
    ));
}

} // namespace detail
} // namespace di
} // namespace boost

