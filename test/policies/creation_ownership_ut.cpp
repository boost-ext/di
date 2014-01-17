//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/policies/creation_ownership.hpp"

#include "boost/di/aux_/memory.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace boost {
namespace di {
namespace policies {

BOOST_AUTO_TEST_CASE(clear) {
    BOOST_CHECK((
        is_base_of<
            mpl::true_
          , creation_ownership::verify<
                mpl::vector0<>
              , int
              , mpl::false_
            >::type
        >::value
    ));

    BOOST_CHECK((
        is_base_of<
            mpl::true_
          , creation_ownership::verify<
                mpl::vector0<>
              , int*
              , mpl::false_
            >::type
        >::value
    ));

    BOOST_CHECK((
        is_base_of<
            mpl::true_
          , creation_ownership::verify<
                mpl::vector0<>
              , const int*
              , mpl::false_
            >::type
        >::value
    ));

    BOOST_CHECK((
        is_base_of<
            mpl::true_
          , creation_ownership::verify<
                mpl::vector0<>
              , aux::shared_ptr<int>
              , mpl::false_
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(not_clear) {
    BOOST_CHECK((
        is_base_of<
            mpl::false_
          , creation_ownership::verify<
                mpl::vector0<>
              , int&
              , mpl::false_
            >::type
        >::value
    ));

    BOOST_CHECK((
        is_base_of<
            mpl::false_
          , creation_ownership::verify<
                mpl::vector0<>
              , const int&
              , mpl::false_
            >::type
        >::value
    ));
}

} // namespace policies
} // namespace di
} // namespace boost

