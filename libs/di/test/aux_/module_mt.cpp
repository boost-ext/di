//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/vector.hpp>
#include "boost/di/named.hpp"
#include "boost/di/concepts/call_stack.hpp"
#include "boost/di/scopes/per_request.hpp"
#include "boost/di/scopes/singleton.hpp"
#include "boost/di/aux/dependency.hpp"
#include "boost/di/aux/instance.hpp"
#include "boost/di/aux/pool.hpp"
#include "boost/di/detail/module.hpp"
#include "dependency.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace detail {

BOOST_AUTO_TEST_CASE(module_create_using_copy)
{
    module<mpl::vector0<> > module_;
    c0 obj = module_.create<c0>();
    (void)(obj);
}

BOOST_AUTO_TEST_CASE(module_create_using_ptr)
{
    module<mpl::vector0<> > module_;
    c0* obj = module_.create<c0*>();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(module_create_using_const_ptr)
{
    module<mpl::vector0<> > module_;
    const c0* obj = module_.create<const c0*>();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(module_create_using_shared_ptr)
{
    module<mpl::vector0<> > module_;
    shared_ptr<c0> obj = module_.create<shared_ptr<c0> >();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(module_create_default_ctor)
{
    module<mpl::vector0<> > module_;
    c0 obj = module_.create<c0>();
    (void)(obj);
}

BOOST_AUTO_TEST_CASE(module_create_per_request)
{
    module<
        mpl::vector<
            dependency<scopes::per_request, if0, c0if0>::type
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

BOOST_AUTO_TEST_CASE(module_create_per_request_singleton)
{
    module<
        mpl::vector<
            dependency<scopes::per_request, if0, c0if0>::type
          , dependency<scopes::singleton, c3>::type
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

BOOST_AUTO_TEST_CASE(module_create_singleton_context)
{
    module<
        mpl::vector<
            dependency<scopes::per_request, if0, c0if0>::type
          , dependency<scopes::per_request, if0, c1if0, concepts::call_stack<c6, c5> >::type
          , dependency<scopes::singleton, c3>::type
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

BOOST_AUTO_TEST_CASE(module_create_per_request_singleton_context_order)
{
    module<
        mpl::vector<
            dependency<scopes::per_request, if0, c0if0>::type
          , dependency<scopes::per_request, if0, c1if0, concepts::call_stack<c6, c5> >::type
          , dependency<scopes::per_request, if0, c2if0, c7>::type
          , dependency<scopes::singleton, c3>::type
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

BOOST_AUTO_TEST_CASE(module_create_per_request_singleton_context_mix)
{
    module<
        mpl::vector<
            dependency<scopes::per_request, if0, c0if0>::type
          , dependency<scopes::per_request, if0, c1if0, concepts::call_stack<c6, c5> >::type
          , dependency<scopes::per_request, if0, c2if0, c7>::type
          , dependency<scopes::singleton, c3>::type
          , dependency<scopes::per_request, int, mpl::int_<1> >::type
          , dependency<scopes::per_request, int, mpl::int_<2>, c8>::type
          , dependency<scopes::per_request, named<int, mpl::string<'1'> >, mpl::int_<3>, concepts::call_stack<c7, c6, c4> >::type
          , dependency<scopes::per_request, named<int, mpl::string<'2'> >, mpl::int_<4>, concepts::call_stack<c7, c6, c4> >::type
          , dependency<scopes::per_request, int, mpl::int_<5>, c2>::type
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

BOOST_AUTO_TEST_CASE(module_create_singleton_impl)
{
    module<
        mpl::vector<
            dependency<scopes::singleton, if0, c0if0>::type
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

BOOST_AUTO_TEST_CASE(module_create_singleton_many)
{
    module<
        mpl::vector<
            dependency<scopes::singleton, if0, c0if0>::type
          , dependency<scopes::singleton, c3>::type
          , dependency<scopes::singleton, c1>::type
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

BOOST_AUTO_TEST_CASE(module_ctor_traits)
{
    const int i = 42;

    module<
        mpl::vector<
            dependency_base_of<scopes::per_request, int, mpl::int_<i> >::type
        >
    > module_;

    c14 obj = module_.create<c14>();

    BOOST_CHECK_EQUAL(i, obj.i);
    BOOST_CHECK_EQUAL(0.0, obj.d);
}

BOOST_AUTO_TEST_CASE(module_class_ctor_traits)
{
    const int i1 = 42;
    const int i2 = 87;

    module<
        mpl::vector<
            dependency<scopes::per_request, named<int, mpl::string<'1'> >, mpl::int_<i1> >::type
          , dependency<scopes::per_request, named<int, mpl::string<'2'> >, mpl::int_<i2> >::type
        >
    > module_;

    c10 obj = module_.create<c10>();

    BOOST_CHECK_EQUAL(i1, obj.i1);
    BOOST_CHECK_EQUAL(i2, obj.i2);
}

BOOST_AUTO_TEST_CASE(module_base_of)
{
    module<
        mpl::vector<
            dependency_base_of<scopes::per_request, int, mpl::int_<1> >::type
          , dependency_base_of<scopes::per_request, named<int, mpl::string<'2'> >, mpl::int_<4>, concepts::call_stack<c7, c6, c4> >::type
          , dependency_base_of<scopes::per_request, int, mpl::int_<5>, c2>::type
          , dependency_base_of<scopes::per_request, c0if0, c0if0>::type
          , dependency_base_of<scopes::per_request, named<int, mpl::string<'1'> >, mpl::int_<3>, concepts::call_stack<c7, c6, c4> >::type
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

BOOST_AUTO_TEST_CASE(module_multiple_calls)
{
    module<
        mpl::vector<
            dependency_base_of<scopes::per_request, c0if0, c0if0>::type
          , dependency_base_of<scopes::singleton, c3, c3, c15, concepts::call_stack<c6, c4> >::type
          , dependency_base_of<scopes::singleton, c3, c3, c6>::type
        >
    > module_;

    shared_ptr<c15> c15_ = module_.create<shared_ptr<c15> >();

    BOOST_CHECK(c15_->c3_ == c15_->c6_.c4_->c3_); BOOST_CHECK(c15_->c3_ != c15_->c6_.c3_);
}

BOOST_AUTO_TEST_CASE(module_base_of_interface_not_trivial_ctor)
{
    module<
        mpl::vector<
            dependency_base_of<scopes::per_request, transaction_provider, transaction_provider>::type
        >
    > module_;

    transaction_usage obj = module_.create<transaction_usage>();

    BOOST_CHECK(obj.p->get().get() != obj.p->get().get());
}

BOOST_AUTO_TEST_CASE(module_named_shared_ptr_base_of)
{
    const int i = 42;

    module<
        mpl::vector<
            dependency<scopes::per_request, named<int, mpl::string<'1'> >, mpl::int_<i> >::type
        >
    > module_;

    c11 obj = module_.create<c11>();

    BOOST_CHECK_EQUAL(i, *obj.i);
}

BOOST_AUTO_TEST_CASE(module_named_shared_ptr)
{
    const int i = 42;

    module<
        mpl::vector<
            dependency_base_of<scopes::per_request, named<int, mpl::string<'1'> >, mpl::int_<i> >::type
        >
    > module_;

    c11 obj = module_.create<c11>();

    BOOST_CHECK_EQUAL(i, *obj.i);
}

BOOST_AUTO_TEST_CASE(module_named_shared_ptr_if)
{
    module<
        mpl::vector<
            dependency_base_of<scopes::per_request, named<if0, mpl::string<'1'> >, c0if0>::type
        >
    > module_;

    c12 obj = module_.create<c12>();

    BOOST_CHECK(dynamic_cast<c0if0*>(obj.if0_.get()));
    BOOST_CHECK_EQUAL(0, obj.c2_->i);
    BOOST_CHECK_EQUAL(0.0, obj.c2_->d);
    BOOST_CHECK_EQUAL(0, obj.c2_->c);
}

BOOST_AUTO_TEST_CASE(module_named_shared_ptr_if_with_not_trivial_ctor)
{
    const int i = 42;

    module<
        mpl::vector<
            dependency_base_of<scopes::per_request, named<if0>, c3if0>::type
          , dependency_base_of<scopes::per_request, int, mpl::int_<i> >::type
        >
    > module_;

    c13 obj = module_.create<c13>();

    c3if0* if0_ = dynamic_cast<c3if0*>(obj.if0_.get());
    BOOST_CHECK(if0_);
    BOOST_CHECK_EQUAL(i, obj.c3_.i);
    BOOST_CHECK_EQUAL(i, if0_->i);
    BOOST_CHECK_EQUAL(0.0, if0_->d);
}

BOOST_AUTO_TEST_CASE(module_externals_create_by_explicit_value)
{
    const int i = 42;
    const double d = 21.0;
    const char c = 'x';

    aux_::instance<int> i_(i);
    aux_::instance<double> d_(d);
    aux_::instance<char> c_(c);

    module<
        mpl::vector<
            dependency<scopes::per_request, std::string, mpl::string<'di'> >::type
        >
      , mpl::vector<
            aux_::instance<int>
          , aux_::instance<double>
          , aux_::instance<char>
        >
    > module_(i_, d_, c_);

    c9 obj = module_.create<c9>();

    BOOST_CHECK_EQUAL(i, obj.i);
    BOOST_CHECK_EQUAL(d, obj.d);
    BOOST_CHECK_EQUAL(c, obj.c);
    BOOST_CHECK_EQUAL("di", obj.s);
}

BOOST_AUTO_TEST_CASE(module_externals_create_with_non_trivial_ctor)
{
    const int i = 42;
    const double d = 21.0;
    const char c = 'x';

    aux_::instance<c2> c2_(make_shared<c2>(i, d, c));

    module<
        mpl::vector<
            dependency<scopes::per_request, c2>::type
        >
      , mpl::vector<
            aux_::instance<c2>
        >
    > module_(c2_);

    c2 obj = module_.create<c2>();

    BOOST_CHECK_EQUAL(i, obj.i);
    BOOST_CHECK_EQUAL(d, obj.d);
    BOOST_CHECK_EQUAL(c, obj.c);
}

BOOST_AUTO_TEST_CASE(module_externals_create_with_attributes)
{
    const int i1 = 42;
    const int i2 = 87;

    typedef named<int, mpl::string<'1'> > named1;
    typedef named<int, mpl::string<'2'> > named2;

    aux_::instance<named1> i1_(i1);
    aux_::instance<named2> i2_(i2);

    module<
        mpl::vector<
            dependency<scopes::per_request, named1, int>::type
          , dependency<scopes::per_request, named2, int>::type
        >
      , mpl::vector<
            aux_::instance<named1>
          , aux_::instance<named2>
        >
    > module_(i1_, i2_);

    c4 obj = module_.create<c4>();

    BOOST_CHECK_EQUAL(i1, obj.i1);
    BOOST_CHECK_EQUAL(i2, obj.i2);
}

} // namespace detail
} // namespace di
} // namespace boost

