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
#include "boost/di/aux/dependency.hpp"
#include "boost/di/named.hpp"
#include "boost/di/fusion_module.hpp"
#include "common.hpp"
#include "data.hpp"

namespace boost {
namespace di {

BOOST_AUTO_TEST_CASE(fusion_module_empty)
{
    BOOST_AUTO(module, fusion_module<>()());
    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        mpl::equal<
            mpl::vector0<>
          , module_t::dependencies
        >::value
    ));

    BOOST_CHECK((mpl::equal<mpl::vector0<>, module_t::pool::sequence>::value));
}

BOOST_AUTO_TEST_CASE(fusion_module_mix)
{
    BOOST_AUTO(module, fusion_module<>()(
        singletons<
            bind<if0, c0if0>
          , c1
          , bind<c2>::in_name<int>
          , bind<c3>::in_call<call_stack<c4, c5> >
        >()
      , per_requests <
            c6
        >()
      , singleton<c7>::in_name<double>::in_call<c1>()
    ));

    typedef BOOST_TYPEOF(module) module_t;

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                dependency<scopes::singleton, if0, c0if0>::type
              , dependency_base_of<scopes::singleton, c1, c1>::type
              , dependency_base_of<scopes::singleton, named<c2, int>, c2>::type
              , dependency_base_of<scopes::singleton, c3, c3, call_stack<c4, c5> >::type
              , dependency_base_of<scopes::per_request, c6, c6>::type
              , dependency_base_of<scopes::singleton, named<c7, double>, c7, c1>::type
            >
          , module_t::dependencies
        >::value
    ));
}

} // namespace di
} // namespace boost

