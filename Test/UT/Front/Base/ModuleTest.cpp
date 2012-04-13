//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/or.hpp>
#include "Test/Common/Data.hpp"
#include "QDeps/Back/Aux/Instance.hpp"
#include "QDeps/Back/Aux/Dependency.hpp"
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
using namespace Back::Aux;
using namespace Back::Scopes;
using namespace Utility;
using namespace boost::mpl;
using namespace boost;

BOOST_AUTO_TEST_CASE(Empty)
{
    struct TestModule : Module<> { };

    BOOST_CHECK((
        equal
        <
            vector0<>,
            TestModule::Dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, TestModule::Pool::Seq>::value));
}

BOOST_AUTO_TEST_CASE(DefaultScope)
{
    struct TestModule : Module<C1> { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                Dependency<Back::Scopes::PerRequest, C1, C1, vector0<>, or_< is_base_of<_1, C1>, is_same<_1, C1> > >
            >,
            TestModule::Dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, TestModule::Pool::Seq>::value));
}

BOOST_AUTO_TEST_CASE(DefaultScopeMany)
{
    struct TestModule : Module<C1, C2, C3> { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                Dependency<Back::Scopes::PerRequest, C1, C1, vector0<>, or_< is_base_of<_1, C1>, is_same<_1, C1> > >,
                Dependency<Back::Scopes::PerRequest, C2, C2, vector0<>, or_< is_base_of<_1, C2>, is_same<_1, C2> > >,
                Dependency<Back::Scopes::PerRequest, C3, C3, vector0<>, or_< is_base_of<_1, C3>, is_same<_1, C3> > >
            >,
            TestModule::Dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, TestModule::Pool::Seq>::value));
}

BOOST_AUTO_TEST_CASE(DefaultScopeBind)
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
    BOOST_CHECK((
        equal
        <
            vector
            <
                Dependency<Back::Scopes::PerRequest, If0, CIf0, vector0<>, is_same<_1, If0> >,
                Dependency<Back::Scopes::PerRequest, C1, C1, vector0<>, is_base_of<_1, C1> >,
                Dependency<Back::Scopes::PerRequest, Named<C2, int>, C2, vector0<>, is_base_of<_1, Named<C2, int> > >,
                Dependency<Back::Scopes::PerRequest, C3, C3, vector<C4, C5>, is_base_of<_1, C3> >
            >,
            TestModule::Dependencies
        >::value
    ));
#endif

    BOOST_CHECK((equal<vector0<>, TestModule::Pool::Seq>::value));
}

BOOST_AUTO_TEST_CASE(OneScope)
{
    struct TestModule : Module
        <
            Scope<Back::Scopes::Singleton>::Bind <
                CIf0
            >
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                Dependency<Back::Scopes::Singleton, CIf0, CIf0, vector0<>, or_< is_base_of<_1, CIf0>, is_same<_1, CIf0> > >
            >,
            TestModule::Dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, TestModule::Pool::Seq>::value));
}

BOOST_AUTO_TEST_CASE(OneScopeAlias)
{
    struct TestModule : Module
        <
            Singletons <
                CIf0
            >
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                Dependency<Back::Scopes::Singleton, CIf0, CIf0, vector0<>, or_< is_base_of<_1, CIf0>, is_same<_1, CIf0> > >
            >,
            TestModule::Dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, TestModule::Pool::Seq>::value));
}

BOOST_AUTO_TEST_CASE(OneScopeDirect)
{
    struct TestModule : Module
        <
            Singleton<CIf0>
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                Dependency<Back::Scopes::Singleton, CIf0, CIf0, vector0<>, or_< is_base_of<_1, CIf0>, is_same<_1, CIf0> > >
            >,
            TestModule::Dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, TestModule::Pool::Seq>::value));
}

BOOST_AUTO_TEST_CASE(Many)
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

    BOOST_CHECK((
        equal
        <
            vector
            <
                Dependency<Back::Scopes::Singleton, C1, C1, vector0<>, or_< is_base_of<_1, C1>, is_same<_1, C1> > >,
                Dependency<Back::Scopes::Singleton, C2, C2, vector0<>, or_< is_base_of<_1, C2>, is_same<_1, C2> > >,
                Dependency<Back::Scopes::Singleton, C3, C3, vector0<>, or_< is_base_of<_1, C3>, is_same<_1, C3> > >
            >,
            TestModule::Dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, TestModule::Pool::Seq>::value));
}

BOOST_AUTO_TEST_CASE(ManyScopes)
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

    BOOST_CHECK((
        equal
        <
            vector
            <
                Dependency<Back::Scopes::Singleton, C1, C1, vector0<>, or_< is_base_of<_1, C1>, is_same<_1, C1> > >,
                Dependency<Back::Scopes::Singleton, C2, C2, vector0<>, or_< is_base_of<_1, C2>, is_same<_1, C2> > >,
                Dependency<Back::Scopes::PerRequest, C3, C3, vector0<>, or_< is_base_of<_1, C3>, is_same<_1, C3> > >,
                Dependency<Back::Scopes::PerRequest, C4, C4, vector0<>, or_< is_base_of<_1, C4>, is_same<_1, C4> > >
            >,
            TestModule::Dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, TestModule::Pool::Seq>::value));
}

BOOST_AUTO_TEST_CASE(InCall)
{
    struct TestModule : Module
        <
            PerRequest<C1>::InCall<C2>
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                Dependency<Back::Scopes::PerRequest, C1, C1, vector<C2>, or_< is_base_of<_1, C1>, is_same<_1, C1> > >
            >,
            TestModule::Dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, TestModule::Pool::Seq>::value));
}

BOOST_AUTO_TEST_CASE(InName)
{
    struct TestModule : Module
        <
            Singleton<C1>::InName<int>
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                Dependency<Back::Scopes::Singleton, Named<C1, int>, C1, vector0<>, or_< is_base_of<_1, Named<C1, int> >, is_same<_1, Named<C1, int> > > >
            >,
            TestModule::Dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, TestModule::Pool::Seq>::value));
}

BOOST_AUTO_TEST_CASE(InNameInCall)
{
    struct TestModule : Module
        <
            Singletons<
                Bind<C1>::InName<int>::InCall<double>,
                Bind<C2>::InName<double>::InCall<int>
            >
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                Dependency<Back::Scopes::Singleton, Named<C1, int>, C1, vector<double>, or_< is_base_of<_1, Named<C1, int> >, is_same<_1, Named<C1, int> > > >,
                Dependency<Back::Scopes::Singleton, Named<C2, double>, C2, vector<int>, or_< is_base_of<_1,  Named<C2, double> >, is_same<_1,  Named<C2, double> > > >
            >,
            TestModule::Dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, TestModule::Pool::Seq>::value));
}

BOOST_AUTO_TEST_CASE(InCallInName)
{
    struct TestModule : Module
        <
            Singletons<
                Bind<C1>::InCall<double>::InName<int>,
                Bind<C2>::InCall<int>::InName<double>
            >
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                Dependency<Back::Scopes::Singleton, Named<C1, int>, C1, vector<double>, or_< is_base_of<_1, Named<C1, int> >, is_same<_1, Named<C1, int> > > >,
                Dependency<Back::Scopes::Singleton, Named<C2, double>, C2, vector<int>, or_< is_base_of<_1,  Named<C2, double> >, is_same<_1,  Named<C2, double> > > >
            >,
            TestModule::Dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, TestModule::Pool::Seq>::value));
}

BOOST_AUTO_TEST_CASE(BindIf)
{
    struct TestModule : Module
        <
            Singletons<
                Bind<If0, CIf0>
            >
        >
    { };

    BOOST_CHECK((
        equal
        <
            vector
            <
                Dependency<Back::Scopes::Singleton, If0, CIf0, vector0<>, is_same<_1, If0> >
            >,
            TestModule::Dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, TestModule::Pool::Seq>::value));
}

BOOST_AUTO_TEST_CASE(Mix)
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

    BOOST_CHECK((
        equal
        <
            vector
            <
                Dependency<Back::Scopes::Singleton, If0, CIf0, vector0<>, is_same<_1, If0> >,
                Dependency<Back::Scopes::Singleton, C1, C1, vector0<>, or_< is_base_of<_1, C1>, is_same<_1, C1> > >,
                Dependency<Back::Scopes::Singleton, Named<C2, int>, C2, vector0<>, or_< is_base_of<_1, Named<C2, int> >, is_same<_1, Named<C2, int> > > >,
                Dependency<Back::Scopes::Singleton, C3, C3, vector<C4, C5>, or_< is_base_of<_1, C3>, is_same<_1, C3> > >,
                Dependency<Back::Scopes::PerRequest, C6, C6, vector0<>, or_< is_base_of<_1, C6>, is_same<_1, C6> > >,
                Dependency<Back::Scopes::Singleton, Named<C7, double>, C7, vector<C1>, or_< is_base_of<_1, Named<C7, double> >, is_same<_1, Named<C7, double> > > >
            >,
            TestModule::Dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, TestModule::Pool::Seq>::value));
}

BOOST_AUTO_TEST_CASE(NamedInCall)
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

    BOOST_CHECK((equal<vector0<>, TestModule::Pool::Seq>::value));

    BOOST_CHECK((
        equal
        <
            vector
            <
                Dependency<Back::Scopes::PerRequest, int, int_<1>, vector0<>, or_< is_base_of<_1, int>, is_same<_1, int> > >,
                Dependency<Back::Scopes::PerRequest, Named<int, string<'2'> >, int_<4>, vector<C7, C6, C4>, or_< is_base_of<_1, Named<int, string<'2'> > >, is_same<_1, Named<int, string<'2'> > > > >,
                Dependency<Back::Scopes::PerRequest, int, int_<5>, vector<C2>, or_< is_base_of<_1, int>, is_same<_1, int> > >
            >,
            TestModule::Dependencies
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(ExternalsBase)
{
    struct TestModule : Module
        <
            Externals<
                C1
            >
        >
    { };

    BOOST_CHECK((equal<vector0<>, TestModule::Dependencies>::value));

    BOOST_CHECK((
        equal
        <
            vector
            <
                Instance<C1>
            >,
            TestModule::Pool::Seq
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(ExternalsMix)
{
    struct TestModule : Module
        <
            Externals<
                C1,
                C2
            >,
            External<C3>
        >
    { };

    BOOST_CHECK((equal<vector0<>, TestModule::Dependencies>::value));

    BOOST_CHECK((
        equal
        <
            vector
            <
                Instance<C1>,
                Instance<C2>,
                Instance<C3>
            >,
            TestModule::Pool::Seq
        >::value
    ));
}

//TODO
#if 0
BOOST_AUTO_TEST_CASE(ExternalsBind)
{
    struct TestModule : Module
        <
            Externals<
                Bind<C1>::InName<int>
            >
        >
    { };

    BOOST_CHECK((equal<vector0<>, TestModule::Dependencies>::value));

    BOOST_CHECK((
        equal
        <
            vector
            <
                Instance< Named<C1, int> >
            >,
            TestModule::Pool::Seq
        >::value
    ));

    std::cout << "DUPA:" << abi::__cxa_demangle(typeid(TestModule::Pool::Seq::type).name(), 0, 0, 0) << std::endl;
}
#endif

} // namespace UT
} // namespace Base
} // namespace Front
} // namespace QDeps

