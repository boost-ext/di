//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "fake_assert.hpp" // has to be first
#include "boost/di/policies/arguments_permission.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

#include "contains_all.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace policies {

BOOST_AUTO_TEST_CASE(value) {
    BOOST_REQUIRE_EXCEPTION(
        (
            arguments_permission<>::assert_policy<
                mpl::vector0<>
              , int
            >()
        )
      , assert_exception
      , [](const assert_exception& e) {
            return e.get_msg() == "ARGUMENT_NOT_PERMITTED" &&
                   e.get_type() == typeid(int);
        }
    );
}

BOOST_AUTO_TEST_CASE(none) {
    BOOST_REQUIRE_EXCEPTION(
        (
            arguments_permission<>::assert_policy<
                mpl::vector0<>
              , c2
            >()
        )
      , assert_exception
      , [](const assert_exception& e) {
            return e.get_msg() == "ARGUMENT_NOT_PERMITTED" &&
                   e.get_type() == typeid(c2);
        }
    );
}

BOOST_AUTO_TEST_CASE(allow_copy) {
    BOOST_CHECK_NO_THROW(
        (
            arguments_permission<allow_copies>::assert_policy<
                mpl::vector0<>
              , c2
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_ptr) {
    BOOST_CHECK_NO_THROW(
        (
            arguments_permission<allow_ptrs>::assert_policy<
                mpl::vector0<>
              , c2*
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_const_ref) {
    BOOST_CHECK_NO_THROW(
        (
            arguments_permission<allow_const_refs>::assert_policy<
                mpl::vector0<>
              , const c2&
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_ref) {
    BOOST_CHECK_NO_THROW(
        (
            arguments_permission<allow_refs>::assert_policy<
                mpl::vector0<>
              , c2&
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_ref_from_const_ref) {
    BOOST_CHECK_NO_THROW(
        (
            arguments_permission<allow_refs>::assert_policy<
                mpl::vector0<>
              , const c2&
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_rvalue_ref) {
    BOOST_CHECK_NO_THROW(
        (
            arguments_permission<allow_rvalue_refs>::assert_policy<
                mpl::vector0<>
              , c2&&
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_smart_ptr) {
    BOOST_CHECK_NO_THROW(
        (
            arguments_permission<allow_smart_ptrs>::assert_policy<
                mpl::vector0<>
              , aux::shared_ptr<c2>
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_many_order) {
    BOOST_CHECK_NO_THROW(
        (
            arguments_permission<allow_refs, allow_ptrs>::assert_policy<
                mpl::vector0<>
              , const c2*
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_nested_type) {
    BOOST_CHECK_NO_THROW(
        (
            arguments_permission<allow_copies, allow_ptrs>::assert_policy<
                mpl::vector0<>
              , std::vector<int*>
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(disallow_nested_type_copy) {
    BOOST_REQUIRE_EXCEPTION(
        (
            arguments_permission<>::assert_policy<
                mpl::vector0<>
              , std::vector<int*>
            >()
        )
      , assert_exception
      , [](const assert_exception& e) {
            return e.get_msg() == "ARGUMENT_NOT_PERMITTED" &&
                   e.get_type() == typeid(std::vector<int*>);
        }
    );
}

BOOST_AUTO_TEST_CASE(disallow_nested_type_nested_ptr) {
    BOOST_REQUIRE_EXCEPTION(
        (
            arguments_permission<allow_copies>::assert_policy<
                mpl::vector0<>
              , std::vector<int*>
            >()
        )
      , assert_exception
      , [](const assert_exception& e) {
            return e.get_msg() == "ARGUMENT_NOT_PERMITTED" &&
                   e.get_type() == typeid(std::vector<int*>);
        }
    );
}

} // namespace policies
} // namespace di
} // namespace boost

