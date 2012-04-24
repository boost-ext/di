//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/empty.hpp>
#include "boost/di/named.hpp"
#include "boost/di/concepts/call_stack.hpp"
#include "boost/di/scopes/per_request.hpp"
#include "boost/di/scopes/singleton.hpp"
#include "boost/di/aux/dependency.hpp"
#include "boost/di/aux/instance.hpp"
#include "boost/di/aux/pool.hpp"
#include "boost/di/detail/factory.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace detail {

template<
    typename TScope
  , typename TExpected
  , typename TGiven
  , typename TContext0 = mpl_::na
  , typename TContext1 = mpl_::na
  , typename TContext2 = mpl_::na
>
struct dependency
{
    typedef mpl::vector<TContext0, TContext1, TContext2> context;
    typedef typename aux::dependency<
        TScope
      , TExpected
      , TGiven
      , typename mpl::if_<mpl::empty<context>, mpl::vector0<>, context>::type
    > type;
};

template<
    typename TScope
  , typename TExpected
  , typename TGiven
  , typename TContext0 = mpl_::na
  , typename TContext1 = mpl_::na
  , typename TContext2 = mpl_::na
>
struct dependency_base_of
{
    typedef mpl::vector<TContext0, TContext1, TContext2> context;
    typedef typename aux::dependency<
        TScope
      , TExpected
      , TGiven
      , typename mpl::if_<mpl::empty<context>, mpl::vector0<>, context>::type
      , mpl::or_<
            is_base_of<mpl::_1, TExpected>
          , is_same<mpl::_1, TExpected>
        >
    > type;
};

BOOST_AUTO_TEST_CASE(create_using_copy)
{
    factory< mpl::vector0<> > factory_;
    c0 obj = factory_.create<c0>();
    (void)(obj);
}

BOOST_AUTO_TEST_CASE(create_using_ptr)
{
    factory< mpl::vector0<> > factory_;
    c0* obj = factory_.create<c0*>();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(create_using_const_ptr)
{
    factory< mpl::vector0<> > factory_;
    const c0* obj = factory_.create<const c0*>();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(create_using_shared_ptr)
{
    factory< mpl::vector0<> > factory_;
    shared_ptr<c0> obj = factory_.create< shared_ptr<c0> >();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(create_default_ctor)
{
    factory< mpl::vector0<> > factory_;
    c0 obj = factory_.create<c0>();
    (void)(obj);
}

BOOST_AUTO_TEST_CASE(create_per_request)
{
    factory<
        mpl::vector<
            dependency<scopes::per_request, if0, c0if0>
        >
    > factory_;

    shared_ptr<c8> c8_ = factory_.create< shared_ptr<c8> >();

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

BOOST_AUTO_TEST_CASE(create_per_request_singleton)
{
    factory<
        mpl::vector<
            dependency<scopes::per_request, if0, c0if0>
          , dependency<scopes::singleton, c3>
        >
    > factory_;

    shared_ptr<c8> c8_ = factory_.create< shared_ptr<c8> >();

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

BOOST_AUTO_TEST_CASE(create_singleton_context)
{
    factory<
        mpl::vector<
            dependency<scopes::per_request, if0, c0if0>
          , dependency<scopes::per_request, if0, c1if0, concepts::call_stack<c6, c5> >
          , dependency<scopes::singleton, c3>
        >
    >
    factory_;

    shared_ptr<c8> c8_ = factory_.create< shared_ptr<c8> >();

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

BOOST_AUTO_TEST_CASE(create_per_request_singleton_context_order)
{
    factory<
        mpl::vector<
            dependency<scopes::per_request, if0, c0if0>
          , dependency<scopes::per_request, if0, c1if0, concepts::call_stack<c6, c5> >
          , dependency<scopes::per_request, if0, c2if0, c7>
          , dependency<scopes::singleton, c3>
        >
    > factory_;

    shared_ptr<c8> c8_ = factory_.create< shared_ptr<c8> >();

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

BOOST_AUTO_TEST_CASE(create_per_request_singleton_context_mix)
{
    factory<
        mpl::vector<
            dependency<scopes::per_request, if0, c0if0>
          , dependency<scopes::per_request, if0, c1if0, concepts::call_stack<c6, c5> >
          , dependency<scopes::per_request, if0, c2if0, c7>
          , dependency<scopes::singleton, c3>
          , dependency<scopes::per_request, int, mpl::int_<1> >
          , dependency<scopes::per_request, int, mpl::int_<2>, c8>
          , dependency<scopes::per_request, named<int, mpl::string<'1'> >, mpl::int_<3>, concepts::call_stack<c7, c6, c4> >
          , dependency<scopes::per_request, named<int, mpl::string<'2'> >, mpl::int_<4>, concepts::call_stack<c7, c6, c4> >
          , dependency<scopes::per_request, int, mpl::int_<5>, c2>
        >
    > factory_;

    shared_ptr<c8> c8_ = factory_.create< shared_ptr<c8> >();

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

BOOST_AUTO_TEST_CASE(create_singleton_impl)
{
    factory<
        mpl::vector<
            dependency<scopes::singleton, if0, c0if0>
        >
    > factory_;

    shared_ptr<c8> c8_ = factory_.create< shared_ptr<c8> >();

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

BOOST_AUTO_TEST_CASE(create_singleton_many)
{
    factory<
        mpl::vector<
            dependency<scopes::singleton, if0, c0if0>
          , dependency<scopes::singleton, c3>
          , dependency<scopes::singleton, c1>
        >
    > factory_;

    shared_ptr<c8> c8_ = factory_.create< shared_ptr<c8> >();

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

BOOST_AUTO_TEST_CASE(ctor_traits)
{
    const int i = 42;

    factory<
        mpl::vector<
            dependency_base_of<scopes::per_request, int, mpl::int_<i> >
        >
    > factory_;

    c14 obj = factory_.create<c14>();

    BOOST_CHECK_EQUAL(i, obj.i);
    BOOST_CHECK_EQUAL(0.0, obj.d);
}

BOOST_AUTO_TEST_CASE(class_ctor_traits)
{
    const int i1 = 42;
    const int i2 = 87;

    factory<
        mpl::vector<
            dependency<scopes::per_request, named<int, mpl::string<'1'> >, mpl::int_<i1> >
          , dependency<scopes::per_request, named<int, mpl::string<'2'> >, mpl::int_<i2> >
        >
    > factory_;

    c10 obj = factory_.create<c10>();

    BOOST_CHECK_EQUAL(i1, obj.i1);
    BOOST_CHECK_EQUAL(i2, obj.i2);
}

BOOST_AUTO_TEST_CASE(base_of)
{
    factory<
        mpl::vector<
            dependency_base_of<scopes::per_request, int, mpl::int_<1> >
          , dependency_base_of<scopes::per_request, named<int, mpl::string<'2'> >, mpl::int_<4>, concepts::call_stack<c7, c6, c4> >
          , dependency_base_of<scopes::per_request, int, mpl::int_<5>, c2>
          , dependency_base_of<scopes::per_request, c0if0, c0if0>
          , dependency_base_of<scopes::per_request, named<int, mpl::string<'1'> >, mpl::int_<3>, concepts::call_stack<c7, c6, c4> >
        >
    > factory_;

    shared_ptr<c8> c8_ = factory_.create< shared_ptr<c8> >();

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

BOOST_AUTO_TEST_CASE(base_of_interface_not_trivial_ctor)
{
    factory<
        mpl::vector<
            dependency_base_of<scopes::per_request, transaction_provider, transaction_provider>
        >
    > factory_;

    transaction_usage obj = factory_.create<transaction_usage>();

    BOOST_CHECK(obj.p->get().get() != obj.p->get().get());
}

BOOST_AUTO_TEST_CASE(named_shared_ptr_base_of)
{
    const int i = 42;

    factory<
        mpl::vector<
            dependency<scopes::per_request, named<int, mpl::string<'1'> >, mpl::int_<i> >
        >
    > factory_;

    c11 obj = factory_.create<c11>();

    BOOST_CHECK_EQUAL(i, *obj.i);
}

BOOST_AUTO_TEST_CASE(named_shared_ptr)
{
    const int i = 42;

    factory<
        mpl::vector<
            dependency_base_of<scopes::per_request, named<int, mpl::string<'1'> >, mpl::int_<i> >
        >
    > factory_;

    c11 obj = factory_.create<c11>();

    BOOST_CHECK_EQUAL(i, *obj.i);
}

BOOST_AUTO_TEST_CASE(named_shared_ptr_if)
{
    factory<
        mpl::vector<
            dependency_base_of<scopes::per_request, named<if0, mpl::string<'1'> >, c0if0>
        >
    > factory_;

    c12 obj = factory_.create<c12>();

    BOOST_CHECK(dynamic_cast<c0if0*>(obj.if0_.get()));
    BOOST_CHECK_EQUAL(0, obj.c2_->i);
    BOOST_CHECK_EQUAL(0.0, obj.c2_->d);
    BOOST_CHECK_EQUAL(0, obj.c2_->c);
}

BOOST_AUTO_TEST_CASE(named_shared_ptr_if_with_not_trivial_ctor)
{
    const int i = 42;

    factory<
        mpl::vector<
            dependency_base_of<scopes::per_request, named<if0>, c3if0>
          , dependency_base_of<scopes::per_request, int, mpl::int_<i> >
        >
    > factory_;

    c13 obj = factory_.create<c13>();

    c3if0* if0_ = dynamic_cast<c3if0*>(obj.if0_.get());
    BOOST_CHECK(if0_);
    BOOST_CHECK_EQUAL(i, obj.c3_.i);
    BOOST_CHECK_EQUAL(i, if0_->i);
    BOOST_CHECK_EQUAL(0.0, if0_->d);
}

BOOST_AUTO_TEST_CASE(externals_create_by_explicit_value)
{
    const int i = 42;
    const double d = 21.0;
    const char c = 'x';

    typedef aux::pool<
        mpl::vector<
            aux::instance<int>
          , aux::instance<double>
          , aux::instance<char>
        >
    > pool_t;

    scoped_ptr<pool_t> pool_(
        new pool_t(
            aux::instance<int>(i)
          , aux::instance<double>(d)
          , aux::instance<char>(c)
        )
    );

    factory<
        mpl::vector<
            dependency<scopes::per_request, std::string, mpl::string<'test'> >
        >
      , pool_t
    > factory_(*pool_);

    c9 obj = factory_.create<c9>();

    BOOST_CHECK_EQUAL(i, obj.i);
    BOOST_CHECK_EQUAL(d, obj.d);
    BOOST_CHECK_EQUAL(c, obj.c);
    BOOST_CHECK_EQUAL("test", obj.s);
}

BOOST_AUTO_TEST_CASE(externals_create_with_non_trivial_ctor)
{
    const int i = 42;
    const double d = 21.0;
    const char c = 'x';

    typedef aux::pool< mpl::vector<aux::instance<c2> > > pool;

    scoped_ptr<pool> pool_(
        new pool(
            aux::instance<c2>(make_shared<c2>(i, d, c))
        )
    );

    factory<
        mpl::vector<
            dependency<scopes::per_request, c2>
        >
      , pool
    > factory_(*pool_);

    c2 obj = factory_.create<c2>();

    BOOST_CHECK_EQUAL(i, obj.i);
    BOOST_CHECK_EQUAL(d, obj.d);
    BOOST_CHECK_EQUAL(c, obj.c);
}

#if 0
BOOST_AUTO_TEST_CASE(externals_create_with_attributes)
{
    const int i1 = 42;
    const int i2 = 87;

    typedef named<int, mpl::string<'1'> > named1;
    typedef named<int, mpl::string<'2'> > named2;
    typedef aux::pool< mpl::vector<aux::instance<named1>, aux::instance<named2> > > pool;

    scoped_ptr<pool> pool_(
        new pool(
            aux::instance<named1>(make_shared<named1>(i1)),
            aux::instance<named2>(make_shared<named2>(i2))
        )
    );

    factory<
        vector<
            dependency<scopes::per_request, named1, int>
          , dependency<scopes::per_request, named2, int>
        >
      , pool
    > factory_(*pool_);

    c4 obj = factory_.create<c4>();

    BOOST_CHECK_EQUAL(i1, obj.i1);
    BOOST_CHECK_EQUAL(i2, obj.i2);
}
#endif

} // namespace detail
} // namespace di
} // namespace boost

