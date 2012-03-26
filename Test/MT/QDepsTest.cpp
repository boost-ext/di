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
#include "QDeps/Front/Base/Module.hpp"

namespace QDeps
{
namespace MT
{

using namespace boost::mpl;
using namespace Test::Common;
using namespace Utility;
using namespace Front::Base;

//TODO add providers to module
//TODO add external instances
//TODO add named
//TODO boost::function

struct BaseModule : Front::Base::Module
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

TEST(QDeps, Module)
{
    Utility::Injector<BaseModule> inj;

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

