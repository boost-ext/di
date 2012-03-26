//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "Test/Common/Data.hpp"
#include "QDeps/Back/Dependency.hpp"
#include "QDeps/Front/Base/Module.hpp"
#include "QDeps/Utility/Named.hpp"

namespace QDeps
{
namespace Front
{
namespace Base
{
namespace UT
{

using namespace Test::Common;
using namespace Back;
using namespace Utility;
using namespace boost::mpl;

TEST(Module, Empty)
{
    struct MyModule : Module<> { };

    EXPECT_TRUE((
        equal
        <
            vector0<>,
            MyModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, MyModule::Externals>::value));
}

TEST(Module, DefaultScope)
{
    struct MyModule : Module<C1> { };

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<Back::Scope::PerRequest, C1, C1, vector0<>, boost::is_base_of<_1, C1> >
            >,
            MyModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, MyModule::Externals>::value));
}

TEST(Module, DefaultScopeMany)
{
    struct MyModule : Module<C1, C2, C3> { };

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<Back::Scope::PerRequest, C1, C1, vector0<>, boost::is_base_of<_1, C1> >,
                Dependency<Back::Scope::PerRequest, C2, C2, vector0<>, boost::is_base_of<_1, C2> >,
                Dependency<Back::Scope::PerRequest, C3, C3, vector0<>, boost::is_base_of<_1, C3> >
            >,
            MyModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, MyModule::Externals>::value));
}

TEST(Module, OneScope)
{
    struct MyModule : Module
        <
            Scope<Back::Scope::Singleton>::Bind <
                CIf0
            >
        >
    { };

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<Back::Scope::Singleton, CIf0, CIf0, vector0<>, boost::is_base_of<_1, CIf0> >
            >,
            MyModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, MyModule::Externals>::value));
}

TEST(Module, OneScopeAlias)
{
    struct MyModule : Module
        <
            Singletons <
                CIf0
            >
        >
    { };

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<Back::Scope::Singleton, CIf0, CIf0, vector0<>, boost::is_base_of<_1, CIf0> >
            >,
            MyModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, MyModule::Externals>::value));
}

TEST(Module, OneScopeDirect)
{
    struct MyModule : Module
        <
            Singleton<CIf0>
        >
    { };

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<Back::Scope::Singleton, CIf0, CIf0, vector0<>, boost::is_base_of<_1, CIf0> >
            >,
            MyModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, MyModule::Externals>::value));
}

TEST(Module, Many)
{
    struct MyModule : Module
        <
            Singletons<
                C1,
                C2,
                C3
            >
        >
    { };

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<Back::Scope::Singleton, C1, C1, vector0<>, boost::is_base_of<_1, C1> >,
                Dependency<Back::Scope::Singleton, C2, C2, vector0<>, boost::is_base_of<_1, C2> >,
                Dependency<Back::Scope::Singleton, C3, C3, vector0<>, boost::is_base_of<_1, C3> >
            >,
            MyModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, MyModule::Externals>::value));
}

TEST(Module, ManyScopes)
{
    struct MyModule : Module
        <
            Singletons<
                C1,
                C2
            >,
            PerRequests<
                C3,
                C4
            >
        >
    { };

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<Back::Scope::Singleton, C1, C1, vector0<>, boost::is_base_of<_1, C1> >,
                Dependency<Back::Scope::Singleton, C2, C2, vector0<>, boost::is_base_of<_1, C2> >,
                Dependency<Back::Scope::PerRequest, C3, C3, vector0<>, boost::is_base_of<_1, C3> >,
                Dependency<Back::Scope::PerRequest, C4, C4, vector0<>, boost::is_base_of<_1, C4> >
            >,
            MyModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, MyModule::Externals>::value));
}

TEST(Module, InCall)
{
    struct MyModule : Module
        <
            PerRequest<C1>::InCall<C2>
        >
    { };

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<Back::Scope::PerRequest, C1, C1, vector<C2>, boost::is_base_of<_1, C1> >
            >,
            MyModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, MyModule::Externals>::value));
}

TEST(Module, InName)
{
    struct MyModule : Module
        <
            Singleton<C1>::InName<int>
        >
    { };

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<Back::Scope::Singleton, Named<C1, int>, C1, vector0<>, boost::is_base_of<_1, Named<C1, int> > >
            >,
            MyModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, MyModule::Externals>::value));
}

TEST(Module, InNameInCall)
{
    struct MyModule : Module
        <
            Singletons<
                Bind<C1>::InName<int>::InCall<double>,
                Bind<C2>::InName<double>::InCall<int>
            >
        >
    { };

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<Back::Scope::Singleton, Named<C1, int>, C1, vector<double>, boost::is_base_of<_1, Named<C1, int> > >,
                Dependency<Back::Scope::Singleton, Named<C2, double>, C2, vector<int>, boost::is_base_of<_1,  Named<C2, double> > >
            >,
            MyModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, MyModule::Externals>::value));
}

TEST(Module, InCallInName)
{
    struct MyModule : Module
        <
            Singletons<
                Bind<C1>::InCall<double>::InName<int>,
                Bind<C2>::InCall<int>::InName<double>
            >
        >
    { };

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<Back::Scope::Singleton, Named<C1, int>, C1, vector<double>, boost::is_base_of<_1, Named<C1, int> > >,
                Dependency<Back::Scope::Singleton, Named<C2, double>, C2, vector<int>, boost::is_base_of<_1,  Named<C2, double> > >
            >,
            MyModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, MyModule::Externals>::value));
}

TEST(Module, BindIf)
{
    struct MyModule : Module
        <
            Singletons<
                Bind<If0, CIf0>
            >
        >
    { };

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<Back::Scope::Singleton, If0, CIf0, vector0<>, boost::is_same<_1, If0> >
            >,
            MyModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, MyModule::Externals>::value));
}

TEST(Module, Mix)
{
    struct MyModule : Module
        <
            Singletons<
               Bind<If0, CIf0>,
                C1,
                Bind<C2>::InName<int>,
                Bind<C3>::InCall<C4, C5>
            >,
            PerRequests <
                C6
            >,
            Singleton<C7>::InName<double>::InCall<C1>
        >
    { };

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<Back::Scope::Singleton, If0, CIf0, vector0<>, boost::is_same<_1, If0> >,
                Dependency<Back::Scope::Singleton, C1, C1, vector0<>, boost::is_base_of<_1, C1> >,
                Dependency<Back::Scope::Singleton, Named<C2, int>, C2, vector0<>, boost::is_base_of<_1, Named<C2, int> > >,
                Dependency<Back::Scope::Singleton, C3, C3, vector<C4, C5>, boost::is_base_of<_1, C3> >,
                Dependency<Back::Scope::PerRequest, C6, C6, vector0<>, boost::is_base_of<_1, C6> >,
                Dependency<Back::Scope::Singleton, Named<C7, double>, C7, vector<C1>, boost::is_base_of<_1, Named<C7, double> > >
            >,
            MyModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, MyModule::Externals>::value));
}

TEST(Module, Externals)
{
    struct MyModule : Module
        <
            Externals<
                int
            >
        >
    { };

    EXPECT_TRUE((equal<vector0<>, MyModule::Dependencies>::value));

    EXPECT_TRUE((
        equal
        <
            vector
            <
                int
            >,
            MyModule::Externals
        >::value
    ));
}

TEST(Module, ExternalsMix)
{
    struct MyModule : Module
        <
            Externals<
                int,
                double
            >,
            External<float>
        >
    { };

    EXPECT_TRUE((equal<vector0<>, MyModule::Dependencies>::value));

    EXPECT_TRUE((
        equal
        <
            vector
            <
                int, double, float
            >,
            MyModule::Externals
        >::value
    ));
}

TEST(Module, ExternalsBind)
{
    struct MyModule : Module
        <
            Externals<
                Bind<C1>::InName<int>
            >
        >
    { };

    EXPECT_TRUE((equal<vector0<>, MyModule::Dependencies>::value));

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Bind<C1>::InName<int>
            >,
            MyModule::Externals
        >::value
    ));
}

} // namespace UT
} // namespace Base
} // namespace Front
} // namespace QDeps

