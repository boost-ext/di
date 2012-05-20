//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/policies/check_for_circular_dependencies.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include "data.hpp"

namespace boost {
namespace di {
namespace policies {

BOOST_AUTO_TEST_CASE(basic) {
    BOOST_CHECK((
        !is_base_of<
            mpl::false_
          , check_for_circular_dependencies::verify<
                mpl::vector0<>
              , mpl::vector0<>
              , c8
              , false
            >::type
         >::value
    ));
}

BOOST_AUTO_TEST_CASE(direct) {
    BOOST_CHECK((
        is_base_of<
            mpl::false_
          , check_for_circular_dependencies::verify<
                mpl::vector0<>
              , mpl::vector0<>
              , cd1
              , false
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(in_direct) {
    BOOST_CHECK((
        is_base_of<
            mpl::false_
          , check_for_circular_dependencies::verify<
                mpl::vector0<>
              , mpl::vector0<>
              , cd5
              , false
            >::type
        >::value
    ));
}

} // namespace policies
} // namespace di
} // namespace boost

