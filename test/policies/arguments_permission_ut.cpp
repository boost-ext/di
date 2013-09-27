//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/policies/arguments_permission.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

#include "contains_all.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace policies {

BOOST_AUTO_TEST_CASE(none) {
    BOOST_CHECK((
        contains_all<
            mpl::vector<int, double, char>
          , arguments_permission<>::verify<
                mpl::vector0<>
              , c2
              , mpl::false_
            >::type
         >::value
    ));
}

BOOST_AUTO_TEST_CASE(allow_requires) {
    BOOST_CHECK((
        contains_all<
            mpl::vector0<>
          , arguments_permission<allow_copies>::verify<
                mpl::vector0<>
              , c2
              , mpl::false_
            >::type
         >::value
    ));
}

BOOST_AUTO_TEST_CASE(allow_some_of_requirements) {
    BOOST_CHECK((
        contains_all<
            mpl::vector<shared_ptr<int>, const std::string&>
          , arguments_permission<allow_ptrs, allow_copies>::verify<
                mpl::vector0<>
              , c24
              , mpl::false_
            >::type
         >::value
    ));
}

BOOST_AUTO_TEST_CASE(allow_some_of_requirements_order) {
    BOOST_CHECK((
        contains_all<
            mpl::vector<shared_ptr<int>, const std::string&>
          , arguments_permission<allow_copies, allow_ptrs>::verify<
                mpl::vector0<>
              , c24
              , mpl::false_
            >::type
         >::value
    ));
}

BOOST_AUTO_TEST_CASE(allow_all_of_requirements) {
    BOOST_CHECK((
        contains_all<
            mpl::vector0<>
          , arguments_permission<
                allow_copies, allow_ptrs, allow_smart_ptrs, allow_refs
            >::verify<
                mpl::vector0<>
              , c24
              , mpl::false_
            >::type
         >::value
    ));
}

} // namespace policies
} // namespace di
} // namespace boost

