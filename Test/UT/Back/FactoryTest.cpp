//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <boost/make_shared.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/or.hpp>
#include "Test/Common/Data.hpp"
#include "QDeps/Back/Factory.hpp"
#include "QDeps/Utility/Named.hpp"
#include "QDeps/Back/Scopes/PerRequest.hpp"
#include "QDeps/Back/Scopes/Singleton.hpp"
#include "QPool/Pool.hpp"

namespace QDeps
{
namespace Back
{
namespace UT
{

using namespace Test::Common;
using namespace Utility;
using namespace Back::Scope;
using namespace Back::Aux;
using namespace boost::mpl;
using namespace boost;

TEST(Factory, CreateUsingCopy)
{
    Factory< vector0<> > factory;
    C0 obj = factory.create<C0>();
    (void)(obj);
}

TEST(Factory, CreateUsingRef)
{
    Factory< vector0<> > factory;
    C0& obj = factory.create<C0&>();
    EXPECT_TRUE(&obj);
}

TEST(Factory, CreateUsingConstRef)
{
    Factory< vector0<> > factory;
    const C0& obj = factory.create<const C0&>();
    EXPECT_TRUE(&obj);
}

TEST(Factory, CreateUsingPtr)
{
    Factory< vector0<> > factory;
    C0* obj = factory.create<C0*>();
    EXPECT_TRUE(obj);
}

TEST(Factory, CreateUsingConstPtr)
{
    Factory< vector0<> > factory;
    const C0* obj = factory.create<const C0*>();
    EXPECT_TRUE(obj);
}

TEST(Factory, CreateUsingSharedPtr)
{
    Factory< vector0<> > factory;
    shared_ptr<C0> obj = factory.create< shared_ptr<C0> >();
    EXPECT_TRUE(obj);
}

TEST(Factory, CreateDefaultCtor)
{
    Factory< vector0<> > factory;
    C0 obj = factory.create<C0>();
    (void)(obj);
}

TEST(Factory, CreateWithValues)
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

    EXPECT_EQ(i, obj.i);
    EXPECT_EQ(d, obj.d);
    EXPECT_EQ(c, obj.c);
    EXPECT_EQ("test", obj.s);
}

TEST(Factory, CreateWithNonTrivialCtor)
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

    EXPECT_EQ(i, obj.i);
    EXPECT_EQ(d, obj.d);
    EXPECT_EQ(c, obj.c);
}

TEST(Factory, CreateWithAttributes)
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
            Dependency<PerRequest, Named<int, string<'1'> > >,
            Dependency<PerRequest, Named<int, string<'2'> > >
        >,
        Pool
    >
    factory(pool);

    C4 obj = factory.create<C4>();

    EXPECT_EQ(i1, obj.i1);
    EXPECT_EQ(i2, obj.i2);
}

TEST(Factory, CreatePerRequest)
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

    EXPECT_NE(c8->c1, c8->c7->c6->c5.c1);
    EXPECT_NE(c8->c7->c6->c4->c3, c8->c7->c6->c3);
    EXPECT_NE(c8->c7->if0, c8->c7->c6->c5.if0);

    EXPECT_TRUE(dynamic_cast<CIf0*>(c8->c7->c6->c5.if0.get()));
    EXPECT_TRUE(dynamic_cast<CIf0*>(c8->c7->if0.get()));

    EXPECT_EQ(0, c8->i);
    EXPECT_EQ(0, c8->c7->c6->c4->i1);
    EXPECT_EQ(0, c8->c7->c6->c4->i2);
    EXPECT_EQ(0, c8->c7->c6->c3->i);
    EXPECT_EQ(0, c8->c7->c6->c5.c2->i);
    EXPECT_EQ(0.0, c8->c7->c6->c5.c2->d);
    EXPECT_EQ(0, c8->c7->c6->c5.c2->c);
}

TEST(Factory, CreatePerRequestSingleton)
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

    EXPECT_NE(c8->c1, c8->c7->c6->c5.c1);
    EXPECT_EQ(c8->c7->c6->c4->c3, c8->c7->c6->c3);
    EXPECT_NE(c8->c7->if0, c8->c7->c6->c5.if0);

    EXPECT_TRUE(dynamic_cast<CIf0*>(c8->c7->c6->c5.if0.get()));
    EXPECT_TRUE(dynamic_cast<CIf0*>(c8->c7->if0.get()));

    EXPECT_EQ(0, c8->i);
    EXPECT_EQ(0, c8->c7->c6->c4->i1);
    EXPECT_EQ(0, c8->c7->c6->c4->i2);
    EXPECT_EQ(0, c8->c7->c6->c3->i);
    EXPECT_EQ(0, c8->c7->c6->c5.c2->i);
    EXPECT_EQ(0.0, c8->c7->c6->c5.c2->d);
    EXPECT_EQ(0, c8->c7->c6->c5.c2->c);
}

TEST(Factory, CreatePerRequestSingletonPath)
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

    EXPECT_NE(c8->c1, c8->c7->c6->c5.c1);
    EXPECT_EQ(c8->c7->c6->c4->c3, c8->c7->c6->c3);
    EXPECT_NE(c8->c7->if0, c8->c7->c6->c5.if0);

    EXPECT_TRUE(dynamic_cast<CIf01*>(c8->c7->c6->c5.if0.get()));
    EXPECT_TRUE(dynamic_cast<CIf0*>(c8->c7->if0.get()));

    EXPECT_EQ(0, c8->i);
    EXPECT_EQ(0, c8->c7->c6->c4->i1);
    EXPECT_EQ(0, c8->c7->c6->c4->i2);
    EXPECT_EQ(0, c8->c7->c6->c3->i);
    EXPECT_EQ(0, c8->c7->c6->c5.c2->i);
    EXPECT_EQ(0.0, c8->c7->c6->c5.c2->d);
    EXPECT_EQ(0, c8->c7->c6->c5.c2->c);
}

TEST(Factory, CreatePerRequestSingletonPathOrder)
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

    EXPECT_NE(c8->c1, c8->c7->c6->c5.c1);
    EXPECT_EQ(c8->c7->c6->c4->c3, c8->c7->c6->c3);
    EXPECT_NE(c8->c7->if0, c8->c7->c6->c5.if0);

    EXPECT_TRUE(dynamic_cast<CIf01*>(c8->c7->c6->c5.if0.get()));
    EXPECT_TRUE(dynamic_cast<CIf02*>(c8->c7->if0.get()));

    EXPECT_EQ(0, c8->i);
    EXPECT_EQ(0, c8->c7->c6->c4->i1);
    EXPECT_EQ(0, c8->c7->c6->c4->i2);
    EXPECT_EQ(0, c8->c7->c6->c3->i);
    EXPECT_EQ(0, c8->c7->c6->c5.c2->i);
    EXPECT_EQ(0.0, c8->c7->c6->c5.c2->d);
    EXPECT_EQ(0, c8->c7->c6->c5.c2->c);
}

TEST(Factory, CreatePerRequestSingletonPathMix)
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

    EXPECT_NE(c8->c1, c8->c7->c6->c5.c1);
    EXPECT_EQ(c8->c7->c6->c4->c3, c8->c7->c6->c3);
    EXPECT_NE(c8->c7->if0, c8->c7->c6->c5.if0);

    EXPECT_TRUE(dynamic_cast<CIf01*>(c8->c7->c6->c5.if0.get()));
    EXPECT_TRUE(dynamic_cast<CIf02*>(c8->c7->if0.get()));

    EXPECT_EQ(2, c8->i);
    EXPECT_EQ(3, c8->c7->c6->c4->i1);
    EXPECT_EQ(4, c8->c7->c6->c4->i2);
    EXPECT_EQ(1, c8->c7->c6->c3->i);
    EXPECT_EQ(5, c8->c7->c6->c5.c2->i);
    EXPECT_EQ(0.0, c8->c7->c6->c5.c2->d);
    EXPECT_EQ(0, c8->c7->c6->c5.c2->c);
}

TEST(Factory, CreateSingletonImpl)
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

    EXPECT_NE(c8->c1, c8->c7->c6->c5.c1);
    EXPECT_NE(c8->c7->c6->c4->c3, c8->c7->c6->c3);
    EXPECT_EQ(c8->c7->if0, c8->c7->c6->c5.if0);

    EXPECT_TRUE(dynamic_cast<CIf0*>(c8->c7->c6->c5.if0.get()));
    EXPECT_TRUE(dynamic_cast<CIf0*>(c8->c7->if0.get()));

    EXPECT_EQ(0, c8->i);
    EXPECT_EQ(0, c8->c7->c6->c4->i1);
    EXPECT_EQ(0, c8->c7->c6->c4->i2);
    EXPECT_EQ(0, c8->c7->c6->c3->i);
    EXPECT_EQ(0, c8->c7->c6->c5.c2->i);
    EXPECT_EQ(0.0, c8->c7->c6->c5.c2->d);
    EXPECT_EQ(0, c8->c7->c6->c5.c2->c);
}

TEST(Factory, CreateSingletonMany)
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

    EXPECT_EQ(c8->c1, c8->c7->c6->c5.c1);
    EXPECT_EQ(c8->c7->c6->c4->c3, c8->c7->c6->c3);
    EXPECT_EQ(c8->c7->if0, c8->c7->c6->c5.if0);

    EXPECT_TRUE(dynamic_cast<CIf0*>(c8->c7->c6->c5.if0.get()));
    EXPECT_TRUE(dynamic_cast<CIf0*>(c8->c7->if0.get()));

    EXPECT_EQ(0, c8->i);
    EXPECT_EQ(0, c8->c7->c6->c4->i1);
    EXPECT_EQ(0, c8->c7->c6->c4->i2);
    EXPECT_EQ(0, c8->c7->c6->c3->i);
    EXPECT_EQ(0, c8->c7->c6->c5.c2->i);
    EXPECT_EQ(0.0, c8->c7->c6->c5.c2->d);
    EXPECT_EQ(0, c8->c7->c6->c5.c2->c);
}

TEST(Factory, BaseOf)
{
    Factory
    <
        vector
        <
            Dependency<Back::Scope::PerRequest, int, int_<1>, vector0<>, or_< is_base_of<_1, int>, is_same<_1, int> > >,
            Dependency<Back::Scope::PerRequest, Named<int, string<'2'> >, int_<4>, vector<C7, C6, C4>, or_< is_base_of<_1, Named<int, string<'2'> > >, is_same<_1, Named<int, string<'2'> > > > >,
            Dependency<Back::Scope::PerRequest, int, int_<5>, vector<C2>, or_< is_base_of<_1, int>, is_same<_1, int> > >,
            Dependency<PerRequest, CIf0, CIf0, vector0<>, or_< is_base_of<_1, CIf0>, is_same<_1, CIf0> > >,
            Dependency<PerRequest, Named<int, string<'1'> >, int_<3>, vector<C7, C6, C4>, or_< is_base_of<_1, Named<int, string<'1'> > >, is_same<_1, Named<int, string<'1'> > > > >
        >
    >
    factory;

    shared_ptr<C8> c8 = factory.create< shared_ptr<C8> >();

    EXPECT_NE(c8->c1, c8->c7->c6->c5.c1);
    EXPECT_NE(c8->c7->c6->c4->c3, c8->c7->c6->c3);
    EXPECT_NE(c8->c7->if0, c8->c7->c6->c5.if0);

    EXPECT_TRUE(dynamic_cast<CIf0*>(c8->c7->c6->c5.if0.get()));
    EXPECT_TRUE(dynamic_cast<CIf0*>(c8->c7->if0.get()));

    EXPECT_EQ(1, c8->i);
    EXPECT_EQ(3, c8->c7->c6->c4->i1);
    EXPECT_EQ(4, c8->c7->c6->c4->i2);
    EXPECT_EQ(1, c8->c7->c6->c3->i);
    EXPECT_EQ(5, c8->c7->c6->c5.c2->i);
    EXPECT_EQ(0.0, c8->c7->c6->c5.c2->d);
    EXPECT_EQ(0, c8->c7->c6->c5.c2->c);
}

TEST(Factory, CtorTraits)
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

    EXPECT_EQ(i1, obj.i1);
    EXPECT_EQ(i2, obj.i2);
}

TEST(Factory, NamedIfBaseOf)
{
    Factory
    <
        vector
        <
            Dependency<PerRequest, Named<If0, string<'1'> >, If0, vector0<>, or_< is_base_of<_1, Named<If0, string<'1'> > >, is_same<_1, Named<If0, string<'1'> > > > >
            //Dependency<PerRequest, Named<int, string<'1'> >, If0, vector0<>, or_< is_base_of<_1, Named<If0, string<'1'> > >, is_same<_1, Named<If0, string<'1'> > > > >
        >
    >
    factory;

    //C11 obj = factory.create<C11>();

    //EXPECT_EQ(i1, obj.i1);
    //EXPECT_EQ(i2, obj.i2);
}

} // namespace UT
} // namespace Back
} // namespace QDeps

