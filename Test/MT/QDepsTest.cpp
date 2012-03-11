//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include "Test/Common/Data.hpp"
#include "QDeps/Utility/Injector.hpp"
#include "QDeps/Utility/Named.hpp"
#include "QDeps/Front/Generic/Module.hpp"

namespace QDeps
{
namespace MT
{

using namespace boost::mpl;
using namespace Utility;
using namespace Test::Common;

//TODO add providers to module
//TODO add external instances
//TODO add named
//TODO boost::function
//TODO is_base_of
//TODO QDEPS_INJECT for method as well

struct GenericModule : Front::Generic::Module
{
    typedef vector
    <
        Scope<Singleton>::Bind
        <
            Instance<C3>
        >,
        Implementation<If0, CIf0>,
        Implementation<If0, CIf01>::Bind< InCall<C6, C5> >,
        Implementation<If0, CIf02>::Bind<C7>,
        Instance<int, int_<1> >,
        Instance<int, int_<2> >::Bind<C8>,
        Instance<Named<int, string<'1'> >, int_<3> >::Bind< InCall<C7, C6, C4> >,
        Instance<Named<int, string<'2'> >, int_<4> >::Bind< InCall<C7, C6, C4> >,
        Instance<int, int_<5> >::Bind<C2>
    >
    Binding;
};

struct GenericModule2 : Front::Generic::Module
{
    typedef vector
    <
    >
    Binding;
};

TEST(QDeps, Module)
{
    Utility::Injector<GenericModule> inj;

    boost::shared_ptr<C8> c8 = inj.create< boost::shared_ptr<C8> >();

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

TEST(QDeps, Modules)
{
}

//TODO define BOOST_ASSERT<false> to make policy test possible
TEST(QDeps, ModulesPolicies)
{
}

} // namespace MT
} // namespace QDeps

