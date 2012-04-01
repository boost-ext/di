//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "Test/Common/Utility.hpp"
#include "Test/Common/Data.hpp"
#include "QDeps/QDeps.hpp"

namespace QDeps
{
namespace MT
{

using namespace boost;
using namespace mpl;
using namespace Test::Common;
using namespace Utility;
using namespace Front::Base;
using namespace Back;

struct BaseModule1 : Front::Base::Module
    <
        Singletons <
            C3
        >,
        PerRequests <
            CIf0,
            Bind<CIf01>::InCall<C6, C5>,
            Bind<CIf02>::InCall<C7>,
            Bind<int, int_<1> >,
            Bind<int, int_<2> >::InCall<C8>,
            Bind<int, int_<3> >::InName< string<'1'> >::InCall<C7, C6, C4>,
            Bind<int, int_<4> >::InName< string<'2'> >::InCall<C7, C6, C4>,
            Bind<int, int_<5> >::InCall<C2>
        >
    >
{ };

struct BaseModule2 : Front::Base::Module
    <
        Singletons <
            C3
        >,
        PerRequests <
            Bind<int, int_<0> >::InName< string<'1'> >,
            Bind<int, int_<1> >
        >
    >
{ };

struct BaseModule3 : Front::Base::Module
    <
        Singletons <
            CIf0
        >,
        PerRequests <
            Bind<int, int_<2> >::InCall<C8>,
            Bind<int, int_<3> >::InName< string<'2'> >
        >
    >
{ };

BOOST_AUTO(fusionModule1, Front::Fusion::Module<>()(
    Singletons <
        C3
    >(),
    PerRequests <
        CIf0,
        Bind<CIf01>::InCall<C6, C5>,
        Bind<CIf02>::InCall<C7>,
        Bind<int, int_<1> >,
        Bind<int, int_<2> >::InCall<C8>,
        Bind<int, int_<3> >::InName< string<'1'> >::InCall<C7, C6, C4>,
        Bind<int, int_<4> >::InName< string<'2'> >::InCall<C7, C6, C4>,
        Bind<int, int_<5> >::InCall<C2>
    >()
));

BOOST_AUTO(fusionModule2, Front::Fusion::Module<>()(
    Singletons <
        CIf0
    >(),
    PerRequests <
        Bind<int, int_<2> >::InCall<C8>,
        Bind<int, int_<3> >::InName< string<'2'> >
    >()
));

BOOST_AUTO(fusionModule3, Front::Fusion::Module<>()(
    Singletons <
        C3
    >(),
    PerRequests <
        Bind<int, int_<0> >::InName< string<'1'> >,
        Bind<int, int_<1> >
    >()
));

TEST_T(QDeps, OneModule,
    Injector<BaseModule1>,
    Injector<BOOST_TYPEOF(fusionModule1)>)
{
    shared_ptr<C8> c8 = this->injector.template create< shared_ptr<C8> >();

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

TEST_T(QDeps, ManyModules,
    Injector<BaseModule2, BaseModule3>,
    Injector<BaseModule3, BaseModule2>,
    Injector<BOOST_TYPEOF(fusionModule2), BOOST_TYPEOF(fusionModule3)>,
    Injector<BOOST_TYPEOF(fusionModule3), BOOST_TYPEOF(fusionModule2)>)
{
    shared_ptr<C8> c8 = this->injector.template create< shared_ptr<C8> >();

    EXPECT_NE(c8->c1, c8->c7->c6->c5.c1);
    EXPECT_EQ(c8->c7->c6->c4->c3, c8->c7->c6->c3);
    EXPECT_EQ(c8->c7->if0, c8->c7->c6->c5.if0);

    EXPECT_TRUE(dynamic_cast<CIf0*>(c8->c7->c6->c5.if0.get()));
    EXPECT_TRUE(dynamic_cast<CIf0*>(c8->c7->if0.get()));

    EXPECT_EQ(2, c8->i);
    EXPECT_EQ(0, c8->c7->c6->c4->i1);
    EXPECT_EQ(3, c8->c7->c6->c4->i2);
    EXPECT_EQ(1, c8->c7->c6->c3->i);
    EXPECT_EQ(1, c8->c7->c6->c5.c2->i);
    EXPECT_EQ(0.0, c8->c7->c6->c5.c2->d);
    EXPECT_EQ(0, c8->c7->c6->c5.c2->c);
}

TEST_T(QDeps, MixModules,
    Injector<BaseModule2, BOOST_TYPEOF(fusionModule2)>,
    Injector<BOOST_TYPEOF(fusionModule2), BaseModule2>)
{
    shared_ptr<C8> c8 = this->injector.template create< shared_ptr<C8> >();

    EXPECT_NE(c8->c1, c8->c7->c6->c5.c1);
    EXPECT_EQ(c8->c7->c6->c4->c3, c8->c7->c6->c3);
    EXPECT_EQ(c8->c7->if0, c8->c7->c6->c5.if0);

    EXPECT_TRUE(dynamic_cast<CIf0*>(c8->c7->c6->c5.if0.get()));
    EXPECT_TRUE(dynamic_cast<CIf0*>(c8->c7->if0.get()));

    EXPECT_EQ(2, c8->i);
    EXPECT_EQ(0, c8->c7->c6->c4->i1);
    EXPECT_EQ(3, c8->c7->c6->c4->i2);
    EXPECT_EQ(1, c8->c7->c6->c3->i);
    EXPECT_EQ(1, c8->c7->c6->c5.c2->i);
    EXPECT_EQ(0.0, c8->c7->c6->c5.c2->d);
    EXPECT_EQ(0, c8->c7->c6->c5.c2->c);
}

//TODO define BOOST_ASSERT<false> to make policy test possible
TEST(QDeps, ModulesPolicies)
{
    Injector< Policy<Policies::DisallowCircularDependencies> > injector;
    //injector.create<CD1>();
}

} // namespace MT
} // namespace QDeps

