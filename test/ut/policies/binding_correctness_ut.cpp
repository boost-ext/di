//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/policies/binding_correctness.hpp"
#include "boost/di/scopes/shared.hpp"
#include "boost/di/scopes/session.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
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
              , mpl::false_
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
                    fake_dependency<none_t, a, a, void, c2>
                >
              , none_t
              , mpl::false_
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
                    fake_dependency<none_t, a, a, void, mpl::vector<c1, c2>>
                >
              , none_t
              , mpl::false_
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
                    fake_dependency<none_t, int, int, void, mpl::vector<c1, c3>>
                  , fake_dependency<none_t, int, int, void, c9, c14>
                >
              , none_t
              , mpl::false_
            >::type
         >::value
    ));
}

BOOST_AUTO_TEST_CASE(none_shareds) {
    BOOST_CHECK((
        contains_all<
            mpl::vector<>
          , binding_correctness::verify<
                mpl::vector<
                    fake_dependency<scopes::unique<>, int, mpl::int_<42>, void, mpl::vector<call_stack<ub1>>>::type
                  , fake_dependency_base_of<scopes::unique<>, ub1>::type
                >
              , ub3
              , mpl::false_
            >::type
        >::value
    ));
}

using scope_types = mpl::vector<
    scopes::shared<>
  , scopes::session<>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(with_call, TScope, scope_types) {
    BOOST_CHECK((
        contains_all<
            mpl::vector<
                ub1
            >
          , typename binding_correctness::verify<
                mpl::vector<
                    fake_dependency<scopes::unique<>, int, mpl::int_<42>, void, mpl::vector<call_stack<ub1>>>::type
                  , typename fake_dependency_base_of<TScope, ub1>::type
                >
              , ub3
              , mpl::false_
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(with_call_stack, TScope, scope_types) {
    BOOST_CHECK((
        contains_all<
            mpl::vector<
                ub1
            >
          , typename binding_correctness::verify<
                mpl::vector<
                    fake_dependency<scopes::unique<>, int, mpl::int_<42>, void, mpl::vector<ub2, ub1>>::type
                  , typename fake_dependency_base_of<TScope, ub1>::type
                >
              , ub3
              , mpl::false_
            >::type
        >::value
    ));
}

} // namespace policies
} // namespace di
} // namespace boost

