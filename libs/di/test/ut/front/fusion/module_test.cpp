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
#include "test/common/data.hpp"
#include "di/back/aux/dependency.hpp"
#include "di/front/fusion/module.hpp"
#include "di/utility/named.hpp"

namespace di
{
namespace front
{
namespace fusion
{
namespace ut
{

using namespace test::common;
using namespace back;
using namespace back::aux;
using namespace utility;
using namespace boost::mpl;
using namespace boost;

BOOST_AUTO_TEST_CASE(fusionEmpty)
{
    BOOST_AUTO(fusionmodule, module<>()());
    typedef BOOST_TYPEOF(fusionmodule) testmodule;

    BOOST_CHECK((
        equal
        <
            vector0<>,
            testmodule::dependencies
        >::value
    ));

    BOOST_CHECK((equal<vector0<>, testmodule::pool::seq>::value));
}

BOOST_AUTO_TEST_CASE(fusionMix)
{
    BOOST_AUTO(fusionmodule, module<>()(
        singletons<
            bind<If0, CIf0>,
            C1,
            bind<C2>::in_name<int>,
            bind<C3>::in_call_stack<C4, C5>
        >(),
        per_requests <
            C6
        >(),
        singleton<C7>::in_name<double>::in_call_stack<C1>()
    ));

    typedef BOOST_TYPEOF(fusionmodule) testmodule;

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, If0, CIf0, vector0<>, is_same<_1, If0> >,
                dependency<back::scopes::singleton, C1, C1, vector0<>, or_< is_base_of<_1, C1>, is_same<_1, C1> > >,
                dependency<back::scopes::singleton, named<C2, int>, C2, vector0<>, or_< is_base_of<_1, named<C2, int> >, is_same<_1, named<C2, int> > > >,
                dependency<back::scopes::singleton, C3, C3, vector<C4, C5>, or_< is_base_of<_1, C3>, is_same<_1, C3> > >,
                dependency<back::scopes::per_request, C6, C6, vector0<>, or_< is_base_of<_1, C6>, is_same<_1, C6> > >,
                dependency<back::scopes::singleton, named<C7, double>, C7, vector<C1>, or_< is_base_of<_1, named<C7, double> >, is_same<_1, named<C7, double> > > >
            >,
            testmodule::dependencies
        >::value
    ));
}

} // namespace ut
} // namespace fusion
} // namespace front
} // namespace di

