//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/mpl::vector.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/or.hpp>
#include "boost/di/aux/dependency.hpp"
#include "boost/di/named.hpp"
#include "boost/di/fusion_module.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace test {
namespace ut {

BOOST_AUTO_TEST_CASE(fusion_module_empty)
{
    BOOST_AUTO(module, fusion_module<>()());
    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        equal<
            mpl::vector0<>
          , module_t::dependencies
        >::value
    ));

    BOOST_CHECK((equal<mpl::vector0<>, module_t::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(fusion_module_mix)
{
    BOOST_AUTO(module, fusion_module<>()(
        singletons<
            bind<if0, c0if0>
          , c1
          , bind<c2>::in_name<int>
          , bind<c3>::in_call_stack<c4, c5>
        >()
      , per_requests <
            c6
        >()
      , singleton<c7>::in_name<double>::in_call_stack<c1>()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        equal<
            mpl::vector<
                dependency<back::scopes::singleton, if0, c0if0, mpl::vector0<>, is_same<mpl::_1, if0> >,
              , dependency<back::scopes::singleton, c1, c1, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, c1>, is_same<mpl::_1, c1> > >
              , dependency<back::scopes::singleton, named<c2, int>, c2, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, named<c2, int> >, is_same<mpl::_1, named<c2, int> > > >
              , dependency<back::scopes::singleton, c3, c3, mpl::vector<c4, c5>, mpl::or_< is_base_of<mpl::_1, c3>, is_same<mpl::_1, c3> > >
              , dependency<back::scopes::per_request, c6, c6, mpl::vector0<>, mpl::or_< is_base_of<mpl::_1, c6>, is_same<mpl::_1, c6> > >
              , dependency<back::scopes::singleton, named<c7, double>, c7, mpl::vector<c1>, mpl::or_< is_base_of<mpl::_1, named<c7, double> >, is_same<mpl::_1, named<c7, double> > > >
            >
          , module_t::dependencies
        >::value
    ));
}

} // namespace ut
} // namespace test
} // namespace di
} // namespace boost

