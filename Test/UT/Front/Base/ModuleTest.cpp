//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/or.hpp>
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
using namespace boost;

TEST(BaseModule, Empty)
{
    struct TestModule : Module<> { };

    EXPECT_TRUE((
        equal
        <
            vector0<>,
            TestModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, TestModule::Externals>::value));
}

TEST(BaseModule, DefaultScope)
{
    struct TestModule : Module<C1> { };

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<Back::Scope::PerRequest, C1, C1, vector0<>, or_< is_base_of<_1, C1>, is_same<_1, C1> > >
            >,
            TestModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, TestModule::Externals>::value));
}

TEST(BaseModule, DefaultScopeMany)
{
    struct TestModule : Module<C1, C2, C3> { };

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<Back::Scope::PerRequest, C1, C1, vector0<>, or_< is_base_of<_1, C1>, is_same<_1, C1> > >,
                Dependency<Back::Scope::PerRequest, C2, C2, vector0<>, or_< is_base_of<_1, C2>, is_same<_1, C2> > >,
                Dependency<Back::Scope::PerRequest, C3, C3, vector0<>, or_< is_base_of<_1, C3>, is_same<_1, C3> > >
            >,
            TestModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, TestModule::Externals>::value));
}

TEST(BaseModule, DefaultScopeBind)
{
    struct TestModule : Module
        <
            Bind<If0, CIf0>,
            C1,
            Bind<C2>::InName<int>,
            Bind<C3>::InCall<C4, C5>
        >
    { };

//TODO
#if 0
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<Back::Scope::PerRequest, If0, CIf0, vector0<>, is_same<_1, If0> >,
                Dependency<Back::Scope::PerRequest, C1, C1, vector0<>, is_base_of<_1, C1> >,
                Dependency<Back::Scope::PerRequest, Named<C2, int>, C2, vector0<>, is_base_of<_1, Named<C2, int> > >,
                Dependency<Back::Scope::PerRequest, C3, C3, vector<C4, C5>, is_base_of<_1, C3> >
            >,
            TestModule::Dependencies
        >::value
    ));
#endif

    EXPECT_TRUE((equal<vector0<>, TestModule::Externals>::value));
}

TEST(BaseModule, OneScope)
{
    struct TestModule : Module
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
                Dependency<Back::Scope::Singleton, CIf0, CIf0, vector0<>, or_< is_base_of<_1, CIf0>, is_same<_1, CIf0> > >
            >,
            TestModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, TestModule::Externals>::value));
}

TEST(BaseModule, OneScopeAlias)
{
    struct TestModule : Module
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
                Dependency<Back::Scope::Singleton, CIf0, CIf0, vector0<>, or_< is_base_of<_1, CIf0>, is_same<_1, CIf0> > >
            >,
            TestModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, TestModule::Externals>::value));
}

TEST(BaseModule, OneScopeDirect)
{
    struct TestModule : Module
        <
            Singleton<CIf0>
        >
    { };

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<Back::Scope::Singleton, CIf0, CIf0, vector0<>, or_< is_base_of<_1, CIf0>, is_same<_1, CIf0> > >
            >,
            TestModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, TestModule::Externals>::value));
}

TEST(BaseModule, Many)
{
    struct TestModule : Module
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
                Dependency<Back::Scope::Singleton, C1, C1, vector0<>, or_< is_base_of<_1, C1>, is_same<_1, C1> > >,
                Dependency<Back::Scope::Singleton, C2, C2, vector0<>, or_< is_base_of<_1, C2>, is_same<_1, C2> > >,
                Dependency<Back::Scope::Singleton, C3, C3, vector0<>, or_< is_base_of<_1, C3>, is_same<_1, C3> > >
            >,
            TestModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, TestModule::Externals>::value));
}

TEST(BaseModule, ManyScopes)
{
    struct TestModule : Module
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
                Dependency<Back::Scope::Singleton, C1, C1, vector0<>, or_< is_base_of<_1, C1>, is_same<_1, C1> > >,
                Dependency<Back::Scope::Singleton, C2, C2, vector0<>, or_< is_base_of<_1, C2>, is_same<_1, C2> > >,
                Dependency<Back::Scope::PerRequest, C3, C3, vector0<>, or_< is_base_of<_1, C3>, is_same<_1, C3> > >,
                Dependency<Back::Scope::PerRequest, C4, C4, vector0<>, or_< is_base_of<_1, C4>, is_same<_1, C4> > >
            >,
            TestModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, TestModule::Externals>::value));
}

TEST(BaseModule, InCall)
{
    struct TestModule : Module
        <
            PerRequest<C1>::InCall<C2>
        >
    { };

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<Back::Scope::PerRequest, C1, C1, vector<C2>, or_< is_base_of<_1, C1>, is_same<_1, C1> > >
            >,
            TestModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, TestModule::Externals>::value));
}

TEST(BaseModule, InName)
{
    struct TestModule : Module
        <
            Singleton<C1>::InName<int>
        >
    { };

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<Back::Scope::Singleton, Named<C1, int>, C1, vector0<>, or_< is_base_of<_1, Named<C1, int> >, is_same<_1, Named<C1, int> > > >
            >,
            TestModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, TestModule::Externals>::value));
}

TEST(BaseModule, InNameInCall)
{
    struct TestModule : Module
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
                Dependency<Back::Scope::Singleton, Named<C1, int>, C1, vector<double>, or_< is_base_of<_1, Named<C1, int> >, is_same<_1, Named<C1, int> > > >,
                Dependency<Back::Scope::Singleton, Named<C2, double>, C2, vector<int>, or_< is_base_of<_1,  Named<C2, double> >, is_same<_1,  Named<C2, double> > > >
            >,
            TestModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, TestModule::Externals>::value));
}

TEST(BaseModule, InCallInName)
{
    struct TestModule : Module
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
                Dependency<Back::Scope::Singleton, Named<C1, int>, C1, vector<double>, or_< is_base_of<_1, Named<C1, int> >, is_same<_1, Named<C1, int> > > >,
                Dependency<Back::Scope::Singleton, Named<C2, double>, C2, vector<int>, or_< is_base_of<_1,  Named<C2, double> >, is_same<_1,  Named<C2, double> > > >
            >,
            TestModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, TestModule::Externals>::value));
}

TEST(BaseModule, BindIf)
{
    struct TestModule : Module
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
                Dependency<Back::Scope::Singleton, If0, CIf0, vector0<>, is_same<_1, If0> >
            >,
            TestModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, TestModule::Externals>::value));
}

TEST(BaseModule, Mix)
{
    struct TestModule : Module
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
                Dependency<Back::Scope::Singleton, If0, CIf0, vector0<>, is_same<_1, If0> >,
                Dependency<Back::Scope::Singleton, C1, C1, vector0<>, or_< is_base_of<_1, C1>, is_same<_1, C1> > >,
                Dependency<Back::Scope::Singleton, Named<C2, int>, C2, vector0<>, or_< is_base_of<_1, Named<C2, int> >, is_same<_1, Named<C2, int> > > >,
                Dependency<Back::Scope::Singleton, C3, C3, vector<C4, C5>, or_< is_base_of<_1, C3>, is_same<_1, C3> > >,
                Dependency<Back::Scope::PerRequest, C6, C6, vector0<>, or_< is_base_of<_1, C6>, is_same<_1, C6> > >,
                Dependency<Back::Scope::Singleton, Named<C7, double>, C7, vector<C1>, or_< is_base_of<_1, Named<C7, double> >, is_same<_1, Named<C7, double> > > >
            >,
            TestModule::Dependencies
        >::value
    ));

    EXPECT_TRUE((equal<vector0<>, TestModule::Externals>::value));
}

TEST(BaseModule, Externals)
{
    struct TestModule : Module
        <
            Externals<
                int
            >
        >
    { };

    EXPECT_TRUE((equal<vector0<>, TestModule::Dependencies>::value));

    EXPECT_TRUE((
        equal
        <
            vector
            <
                int
            >,
            TestModule::Externals
        >::value
    ));
}

TEST(BaseModule, ExternalsMix)
{
    struct TestModule : Module
        <
            Externals<
                int,
                double
            >,
            External<float>
        >
    { };

    EXPECT_TRUE((equal<vector0<>, TestModule::Dependencies>::value));

    EXPECT_TRUE((
        equal
        <
            vector
            <
                int, double, float
            >,
            TestModule::Externals
        >::value
    ));
}

TEST(BaseModule, ExternalsBind)
{
    struct TestModule : Module
        <
            Externals<
                Bind<C1>::InName<int>
            >
        >
    { };

    EXPECT_TRUE((equal<vector0<>, TestModule::Dependencies>::value));

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Bind<C1>::InName<int>
            >,
            TestModule::Externals
        >::value
    ));
}

TEST(BaseModule, NamedInCall)
{
    struct TestModule : Front::Base::Module
        <
            PerRequests <
                Bind<int, int_<1> >,
                Bind<int, int_<4> >::InName< string<'2'> >::InCall<C7, C6, C4>,
                Bind<int, int_<5> >::InCall<C2>
            >
        >
    { };

    EXPECT_TRUE((equal<vector0<>, TestModule::Externals>::value));

    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<Back::Scope::PerRequest, int, int_<1>, vector0<>, or_< is_base_of<_1, int>, is_same<_1, int> > >,
                Dependency<Back::Scope::PerRequest, Named<int, string<'2'> >, int_<4>, vector<C7, C6, C4>, or_< is_base_of<_1, Named<int, string<'2'> > >, is_same<_1, Named<int, string<'2'> > > > >,
                Dependency<Back::Scope::PerRequest, int, int_<5>, vector<C2>, or_< is_base_of<_1, int>, is_same<_1, int> > >
            >,
            TestModule::Dependencies
        >::value
    ));
}

} // namespace UT
} // namespace Base
} // namespace Front
} // namespace QDeps

