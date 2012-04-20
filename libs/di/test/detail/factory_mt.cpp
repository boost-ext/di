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
#include "boost/di/named.hpp"
#include "boost/di/scopes/per_request.hpp"
#include "boost/di/scopes/singleton.hpp"
#include "boost/di/aux/dependency.hpp"
#include "boost/di/aux/instance.hpp"
#include "boost/di/aux/pool.hpp"
#include "boost/di/detail/factory.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace test {

BOOST_AUTO_TEST_CASE(create_using_copy)
{
    detail::factory< mpl::vector0<> > factory;
    c0 obj = factory.create<c0>();
    (void)(obj);
}

BOOST_AUTO_TEST_CASE(create_using_ptr)
{
    detail::factory< mpl::vector0<> > factory;
    c0* obj = factory.create<c0*>();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(create_using_const_ptr)
{
    detail::factory< mpl::vector0<> > factory;
    const c0* obj = factory.create<const c0*>();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(create_using_shared_ptr)
{
    detail::factory< mpl::vector0<> > factory;
    shared_ptr<c0> obj = factory.create< shared_ptr<c0> >();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(create_default_ctor)
{
    detail::factory< mpl::vector0<> > factory;
    c0 obj = factory.create<c0>();
    (void)(obj);
}

BOOST_AUTO_TEST_CASE(create_per_request)
{
    detail::factory<
        mpl::vector<
            aux::dependency<scopes::per_request, if0, c0if0>
        >
    > factory;

    shared_ptr<c8> c8_ = factory.create< shared_ptr<c8> >();

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
    detail::factory<
        mpl::vector<
            aux::dependency<scopes::per_request, if0, c0if0>
          , aux::dependency<scopes::singleton, c3>
        >
    > factory;

    shared_ptr<c8> c8_ = factory.create< shared_ptr<c8> >();

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

BOOST_AUTO_TEST_CASE(create_singleton_path)
{
    detail::factory<
        mpl::vector<
            aux::dependency<scopes::per_request, if0, c0if0>
          , aux::dependency<scopes::per_request, if0, c1if0, mpl::vector<c6, c5> >
          , aux::dependency<scopes::singleton, c3>
        >
    >
    factory;

    shared_ptr<c8> c8_ = factory.create< shared_ptr<c8> >();

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

BOOST_AUTO_TEST_CASE(create_per_request_singleton_path_order)
{
    detail::factory<
        mpl::vector<
            aux::dependency<scopes::per_request, if0, c0if0>
          , aux::dependency<scopes::per_request, if0, c1if0, mpl::vector<c6, c5> >
          , aux::dependency<scopes::per_request, if0, c2if0, mpl::vector<c7> >
          , aux::dependency<scopes::singleton, c3>
        >
    > factory;

    shared_ptr<c8> c8_ = factory.create< shared_ptr<c8> >();

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

BOOST_AUTO_TEST_CASE(create_per_request_singleton_path_mix)
{
    detail::factory<
        mpl::vector<
            aux::dependency<scopes::per_request, if0, c0if0>
          , aux::dependency<scopes::per_request, if0, c1if0, mpl::vector<c6, c5> >
          , aux::dependency<scopes::per_request, if0, c2if0, mpl::vector<c7> >
          , aux::dependency<scopes::singleton, c3>
          , aux::dependency<scopes::per_request, int, mpl::int_<1> >
          , aux::dependency<scopes::per_request, int, mpl::int_<2>, mpl::vector<c8> >
          , aux::dependency<scopes::per_request, named<int, mpl::string<'1'> >, mpl::int_<3>, mpl::vector<c7, c6, c4> >
          , aux::dependency<scopes::per_request, named<int, mpl::string<'2'> >, mpl::int_<4>, mpl::vector<c7, c6, c4> >
          , aux::dependency<scopes::per_request, int, mpl::int_<5>, mpl::vector<c2> >
        >
    > factory;

    shared_ptr<c8> c8_ = factory.create< shared_ptr<c8> >();

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
    detail::factory<
        mpl::vector<
            aux::dependency<scopes::singleton, if0, c0if0>
        >
    > factory;

    shared_ptr<c8> c8_ = factory.create< shared_ptr<c8> >();

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
    detail::factory<
        mpl::vector<
            aux::dependency<scopes::singleton, if0, c0if0>
          , aux::dependency<scopes::singleton, c3>
          , aux::dependency<scopes::singleton, c1>
        >
    > factory;

    shared_ptr<c8> c8_ = factory.create< shared_ptr<c8> >();

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

    detail::factory<
        mpl::vector<
            aux::dependency<scopes::per_request, int, mpl::int_<i>, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, int>, is_same<mpl::_1, int> > >
        >
    > factory;

    c14 obj = factory.create<c14>();

    BOOST_CHECK_EQUAL(i, obj.i);
    BOOST_CHECK_EQUAL(0.0, obj.d);
}

BOOST_AUTO_TEST_CASE(class_ctor_traits)
{
    const int i1 = 42;
    const int i2 = 87;

    detail::factory<
        mpl::vector<
            aux::dependency<scopes::per_request, named<int, mpl::string<'1'> >, mpl::int_<i1> >
          , aux::dependency<scopes::per_request, named<int, mpl::string<'2'> >, mpl::int_<i2> >
        >
    > factory;

    c10 obj = factory.create<c10>();

    BOOST_CHECK_EQUAL(i1, obj.i1);
    BOOST_CHECK_EQUAL(i2, obj.i2);
}

BOOST_AUTO_TEST_CASE(base_of)
{
    detail::factory<
        mpl::vector<
            aux::dependency<scopes::per_request, int, mpl::int_<1>, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, int>, is_same<mpl::_1, int> > >
          , aux::dependency<scopes::per_request, named<int, mpl::string<'2'> >, mpl::int_<4>, mpl::vector<c7, c6, c4>, mpl::or_< is_base_of<mpl::_1, named<int, mpl::string<'2'> > >, is_same<mpl::_1, named<int, mpl::string<'2'> > > > >
          , aux::dependency<scopes::per_request, int, mpl::int_<5>, mpl::vector<c2>, mpl::or_< is_base_of<mpl::_1, int>, is_same<mpl::_1, int> > >
          , aux::dependency<scopes::per_request, c0if0, c0if0, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, c0if0>, is_same<mpl::_1, c0if0> > >
          , aux::dependency<scopes::per_request, named<int, mpl::string<'1'> >, mpl::int_<3>, mpl::vector<c7, c6, c4>, mpl::or_< is_base_of<mpl::_1, named<int, mpl::string<'1'> > >, is_same<mpl::_1, named<int, mpl::string<'1'> > > > >
        >
    > factory;

    shared_ptr<c8> c8_ = factory.create< shared_ptr<c8> >();

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
    detail::factory<
        mpl::vector<
            aux::dependency<scopes::per_request, transaction_provider, transaction_provider, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, transaction_provider>, is_same<mpl::_1, transaction_provider> > >
        >
    > factory;

    transaction_usage obj = factory.create<transaction_usage>();

    BOOST_CHECK(obj.p->get().get() != obj.p->get().get());
}

BOOST_AUTO_TEST_CASE(named_shared_ptr_base_of)
{
    const int i = 42;

    detail::factory<
        mpl::vector<
            aux::dependency<scopes::per_request, named<int, mpl::string<'1'> >, mpl::int_<i>, mpl::vector0<> >
        >
    > factory;

    c11 obj = factory.create<c11>();

    BOOST_CHECK_EQUAL(i, *obj.i);
}

BOOST_AUTO_TEST_CASE(named_shared_ptr)
{
    const int i = 42;

    detail::factory<
        mpl::vector<
            aux::dependency<scopes::per_request, named<int, mpl::string<'1'> >, mpl::int_<i>, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, named<int, mpl::string<'1'> > >, is_same<mpl::_1, named<int, mpl::string<'1'> > > > >
        >
    > factory;

    c11 obj = factory.create<c11>();

    BOOST_CHECK_EQUAL(i, *obj.i);
}

BOOST_AUTO_TEST_CASE(named_shared_ptr_if)
{
    detail::factory<
        mpl::vector<
            aux::dependency<scopes::per_request, named<if0, mpl::string<'1'> >, c0if0, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, named<if0, mpl::string<'1'> > >, is_same<mpl::_1, named<if0, mpl::string<'1'> > > > >
        >
    > factory;

    c12 obj = factory.create<c12>();

    BOOST_CHECK(dynamic_cast<c0if0*>(obj.if0_.get()));
    BOOST_CHECK_EQUAL(0, obj.c2_->i);
    BOOST_CHECK_EQUAL(0.0, obj.c2_->d);
    BOOST_CHECK_EQUAL(0, obj.c2_->c);
}

BOOST_AUTO_TEST_CASE(named_shared_ptr_if_with_not_trivial_ctor)
{
    const int i = 42;

    detail::factory<
        mpl::vector<
            aux::dependency<scopes::per_request, named<if0>, c3if0, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, named<if0> >, is_same<mpl::_1, named<if0> > > >
          , aux::dependency<scopes::per_request, int, mpl::int_<i>, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, int>, is_same<mpl::_1, int> > >
        >
    > factory;

    c13 obj = factory.create<c13>();

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

    typedef aux::pool< mpl::vector<aux::instance<int>, aux::instance<double>, aux::instance<char> > > pool;

    scoped_ptr<pool> test_pool(
        new pool(
            aux::instance<int>(i),
            aux::instance<double>(d),
            aux::instance<char>(c)
        )
    );

    detail::factory<
        mpl::vector<
            aux::dependency<scopes::per_request, std::string, mpl::string<'test'> >
        >
      , pool
    > factory(*test_pool);

    c9 obj = factory.create<c9>();

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

    scoped_ptr<pool> test_pool(
        new pool(
            aux::instance<c2>(make_shared<c2>(i, d, c))
        )
    );

    detail::factory<
        mpl::vector<
            aux::dependency<scopes::per_request, c2>
        >
      , pool
    > factory(*test_pool);

    c2 obj = factory.create<c2>();

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

    scoped_ptr<pool> test_pool(
        new pool(
            aux::instance<named1>(make_shared<named1>(i1)),
            aux::instance<named2>(make_shared<named2>(i2))
        )
    );

    factory<
        vector<
            aux::dependency<scopes::per_request, named1, int>
          , aux::dependency<scopes::per_request, named2, int>
        >
      , pool
    > factory(*test_pool);

    c4 obj = factory.create<c4>();

    BOOST_CHECK_EQUAL(i1, obj.i1);
    BOOST_CHECK_EQUAL(i2, obj.i2);
}
#endif

} // namespace test
} // namespace di
} // namespace boost

