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
            bind<if0, cif0>,
            c1,
            bind<c2>::in_name<int>,
            bind<c3>::in_call_stack<c4, c5>
        >(),
        per_requests <
            c6
        >(),
        singleton<c7>::in_name<double>::in_call_stack<c1>()
    ));

    typedef BOOST_TYPEOF(fusionmodule) testmodule;

    BOOST_CHECK((
        equal
        <
            vector
            <
                dependency<back::scopes::singleton, if0, cif0, vector0<>, is_same<_1, if0> >,
                dependency<back::scopes::singleton, c1, c1, vector0<>, or_< is_base_of<_1, c1>, is_same<_1, c1> > >,
                dependency<back::scopes::singleton, named<c2, int>, c2, vector0<>, or_< is_base_of<_1, named<c2, int> >, is_same<_1, named<c2, int> > > >,
                dependency<back::scopes::singleton, c3, c3, vector<c4, c5>, or_< is_base_of<_1, c3>, is_same<_1, c3> > >,
                dependency<back::scopes::per_request, c6, c6, vector0<>, or_< is_base_of<_1, c6>, is_same<_1, c6> > >,
                dependency<back::scopes::singleton, named<c7, double>, c7, vector<c1>, or_< is_base_of<_1, named<c7, double> >, is_same<_1, named<c7, double> > > >
            >,
            testmodule::dependencies
        >::value
    ));
}

} // namespace ut
} // namespace fusion
} // namespace front
} // namespace di

