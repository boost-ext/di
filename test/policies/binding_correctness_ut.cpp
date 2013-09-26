//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/policies/binding_correctness.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/none_t.hpp>
#include <boost/mpl/vector.hpp>

#include "fake_dependency.hpp"
#include "fake_dependency_base_of.hpp"
#include "contains_all.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace policies {

BOOST_AUTO_TEST_CASE(empty) {
    BOOST_CHECK((
        contains_all<
            mpl::vector0<>
          , binding_correctness::verify<
                mpl::vector0<>
              , none_t
              , false
            >::type
         >::value
    ));
}

BOOST_AUTO_TEST_CASE(not_correct) {
    BOOST_CHECK((
        contains_all<
            mpl::vector<
                mpl::pair<a, c2>
            >
          , binding_correctness::verify<
                mpl::vector<
                    fake_dependency<none_t, a, a, c2>
                >
              , none_t
              , false
            >::type
         >::value
    ));
}

BOOST_AUTO_TEST_CASE(not_correct_call_stack) {
    BOOST_CHECK((
        contains_all<
            mpl::vector<
                mpl::pair<a, c2>
            >
          , binding_correctness::verify<
                mpl::vector<
                    fake_dependency<none_t, a, a, mpl::vector<c1, c2>>
                >
              , none_t
              , false
            >::type
         >::value
    ));
}

BOOST_AUTO_TEST_CASE(correct_many) {
    BOOST_CHECK((
        contains_all<
            mpl::vector0<>
          , binding_correctness::verify<
                mpl::vector<
                    fake_dependency<none_t, int, int, mpl::vector<c1, c3>>
                  , fake_dependency<none_t, int, int, c9, c14>
                >
              , none_t
              , false
            >::type
         >::value
    ));
}

BOOST_AUTO_TEST_CASE(none_singletons) {
    BOOST_CHECK((
        contains_all<
            mpl::vector<>
          , binding_correctness::verify<
                mpl::vector<
                    fake_dependency<scopes::per_request<>, int, mpl::int_<42>, ub1>::type
                  , fake_dependency_base_of<scopes::per_request<>, ub1>::type
                >
              , ub3
              , false
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(with_call) {
    BOOST_CHECK((
        contains_all<
            mpl::vector<
                ub1
            >
          , binding_correctness::verify<
                mpl::vector<
                    fake_dependency<scopes::per_request<>, int, mpl::int_<42>, ub1>::type
                  , fake_dependency_base_of<scopes::singleton<>, ub1>::type
                >
              , ub3
              , false
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(with_call_stack) {
    BOOST_CHECK((
        contains_all<
            mpl::vector<
                ub1
            >
          , binding_correctness::verify<
                mpl::vector<
                    fake_dependency<scopes::per_request<>, int, mpl::int_<42>, mpl::vector<ub2, ub1>>::type
                  , fake_dependency_base_of<scopes::singleton<>, ub1>::type
                >
              , ub3
              , false
            >::type
        >::value
    ));
}

} // namespace policies
} // namespace di
} // namespace boost

