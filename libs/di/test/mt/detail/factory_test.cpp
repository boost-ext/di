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
    C0 obj = factory.create<C0>();
    (void)(obj);
}

BOOST_AUTO_TEST_CASE(CreateUsingPtr)
{
    factory< vector0<> > factory;
    C0* obj = factory.create<C0*>();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(CreateUsingConstPtr)
{
    factory< vector0<> > factory;
    const C0* obj = factory.create<const C0*>();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(CreateUsingSharedPtr)
{
    factory< vector0<> > factory;
    shared_ptr<C0> obj = factory.create< shared_ptr<C0> >();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(CreateDefaultctor)
{
    factory< vector0<> > factory;
    C0 obj = factory.create<C0>();
    (void)(obj);
}

BOOST_AUTO_TEST_CASE(Createper_request)
{
    factory
    <
        vector
        <
            dependency<per_request, If0, CIf0>
        >
    >
    factory;

    shared_ptr<C8> c8 = factory.create< shared_ptr<C8> >();

    BOOST_CHECK(c8->c1 != c8->c7->c6->c5.c1);
    BOOST_CHECK(c8->c7->c6->c4->c3 != c8->c7->c6->c3);
    BOOST_CHECK(c8->c7->if0 != c8->c7->c6->c5.if0);

    BOOST_CHECK(dynamic_cast<CIf0*>(c8->c7->c6->c5.if0.get()));
    BOOST_CHECK(dynamic_cast<CIf0*>(c8->c7->if0.get()));

    BOOST_CHECK_EQUAL(0, c8->i);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c4->i1);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c4->i2);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c3->i);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c5.c2->i);
    BOOST_CHECK_EQUAL(0.0, c8->c7->c6->c5.c2->d);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c5.c2->c);
}

BOOST_AUTO_TEST_CASE(Createper_requestsingleton)
{
    factory
    <
        vector
        <
            dependency<per_request, If0, CIf0>,
            dependency<singleton, C3>
        >
    >
    factory;

    shared_ptr<C8> c8 = factory.create< shared_ptr<C8> >();

    BOOST_CHECK(c8->c1 != c8->c7->c6->c5.c1);
    BOOST_CHECK(c8->c7->c6->c4->c3 == c8->c7->c6->c3);
    BOOST_CHECK(c8->c7->if0 != c8->c7->c6->c5.if0);

    BOOST_CHECK(dynamic_cast<CIf0*>(c8->c7->c6->c5.if0.get()));
    BOOST_CHECK(dynamic_cast<CIf0*>(c8->c7->if0.get()));

    BOOST_CHECK_EQUAL(0, c8->i);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c4->i1);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c4->i2);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c3->i);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c5.c2->i);
    BOOST_CHECK_EQUAL(0.0, c8->c7->c6->c5.c2->d);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c5.c2->c);
}

BOOST_AUTO_TEST_CASE(Createper_requestsingletonPath)
{
    factory
    <
        vector
        <
            dependency<per_request, If0, CIf0>,
            dependency<per_request, If0, CIf01, vector<C6, C5> >,
            dependency<singleton, C3>
        >
    >
    factory;

    shared_ptr<C8> c8 = factory.create< shared_ptr<C8> >();

    BOOST_CHECK(c8->c1 != c8->c7->c6->c5.c1);
    BOOST_CHECK(c8->c7->c6->c4->c3 == c8->c7->c6->c3);
    BOOST_CHECK(c8->c7->if0 != c8->c7->c6->c5.if0);

    BOOST_CHECK(dynamic_cast<CIf01*>(c8->c7->c6->c5.if0.get()));
    BOOST_CHECK(dynamic_cast<CIf0*>(c8->c7->if0.get()));

    BOOST_CHECK_EQUAL(0, c8->i);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c4->i1);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c4->i2);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c3->i);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c5.c2->i);
    BOOST_CHECK_EQUAL(0.0, c8->c7->c6->c5.c2->d);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c5.c2->c);
}

BOOST_AUTO_TEST_CASE(Createper_requestsingletonPathOrder)
{
    factory
    <
        vector
        <
            dependency<per_request, If0, CIf0>,
            dependency<per_request, If0, CIf01, vector<C6, C5> >,
            dependency<per_request, If0, CIf02, vector<C7> >,
            dependency<singleton, C3>
        >
    >
    factory;

    shared_ptr<C8> c8 = factory.create< shared_ptr<C8> >();

    BOOST_CHECK(c8->c1 != c8->c7->c6->c5.c1);
    BOOST_CHECK(c8->c7->c6->c4->c3 == c8->c7->c6->c3);
    BOOST_CHECK(c8->c7->if0 != c8->c7->c6->c5.if0);

    BOOST_CHECK(dynamic_cast<CIf01*>(c8->c7->c6->c5.if0.get()));
    BOOST_CHECK(dynamic_cast<CIf02*>(c8->c7->if0.get()));

    BOOST_CHECK_EQUAL(0, c8->i);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c4->i1);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c4->i2);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c3->i);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c5.c2->i);
    BOOST_CHECK_EQUAL(0.0, c8->c7->c6->c5.c2->d);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c5.c2->c);
}

BOOST_AUTO_TEST_CASE(Createper_requestsingletonPathMix)
{
    factory
    <
        vector
        <
            dependency<per_request, If0, CIf0>,
            dependency<per_request, If0, CIf01, vector<C6, C5> >,
            dependency<per_request, If0, CIf02, vector<C7> >,
            dependency<singleton, C3>,
            dependency<per_request, int, int_<1> >,
            dependency<per_request, int, int_<2>, vector<C8> >,
            dependency<per_request, named<int, string<'1'> >, int_<3>, vector<C7, C6, C4> >,
            dependency<per_request, named<int, string<'2'> >, int_<4>, vector<C7, C6, C4> >,
            dependency<per_request, int, int_<5>, vector<C2> >
        >
    >
    factory;

    shared_ptr<C8> c8 = factory.create< shared_ptr<C8> >();

    BOOST_CHECK(c8->c1 != c8->c7->c6->c5.c1);
    BOOST_CHECK(c8->c7->c6->c4->c3 == c8->c7->c6->c3);
    BOOST_CHECK(c8->c7->if0 != c8->c7->c6->c5.if0);

    BOOST_CHECK(dynamic_cast<CIf01*>(c8->c7->c6->c5.if0.get()));
    BOOST_CHECK(dynamic_cast<CIf02*>(c8->c7->if0.get()));

    BOOST_CHECK_EQUAL(2, c8->i);
    BOOST_CHECK_EQUAL(3, c8->c7->c6->c4->i1);
    BOOST_CHECK_EQUAL(4, c8->c7->c6->c4->i2);
    BOOST_CHECK_EQUAL(1, c8->c7->c6->c3->i);
    BOOST_CHECK_EQUAL(5, c8->c7->c6->c5.c2->i);
    BOOST_CHECK_EQUAL(0.0, c8->c7->c6->c5.c2->d);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c5.c2->c);
}

BOOST_AUTO_TEST_CASE(CreatesingletonImpl)
{
    factory
    <
        vector
        <
            dependency<singleton, If0, CIf0>
        >
    >
    factory;

    shared_ptr<C8> c8 = factory.create< shared_ptr<C8> >();

    BOOST_CHECK(c8->c1 != c8->c7->c6->c5.c1);
    BOOST_CHECK(c8->c7->c6->c4->c3 != c8->c7->c6->c3);
    BOOST_CHECK(c8->c7->if0 == c8->c7->c6->c5.if0);

    BOOST_CHECK(dynamic_cast<CIf0*>(c8->c7->c6->c5.if0.get()));
    BOOST_CHECK(dynamic_cast<CIf0*>(c8->c7->if0.get()));

    BOOST_CHECK_EQUAL(0, c8->i);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c4->i1);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c4->i2);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c3->i);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c5.c2->i);
    BOOST_CHECK_EQUAL(0.0, c8->c7->c6->c5.c2->d);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c5.c2->c);
}

BOOST_AUTO_TEST_CASE(CreatesingletonMany)
{
    factory
    <
        vector
        <
            dependency<singleton, If0, CIf0>,
            dependency<singleton, C3>,
            dependency<singleton, C1>
        >
    >
    factory;

    shared_ptr<C8> c8 = factory.create< shared_ptr<C8> >();

    BOOST_CHECK(c8->c1 == c8->c7->c6->c5.c1);
    BOOST_CHECK(c8->c7->c6->c4->c3 == c8->c7->c6->c3);
    BOOST_CHECK(c8->c7->if0 == c8->c7->c6->c5.if0);

    BOOST_CHECK(dynamic_cast<CIf0*>(c8->c7->c6->c5.if0.get()));
    BOOST_CHECK(dynamic_cast<CIf0*>(c8->c7->if0.get()));

    BOOST_CHECK_EQUAL(0, c8->i);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c4->i1);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c4->i2);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c3->i);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c5.c2->i);
    BOOST_CHECK_EQUAL(0.0, c8->c7->c6->c5.c2->d);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c5.c2->c);
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

    C14 obj = factory.create<C14>();

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

    C10 obj = factory.create<C10>();

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
            dependency<per_request, named<int, string<'2'> >, int_<4>, vector<C7, C6, C4>, or_< is_base_of<_1, named<int, string<'2'> > >, is_same<_1, named<int, string<'2'> > > > >,
            dependency<per_request, int, int_<5>, vector<C2>, or_< is_base_of<_1, int>, is_same<_1, int> > >,
            dependency<per_request, CIf0, CIf0, vector0<>, or_< is_base_of<_1, CIf0>, is_same<_1, CIf0> > >,
            dependency<per_request, named<int, string<'1'> >, int_<3>, vector<C7, C6, C4>, or_< is_base_of<_1, named<int, string<'1'> > >, is_same<_1, named<int, string<'1'> > > > >
        >
    >
    factory;

    shared_ptr<C8> c8 = factory.create< shared_ptr<C8> >();

    BOOST_CHECK(c8->c1 != c8->c7->c6->c5.c1);
    BOOST_CHECK(c8->c7->c6->c4->c3 != c8->c7->c6->c3);
    BOOST_CHECK(c8->c7->if0 != c8->c7->c6->c5.if0);

    BOOST_CHECK(dynamic_cast<CIf0*>(c8->c7->c6->c5.if0.get()));
    BOOST_CHECK(dynamic_cast<CIf0*>(c8->c7->if0.get()));

    BOOST_CHECK_EQUAL(1, c8->i);
    BOOST_CHECK_EQUAL(3, c8->c7->c6->c4->i1);
    BOOST_CHECK_EQUAL(4, c8->c7->c6->c4->i2);
    BOOST_CHECK_EQUAL(1, c8->c7->c6->c3->i);
    BOOST_CHECK_EQUAL(5, c8->c7->c6->c5.c2->i);
    BOOST_CHECK_EQUAL(0.0, c8->c7->c6->c5.c2->d);
    BOOST_CHECK_EQUAL(0, c8->c7->c6->c5.c2->c);
}

BOOST_AUTO_TEST_CASE(baseOfInterfaceNotTrivialctor)
{
    factory
    <
        vector
        <
            dependency<per_request, Transactionprovider, Transactionprovider, vector0<>, or_< is_base_of<_1, Transactionprovider>, is_same<_1, Transactionprovider> > >
        >
    >
    factory;

    TransactionUsage obj = factory.create<TransactionUsage>();

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

    C11 obj = factory.create<C11>();

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

    C11 obj = factory.create<C11>();

    BOOST_CHECK_EQUAL(i, *obj.i);
}

BOOST_AUTO_TEST_CASE(namedSharedPtrIf)
{
    factory
    <
        vector
        <
            dependency<per_request, named<If0, string<'1'> >, CIf0, vector0<>, or_< is_base_of<_1, named<If0, string<'1'> > >, is_same<_1, named<If0, string<'1'> > > > >
        >
    >
    factory;

    C12 obj = factory.create<C12>();

    BOOST_CHECK(dynamic_cast<CIf0*>(obj.if0.get()));
    BOOST_CHECK_EQUAL(0, obj.c2->i);
    BOOST_CHECK_EQUAL(0.0, obj.c2->d);
    BOOST_CHECK_EQUAL(0, obj.c2->c);
}

BOOST_AUTO_TEST_CASE(namedSharedPtrIfwithNotTrivialctor)
{
    const int i = 42;

    factory
    <
        vector
        <
            dependency<per_request, named<If0>, CIf03, vector0<>, or_< is_base_of<_1, named<If0> >, is_same<_1, named<If0> > > >,
            dependency<per_request, int, int_<i>, vector0<>, or_< is_base_of<_1, int>, is_same<_1, int> > >
        >
    >
    factory;

    C13 obj = factory.create<C13>();

    CIf03* if0 = dynamic_cast<CIf03*>(obj.if0.get());
    BOOST_CHECK(if0);
    BOOST_CHECK_EQUAL(i, obj.c3.i);
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

    C9 obj = factory.create<C9>();

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

    typedef pool< vector<instance<C2> > > pool;

    boost::scoped_ptr<pool> test_pool(
        new pool(
            instance<C2>(make_shared<C2>(i, d, c))
        )
    );

    factory
    <
        vector
        <
            dependency<per_request, C2>
        >,
        pool
    >
    factory(*test_pool);

    C2 obj = factory.create<C2>();

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

    C4 obj = factory.create<C4>();

    BOOST_CHECK_EQUAL(i1, obj.i1);
    BOOST_CHECK_EQUAL(i2, obj.i2);
}
#endif

} // namespace mt
} // namespace back
} // namespace di

