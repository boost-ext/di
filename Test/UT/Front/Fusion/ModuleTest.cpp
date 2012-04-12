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
#include "QDeps/Back/Aux/Dependency.hpp"
#include "QDeps/Front/Fusion/Module.hpp"
#include "QDeps/Utility/Named.hpp"

namespace QDeps
{
namespace Front
{
namespace Fusion
{
namespace UT
{

using namespace Test::Common;
using namespace Back;
using namespace Back::Aux;
using namespace Utility;
using namespace boost::mpl;
using namespace boost;

BOOST_AUTO_TEST_CASE(FusionEmpty)
{
    BOOST_AUTO(fusionModule, Module<>()());
    typedef BOOST_TYPEOF(fusionModule) TestModule;

    BOOST_CHECK((
        equal
        <
            vector0<>,
            TestModule::Dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, TestModule::Pool::Seq>::value));
}

BOOST_AUTO_TEST_CASE(FusionMix)
{
    BOOST_AUTO(fusionModule, Module<>()(
        Singletons<
            Bind<If0, CIf0>,
            C1,
            Bind<C2>::InName<int>,
            Bind<C3>::InCall<C4, C5>
        >(),
        PerRequests <
            C6
        >(),
        Singleton<C7>::InName<double>::InCall<C1>()
    ));

    typedef BOOST_TYPEOF(fusionModule) TestModule;

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
}

} // namespace UT
} // namespace Fusion
} // namespace Front
} // namespace QDeps

