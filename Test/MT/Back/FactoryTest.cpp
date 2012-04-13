//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/make_shared.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/or.hpp>
#include "Test/Common/Data.hpp"
#include "QDeps/Utility/Named.hpp"
#include "QDeps/Back/Aux/Dependency.hpp"
#include "QDeps/Back/Scopes/PerRequest.hpp"
#include "QDeps/Back/Scopes/Singleton.hpp"
#include "QDeps/Back/Aux/Pool.hpp"
#include "QDeps/Back/Factory.hpp"

namespace QDeps
{
namespace Back
{
namespace MT
{

using namespace Test::Common;
using namespace Utility;
using namespace Scopes;
using namespace Aux;
using namespace boost::mpl;
using namespace boost;

BOOST_AUTO_TEST_CASE(CreateUsingCopy)
{
    Factory< vector0<> > factory;
    C0 obj = factory.create<C0>();
    (void)(obj);
}

BOOST_AUTO_TEST_CASE(CreateUsingPtr)
{
    Factory< vector0<> > factory;
    C0* obj = factory.create<C0*>();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(CreateUsingConstPtr)
{
    Factory< vector0<> > factory;
    const C0* obj = factory.create<const C0*>();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(CreateUsingSharedPtr)
{
    Factory< vector0<> > factory;
    shared_ptr<C0> obj = factory.create< shared_ptr<C0> >();
    BOOST_CHECK(obj);
}

BOOST_AUTO_TEST_CASE(CreateDefaultCtor)
{
    Factory< vector0<> > factory;
    C0 obj = factory.create<C0>();
    (void)(obj);
}

BOOST_AUTO_TEST_CASE(CreatePerRequest)
{
    Factory
    <
        vector
        <
            Dependency<PerRequest, If0, CIf0>
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

BOOST_AUTO_TEST_CASE(CreatePerRequestSingleton)
{
    Factory
    <
        vector
        <
            Dependency<PerRequest, If0, CIf0>,
            Dependency<Singleton, C3>
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

BOOST_AUTO_TEST_CASE(CreatePerRequestSingletonPath)
{
    Factory
    <
        vector
        <
            Dependency<PerRequest, If0, CIf0>,
            Dependency<PerRequest, If0, CIf01, vector<C6, C5> >,
            Dependency<Singleton, C3>
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

BOOST_AUTO_TEST_CASE(CreatePerRequestSingletonPathOrder)
{
    Factory
    <
        vector
        <
            Dependency<PerRequest, If0, CIf0>,
            Dependency<PerRequest, If0, CIf01, vector<C6, C5> >,
            Dependency<PerRequest, If0, CIf02, vector<C7> >,
            Dependency<Singleton, C3>
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

BOOST_AUTO_TEST_CASE(CreatePerRequestSingletonPathMix)
{
    Factory
    <
        vector
        <
            Dependency<PerRequest, If0, CIf0>,
            Dependency<PerRequest, If0, CIf01, vector<C6, C5> >,
            Dependency<PerRequest, If0, CIf02, vector<C7> >,
            Dependency<Singleton, C3>,
            Dependency<PerRequest, int, int_<1> >,
            Dependency<PerRequest, int, int_<2>, vector<C8> >,
            Dependency<PerRequest, Named<int, string<'1'> >, int_<3>, vector<C7, C6, C4> >,
            Dependency<PerRequest, Named<int, string<'2'> >, int_<4>, vector<C7, C6, C4> >,
            Dependency<PerRequest, int, int_<5>, vector<C2> >
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

BOOST_AUTO_TEST_CASE(CreateSingletonImpl)
{
    Factory
    <
        vector
        <
            Dependency<Singleton, If0, CIf0>
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

BOOST_AUTO_TEST_CASE(CreateSingletonMany)
{
    Factory
    <
        vector
        <
            Dependency<Singleton, If0, CIf0>,
            Dependency<Singleton, C3>,
            Dependency<Singleton, C1>
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

BOOST_AUTO_TEST_CASE(CtorTraits)
{
    const int i = 42;

    Factory
    <
        vector
        <
            Dependency<PerRequest, int, int_<i>, vector0<>, or_< is_base_of<_1, int>, is_same<_1, int> > >
        >
    >
    factory;

    C14 obj = factory.create<C14>();

    BOOST_CHECK_EQUAL(i, obj.i);
    BOOST_CHECK_EQUAL(0.0, obj.d);
}

BOOST_AUTO_TEST_CASE(ClassCtorTraits)
{
    const int i1 = 42;
    const int i2 = 87;

    Factory
    <
        vector
        <
            Dependency<PerRequest, Named<int, string<'1'> >, int_<i1> >,
            Dependency<PerRequest, Named<int, string<'2'> >, int_<i2> >
        >
    >
    factory;

    C10 obj = factory.create<C10>();

    BOOST_CHECK_EQUAL(i1, obj.i1);
    BOOST_CHECK_EQUAL(i2, obj.i2);
}

BOOST_AUTO_TEST_CASE(BaseOf)
{
    Factory
    <
        vector
        <
            Dependency<PerRequest, int, int_<1>, vector0<>, or_< is_base_of<_1, int>, is_same<_1, int> > >,
            Dependency<PerRequest, Named<int, string<'2'> >, int_<4>, vector<C7, C6, C4>, or_< is_base_of<_1, Named<int, string<'2'> > >, is_same<_1, Named<int, string<'2'> > > > >,
            Dependency<PerRequest, int, int_<5>, vector<C2>, or_< is_base_of<_1, int>, is_same<_1, int> > >,
            Dependency<PerRequest, CIf0, CIf0, vector0<>, or_< is_base_of<_1, CIf0>, is_same<_1, CIf0> > >,
            Dependency<PerRequest, Named<int, string<'1'> >, int_<3>, vector<C7, C6, C4>, or_< is_base_of<_1, Named<int, string<'1'> > >, is_same<_1, Named<int, string<'1'> > > > >
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

BOOST_AUTO_TEST_CASE(BaseOfInterfaceNotTrivialCtor)
{
    Factory
    <
        vector
        <
            Dependency<PerRequest, TransactionProvider, TransactionProvider, vector0<>, or_< is_base_of<_1, TransactionProvider>, is_same<_1, TransactionProvider> > >
        >
    >
    factory;

    TransactionUsage obj = factory.create<TransactionUsage>();

    BOOST_CHECK(obj.p->get().get() != obj.p->get().get());
}

BOOST_AUTO_TEST_CASE(NamedSharedPtrBaseOf)
{
    const int i = 42;

    Factory
    <
        vector
        <
            Dependency<PerRequest, Named<int, string<'1'> >, int_<i>, vector0<> >
        >
    >
    factory;

    C11 obj = factory.create<C11>();

    BOOST_CHECK_EQUAL(i, *obj.i);
}

BOOST_AUTO_TEST_CASE(NamedSharedPtr)
{
    const int i = 42;

    Factory
    <
        vector
        <
            Dependency<PerRequest, Named<int, string<'1'> >, int_<i>, vector0<>, or_< is_base_of<_1, Named<int, string<'1'> > >, is_same<_1, Named<int, string<'1'> > > > >
        >
    >
    factory;

    C11 obj = factory.create<C11>();

    BOOST_CHECK_EQUAL(i, *obj.i);
}

BOOST_AUTO_TEST_CASE(NamedSharedPtrIf)
{
    Factory
    <
        vector
        <
            Dependency<PerRequest, Named<If0, string<'1'> >, CIf0, vector0<>, or_< is_base_of<_1, Named<If0, string<'1'> > >, is_same<_1, Named<If0, string<'1'> > > > >
        >
    >
    factory;

    C12 obj = factory.create<C12>();

    BOOST_CHECK(dynamic_cast<CIf0*>(obj.if0.get()));
    BOOST_CHECK_EQUAL(0, obj.c2->i);
    BOOST_CHECK_EQUAL(0.0, obj.c2->d);
    BOOST_CHECK_EQUAL(0, obj.c2->c);
}

#if 0
BOOST_AUTO_TEST_CASE(NamedSharedPtrConcreteTypeWithNotTrivialCtor)
{
}
#endif

#if 0
BOOST_AUTO_TEST_CASE(CreateWithValues)
{
    const int i = 42;
    const double d = 21.0;
    const char c = 'x';

    typedef QPool::Pool< vector<double, char> > Pool;

    Pool pool
    (
        make_shared<double>(d),
        make_shared<char>(c)
    );

    Factory
    <
        vector
        <
            Dependency<PerRequest, int, int_<i> >,
            Dependency<PerRequest, double>,
            Dependency<PerRequest, char>,
            Dependency<PerRequest, std::string, string<'test'> >
        >,
        Pool
    >
    factory(pool);

    C9 obj = factory.create<C9>();

    BOOST_CHECK_EQUAL(i, obj.i);
    BOOST_CHECK_EQUAL(d, obj.d);
    BOOST_CHECK_EQUAL(c, obj.c);
    BOOST_CHECK_EQUAL("test", obj.s);
}

BOOST_AUTO_TEST_CASE(CreateWithNonTrivialCtor)
{
    const int i = 42;
    const double d = 21.0;
    const char c = 'x';

    typedef QPool::Pool< vector<C2> > Pool;

    Pool pool
    (
        make_shared<C2>(i, d, c)
    );

    Factory
    <
        vector
        <
            Dependency<PerRequest, C2>
        >,
        Pool
    >
    factory(pool);

    C2 obj = factory.create<C2>();

    BOOST_CHECK_EQUAL(i, obj.i);
    BOOST_CHECK_EQUAL(d, obj.d);
    BOOST_CHECK_EQUAL(c, obj.c);
}

BOOST_AUTO_TEST_CASE(CreateWithAttributes)
{
    const int i1 = 42;
    const int i2 = 87;

    typedef QPool::Pool< vector<Named<int, string<'1'> >, Named<int, string<'2'> > > > Pool;

    Pool pool
    (
        make_shared<Named<int, string<'1'> > >(i1),
        make_shared<Named<int, string<'2'> > >(i2)
    );

    Factory
    <
        vector
        <
            Dependency<PerRequest, Named<int, string<'1'> >, int>,
            Dependency<PerRequest, Named<int, string<'2'> >, int>
        >,
        Pool
    >
    factory(pool);

    C4 obj = factory.create<C4>();

    BOOST_CHECK_EQUAL(i1, obj.i1);
    BOOST_CHECK_EQUAL(i2, obj.i2);
}
#endif

} // namespace MT
} // namespace Back
} // namespace QDeps

