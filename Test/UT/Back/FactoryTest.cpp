//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSEstring<'1'> _0.txt or copy at http://www.boost.org/LICENSEstring<'1'> _0.txt)
//
#include <gtest/gtest.h>
#include <boost/make_shared.hpp>
#include <boost/mpl/vector.hpp>
#include "Test/Common/Ctors.hpp"
#include "QDeps/Back/Factory.hpp"
#include "QDeps/Utility/Attr.hpp"
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
using namespace Aux;
using namespace Utility;
using namespace Back::Scopes;
using namespace boost::mpl;
using namespace boost;

TEST(Factory, CreateUsingCopy)
{
    Factory< vector0<> > l_factory;
    C0 l_obj = l_factory.create<C0>();
    (void)(l_obj);
}

TEST(Factory, CreateUsingRef)
{
    Factory< vector0<> > l_factory;
    C0& l_obj = l_factory.create<C0&>();
    EXPECT_TRUE(&l_obj);
}

TEST(Factory, CreateUsingConstRef)
{
    Factory< vector0<> > l_factory;
    const C0& l_obj = l_factory.create<const C0&>();
    EXPECT_TRUE(&l_obj);
}

TEST(Factory, CreateUsingPtr)
{
    Factory< vector0<> > l_factory;
    C0* l_obj = l_factory.create<C0*>();
    EXPECT_TRUE(l_obj);
}

TEST(Factory, CreateUsingConstPtr)
{
    Factory< vector0<> > l_factory;
    const C0* l_obj = l_factory.create<const C0*>();
    EXPECT_TRUE(l_obj);
}

TEST(Factory, CreateUsingSharedPtr)
{
    Factory< vector0<> > l_factory;
    shared_ptr<C0> l_obj = l_factory.create< shared_ptr<C0> >();
    EXPECT_TRUE(l_obj);
}

TEST(Factory, CreateDefaultCtor)
{
    Factory< vector0<> > l_factory;
    C0 l_obj = l_factory.create<C0>();
    (void)(l_obj);
}

TEST(Factory, CreateWithValues)
{
    const int i = 42;
    const double d = 21.0;
    const char c = 'x';

    typedef QPool::Pool< vector<double, char> > Pool;

    Pool l_pool
    (
        make_shared<double>(d),
        make_shared<char>(c)
    );

    Factory
    <
        vector
        <
            Inst<PerRequest, int, int_<i> >,
            Inst<PerRequest, double>,
            Inst<PerRequest, char>,
            Inst<PerRequest, std::string, string<'test'> >
        >,
        Pool
    >
    l_factory(l_pool);

    C9 l_obj = l_factory.create<C9>();

    EXPECT_EQ(i, l_obj.i);
    EXPECT_EQ(d, l_obj.d);
    EXPECT_EQ(c, l_obj.c);
    EXPECT_EQ("test", l_obj.s);
}

TEST(Factory, CreateWithAttributes)
{
    const int i1 = 42;
    const int i2 = 87;

    typedef QPool::Pool< vector<Attr<int, string<'1'> >, Attr<int, string<'2'> > > > Pool;

    Pool l_pool
    (
        make_shared<Attr<int, string<'1'> > >(i1),
        make_shared<Attr<int, string<'2'> > >(i2)
    );

    Factory
    <
        vector
        <
            Inst<PerRequest, Attr<int, string<'1'> > >,
            Inst<PerRequest, Attr<int, string<'2'> > >
        >,
        Pool
    >
    l_factory(l_pool);

    C4 l_obj = l_factory.create<C4>();

    EXPECT_EQ(i1, l_obj.i1);
    EXPECT_EQ(i2, l_obj.i2);
}

TEST(Factory, CreatePerRequest)
{
    Factory
    <
        vector
        <
            Impl<PerRequest, If0, CIf0>
        >
    >
    l_factory;

    boost::shared_ptr<C8> c8 = l_factory.create< boost::shared_ptr<C8> >();

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
            Impl<PerRequest, If0, CIf0>,
            Inst<Singleton, C3>
        >
    >
    l_factory;

    boost::shared_ptr<C8> c8 = l_factory.create< boost::shared_ptr<C8> >();

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
            Impl<PerRequest, If0, CIf0>,
            Impl<PerRequest, If0, CIf01, vector<C6, C5> >,
            Inst<Singleton, C3>
        >
    >
    l_factory;

    boost::shared_ptr<C8> c8 = l_factory.create< boost::shared_ptr<C8> >();

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
            Impl<PerRequest, If0, CIf0>,
            Impl<PerRequest, If0, CIf01, vector<C6, C5> >,
            Impl<PerRequest, If0, CIf02, vector<C7> >,
            Inst<Singleton, C3>
        >
    >
    l_factory;

    boost::shared_ptr<C8> c8 = l_factory.create< boost::shared_ptr<C8> >();

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
            Impl<PerRequest, If0, CIf0>,
            Impl<PerRequest, If0, CIf01, vector<C6, C5> >,
            Impl<PerRequest, If0, CIf02, vector<C7> >,
            Inst<Singleton, C3>,
            Inst<PerRequest, int, int_<1> >,
            Inst<PerRequest, int, int_<2>, vector<C8> >,
            Inst<PerRequest, Attr<int, mpl::string<'1'> >, int_<3>, vector<C7, C6, C4> >,
            Inst<PerRequest, Attr<int, mpl::string<'2'> >, int_<4>, vector<C7, C6, C4> >,
            Inst<PerRequest, int, int_<5>, vector<C2> >
        >
    >
    l_factory;

    boost::shared_ptr<C8> c8 = l_factory.create< boost::shared_ptr<C8> >();

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
            Impl<Singleton, If0, CIf0>
        >
    >
    l_factory;

    boost::shared_ptr<C8> c8 = l_factory.create< boost::shared_ptr<C8> >();

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
            Impl<Singleton, If0, CIf0>,
            Inst<Singleton, C3>,
            Inst<Singleton, C1>
        >
    >
    l_factory;

    boost::shared_ptr<C8> c8 = l_factory.create< boost::shared_ptr<C8> >();

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

} // namespace UT
} // namespace Back
} // namespace QDeps

