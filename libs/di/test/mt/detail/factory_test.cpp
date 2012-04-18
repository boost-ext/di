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
#include "test/common/data.hpp"
#include "di/utility/named.hpp"
#include "di/back/scopes/per_request.hpp"
#include "di/back/scopes/singleton.hpp"
#include "di/back/aux/dependency.hpp"
#include "di/back/aux/instance.hpp"
#include "di/back/aux/pool.hpp"
#include "di/back/factory.hpp"

namespace di
{
namespace back
{
namespace mt
{

using namespace test::common;
using namespace utility;
using namespace scopes;
using namespace aux;
using namespace boost::mpl;
using namespace boost;

BOOST_AUTO_TEST_CASE(CreateUsingCopy)
{
    factory< vector0<> > factory;
    c0 obj = factory.create<c0>();
    (void)(obj);
}

BOOST_AUTO_TEST_CASE(CreateUsingPtr)
{
    factory< vector0<> > factory;
    c0* obj = factory.create<c0*>();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(CreateUsingConstPtr)
{
    factory< vector0<> > factory;
    const c0* obj = factory.create<const c0*>();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(CreateUsingSharedPtr)
{
    factory< vector0<> > factory;
    shared_ptr<c0> obj = factory.create< shared_ptr<c0> >();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(CreateDefaultctor)
{
    factory< vector0<> > factory;
    c0 obj = factory.create<c0>();
    (void)(obj);
}

BOOST_AUTO_TEST_CASE(Createper_request)
{
    factory
    <
        vector
        <
            dependency<per_request, if0, c0if0>
        >
    >
    factory;

    shared_ptr<c8> c8_ = factory.create< shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ != c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0 != c8_->c7_->c6_->c5_.if0);

    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->c6_->c5_.if0.get()));
    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0.get()));

    BOOST_CHECK_EQUAL(0, c8_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
}

BOOST_AUTO_TEST_CASE(Createper_requestsingleton)
{
    factory
    <
        vector
        <
            dependency<per_request, if0, c0if0>,
            dependency<singleton, c3>
        >
    >
    factory;

    shared_ptr<c8> c8_ = factory.create< shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0 != c8_->c7_->c6_->c5_.if0);

    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->c6_->c5_.if0.get()));
    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0.get()));

    BOOST_CHECK_EQUAL(0, c8_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
}

BOOST_AUTO_TEST_CASE(Createper_requestsingletonPath)
{
    factory
    <
        vector
        <
            dependency<per_request, if0, c0if0>,
            dependency<per_request, if0, c1if0, vector<c6, c5> >,
            dependency<singleton, c3>
        >
    >
    factory;

    shared_ptr<c8> c8_ = factory.create< shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0 != c8_->c7_->c6_->c5_.if0);

    BOOST_CHECK(dynamic_cast<c1if0*>(c8_->c7_->c6_->c5_.if0.get()));
    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0.get()));

    BOOST_CHECK_EQUAL(0, c8_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
}

BOOST_AUTO_TEST_CASE(Createper_requestsingletonPathOrder)
{
    factory
    <
        vector
        <
            dependency<per_request, if0, c0if0>,
            dependency<per_request, if0, c1if0, vector<c6, c5> >,
            dependency<per_request, if0, c2if0, vector<c7> >,
            dependency<singleton, c3>
        >
    >
    factory;

    shared_ptr<c8> c8_ = factory.create< shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0 != c8_->c7_->c6_->c5_.if0);

    BOOST_CHECK(dynamic_cast<c1if0*>(c8_->c7_->c6_->c5_.if0.get()));
    BOOST_CHECK(dynamic_cast<c2if0*>(c8_->c7_->if0.get()));

    BOOST_CHECK_EQUAL(0, c8_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
}

BOOST_AUTO_TEST_CASE(Createper_requestsingletonPathMix)
{
    factory
    <
        vector
        <
            dependency<per_request, if0, c0if0>,
            dependency<per_request, if0, c1if0, vector<c6, c5> >,
            dependency<per_request, if0, c2if0, vector<c7> >,
            dependency<singleton, c3>,
            dependency<per_request, int, int_<1> >,
            dependency<per_request, int, int_<2>, vector<c8> >,
            dependency<per_request, named<int, string<'1'> >, int_<3>, vector<c7, c6, c4> >,
            dependency<per_request, named<int, string<'2'> >, int_<4>, vector<c7, c6, c4> >,
            dependency<per_request, int, int_<5>, vector<c2> >
        >
    >
    factory;

    shared_ptr<c8> c8_ = factory.create< shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0 != c8_->c7_->c6_->c5_.if0);

    BOOST_CHECK(dynamic_cast<c1if0*>(c8_->c7_->c6_->c5_.if0.get()));
    BOOST_CHECK(dynamic_cast<c2if0*>(c8_->c7_->if0.get()));

    BOOST_CHECK_EQUAL(2, c8_->i);
    BOOST_CHECK_EQUAL(3, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(4, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(1, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(5, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
}

BOOST_AUTO_TEST_CASE(CreatesingletonImpl)
{
    factory
    <
        vector
        <
            dependency<singleton, if0, c0if0>
        >
    >
    factory;

    shared_ptr<c8> c8_ = factory.create< shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ != c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0 == c8_->c7_->c6_->c5_.if0);

    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->c6_->c5_.if0.get()));
    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0.get()));

    BOOST_CHECK_EQUAL(0, c8_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
}

BOOST_AUTO_TEST_CASE(CreatesingletonMany)
{
    factory
    <
        vector
        <
            dependency<singleton, if0, c0if0>,
            dependency<singleton, c3>,
            dependency<singleton, c1>
        >
    >
    factory;

    shared_ptr<c8> c8_ = factory.create< shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ == c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ == c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0 == c8_->c7_->c6_->c5_.if0);

    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->c6_->c5_.if0.get()));
    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0.get()));

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

    factory
    <
        vector
        <
            dependency<per_request, int, int_<i>, vector0<>, or_< is_base_of<_1, int>, is_same<_1, int> > >
        >
    >
    factory;

    c14_ obj = factory.create<c14_>();

    BOOST_CHECK_EQUAL(i, obj.i);
    BOOST_CHECK_EQUAL(0.0, obj.d);
}

BOOST_AUTO_TEST_CASE(Classctor_traits)
{
    const int i1 = 42;
    const int i2 = 87;

    factory
    <
        vector
        <
            dependency<per_request, named<int, string<'1'> >, int_<i1> >,
            dependency<per_request, named<int, string<'2'> >, int_<i2> >
        >
    >
    factory;

    c10 obj = factory.create<c10>();

    BOOST_CHECK_EQUAL(i1, obj.i1);
    BOOST_CHECK_EQUAL(i2, obj.i2);
}

BOOST_AUTO_TEST_CASE(baseOf)
{
    factory
    <
        vector
        <
            dependency<per_request, int, int_<1>, vector0<>, or_< is_base_of<_1, int>, is_same<_1, int> > >,
            dependency<per_request, named<int, string<'2'> >, int_<4>, vector<c7, c6, c4>, or_< is_base_of<_1, named<int, string<'2'> > >, is_same<_1, named<int, string<'2'> > > > >,
            dependency<per_request, int, int_<5>, vector<c2>, or_< is_base_of<_1, int>, is_same<_1, int> > >,
            dependency<per_request, c0if0, c0if0, vector0<>, or_< is_base_of<_1, c0if0>, is_same<_1, c0if0> > >,
            dependency<per_request, named<int, string<'1'> >, int_<3>, vector<c7, c6, c4>, or_< is_base_of<_1, named<int, string<'1'> > >, is_same<_1, named<int, string<'1'> > > > >
        >
    >
    factory;

    shared_ptr<c8> c8_ = factory.create< shared_ptr<c8> >();

    BOOST_CHECK(c8_->c1_ != c8_->c7_->c6_->c5_.c1_);
    BOOST_CHECK(c8_->c7_->c6_->c4_->c3_ != c8_->c7_->c6_->c3_);
    BOOST_CHECK(c8_->c7_->if0 != c8_->c7_->c6_->c5_.if0);

    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->c6_->c5_.if0.get()));
    BOOST_CHECK(dynamic_cast<c0if0*>(c8_->c7_->if0.get()));

    BOOST_CHECK_EQUAL(1, c8_->i);
    BOOST_CHECK_EQUAL(3, c8_->c7_->c6_->c4_->i1);
    BOOST_CHECK_EQUAL(4, c8_->c7_->c6_->c4_->i2);
    BOOST_CHECK_EQUAL(1, c8_->c7_->c6_->c3_->i);
    BOOST_CHECK_EQUAL(5, c8_->c7_->c6_->c5_.c2_->i);
    BOOST_CHECK_EQUAL(0.0, c8_->c7_->c6_->c5_.c2_->d);
    BOOST_CHECK_EQUAL(0, c8_->c7_->c6_->c5_.c2_->c);
}

BOOST_AUTO_TEST_CASE(baseOfInterfaceNotTrivialctor)
{
    factory
    <
        vector
        <
            dependency<per_request, transaction_provider, transaction_provider, vector0<>, or_< is_base_of<_1, transaction_provider>, is_same<_1, transaction_provider> > >
        >
    >
    factory;

    transaction_usage obj = factory.create<transaction_usage>();

    BOOST_CHECK(obj.p->get().get() != obj.p->get().get());
}

BOOST_AUTO_TEST_CASE(namedSharedPtrbaseOf)
{
    const int i = 42;

    factory
    <
        vector
        <
            dependency<per_request, named<int, string<'1'> >, int_<i>, vector0<> >
        >
    >
    factory;

    c11 obj = factory.create<c11>();

    BOOST_CHECK_EQUAL(i, *obj.i);
}

BOOST_AUTO_TEST_CASE(namedSharedPtr)
{
    const int i = 42;

    factory
    <
        vector
        <
            dependency<per_request, named<int, string<'1'> >, int_<i>, vector0<>, or_< is_base_of<_1, named<int, string<'1'> > >, is_same<_1, named<int, string<'1'> > > > >
        >
    >
    factory;

    c11 obj = factory.create<c11>();

    BOOST_CHECK_EQUAL(i, *obj.i);
}

BOOST_AUTO_TEST_CASE(namedSharedPtrIf)
{
    factory
    <
        vector
        <
            dependency<per_request, named<if0, string<'1'> >, c0if0, vector0<>, or_< is_base_of<_1, named<if0, string<'1'> > >, is_same<_1, named<if0, string<'1'> > > > >
        >
    >
    factory;

    c12 obj = factory.create<c12>();

    BOOST_CHECK(dynamic_cast<c0if0*>(obj.if0.get()));
    BOOST_CHECK_EQUAL(0, obj.c2_->i);
    BOOST_CHECK_EQUAL(0.0, obj.c2_->d);
    BOOST_CHECK_EQUAL(0, obj.c2_->c);
}

BOOST_AUTO_TEST_CASE(namedSharedPtrIfwithNotTrivialctor)
{
    const int i = 42;

    factory
    <
        vector
        <
            dependency<per_request, named<if0>, c2if0, vector0<>, or_< is_base_of<_1, named<if0> >, is_same<_1, named<if0> > > >,
            dependency<per_request, int, int_<i>, vector0<>, or_< is_base_of<_1, int>, is_same<_1, int> > >
        >
    >
    factory;

    c13 obj = factory.create<c13>();

    c2if0* if0 = dynamic_cast<c2if0*>(obj.if0.get());
    BOOST_CHECK(if0);
    BOOST_CHECK_EQUAL(i, obj.c3_.i);
    BOOST_CHECK_EQUAL(i, if0->i);
    BOOST_CHECK_EQUAL(0.0, if0->d);
}

BOOST_AUTO_TEST_CASE(externalsCreateByvalues)
{
    const int i = 42;
    const double d = 21.0;
    const char c = 'x';

    typedef pool< vector<instance<int>, instance<double>, instance<char> > > pool;

    boost::scoped_ptr<pool> test_pool(
        new pool(
            instance<int>(i),
            instance<double>(d),
            instance<char>(c)
        )
    );

    factory
    <
        vector
        <
            dependency<per_request, std::string, string<'test'> >
        >,
        pool
    >
    factory(*test_pool);

    c9 obj = factory.create<c9>();

    BOOST_CHECK_EQUAL(i, obj.i);
    BOOST_CHECK_EQUAL(d, obj.d);
    BOOST_CHECK_EQUAL(c, obj.c);
    BOOST_CHECK_EQUAL("test", obj.s);
}

BOOST_AUTO_TEST_CASE(externalsCreatewithNonTrivialctor)
{
    const int i = 42;
    const double d = 21.0;
    const char c = 'x';

    typedef pool< vector<instance<c2> > > pool;

    boost::scoped_ptr<pool> test_pool(
        new pool(
            instance<c2>(make_shared<c2>(i, d, c))
        )
    );

    factory
    <
        vector
        <
            dependency<per_request, c2>
        >,
        pool
    >
    factory(*test_pool);

    c2 obj = factory.create<c2>();

    BOOST_CHECK_EQUAL(i, obj.i);
    BOOST_CHECK_EQUAL(d, obj.d);
    BOOST_CHECK_EQUAL(c, obj.c);
}

#if 0
BOOST_AUTO_TEST_CASE(externalsCreatewithAttributes)
{
    const int i1 = 42;
    const int i2 = 87;

    typedef named<int, string<'1'> > named1;
    typedef named<int, string<'2'> > named2;
    typedef pool< vector<instance<named1>, instance<named2> > > pool;

    boost::scoped_ptr<pool> test_pool(
        new pool(
            instance<named1>(make_shared<named1>(i1)),
            instance<named2>(make_shared<named2>(i2))
        )
    );

    factory
    <
        vector
        <
            dependency<per_request, named1, int>,
            dependency<per_request, named2, int>
        >,
        pool
    >
    factory(*test_pool);

    c4 obj = factory.create<c4>();

    BOOST_CHECK_EQUAL(i1, obj.i1);
    BOOST_CHECK_EQUAL(i2, obj.i2);
}
#endif

} // namespace mt
} // namespace back
} // namespace di

