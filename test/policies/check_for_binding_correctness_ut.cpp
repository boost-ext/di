//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/policies/check_for_binding_correctness.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/none_t.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/vector.hpp>

#include "fake_dependency.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace policies {

BOOST_AUTO_TEST_CASE(empty) {
    BOOST_CHECK((
        !is_base_of<
            mpl::false_
          , check_for_binding_correctness::verify<
                mpl::vector0<>
              , mpl::vector0<>
              , none_t
              , false
            >::type
         >::value
    ));
}

BOOST_AUTO_TEST_CASE(not_correct) {
    BOOST_CHECK((
        is_base_of<
            mpl::false_
          , check_for_binding_correctness::verify<
                mpl::vector<
                    fake_dependency<none_t, a, a, c2>
                >
              , mpl::vector0<>
              , none_t
              , false
            >::type
         >::value
    ));
}

BOOST_AUTO_TEST_CASE(not_correct_call_stack) {
    BOOST_CHECK((
        is_base_of<
            mpl::false_
          , check_for_binding_correctness::verify<
                mpl::vector<
                    fake_dependency<none_t, a, a, mpl::vector<c1, c2> >
                >
              , mpl::vector0<>
              , none_t
              , false
            >::type
         >::value
    ));
}

BOOST_AUTO_TEST_CASE(correct_many) {
    BOOST_CHECK((
        !is_base_of<
            mpl::false_
          , check_for_binding_correctness::verify<
                mpl::vector<
                    fake_dependency<none_t, int, int, mpl::vector<c1, c3> >
                  , fake_dependency<none_t, int, int, c9, c14>
                >
              , mpl::vector0<>
              , none_t
              , false
            >::type
         >::value
    ));
}

} // namespace policies
} // namespace di
} // namespace boost

