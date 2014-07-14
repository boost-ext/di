//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "common/fakes/fake_assert.hpp" // has to be first
#include "boost/di/aux_/config.hpp"
#undef BOOST_DI_WKND_NO_MSVC
#define BOOST_DI_WKND_NO_MSVC(...) __VA_ARGS__
#undef BOOST_DI_WKND_MSVC
#define BOOST_DI_WKND_MSVC(...)
#include "boost/di/policies/parameters_permission.hpp"
#undef BOOST_DI_WKND_NO_MSVC
#define BOOST_DI_WKND_NO_MSVC(...)
#undef BOOST_DI_WKND_MSVC
#define BOOST_DI_WKND_MSVC(...) __VA_ARGS__

#include <boost/test/unit_test.hpp>
#include <boost/mpl/identity.hpp>

#include "common/data.hpp"

namespace boost {
namespace di {
namespace policies {

template<typename T>
bool verify_assert_exception(const assert_exception& e) {
    return e.what() == std::string("PARAMETER_NOT_PERMITTED") &&
           e.get_type() == typeid(T);
}

BOOST_AUTO_TEST_CASE(value) {
    BOOST_REQUIRE_EXCEPTION(
        (
            parameters_permission<>().assert_policy<
                mpl::identity<int>
            >()
        )
      , assert_exception
      , verify_assert_exception<int>
    );
}

BOOST_AUTO_TEST_CASE(none) {
    BOOST_REQUIRE_EXCEPTION(
        (
            parameters_permission<>().assert_policy<
                mpl::identity<c2>
            >()
        )
      , assert_exception
      , verify_assert_exception<c2>
    );
}

BOOST_AUTO_TEST_CASE(allow_copy) {
    BOOST_CHECK_NO_THROW(
        (
            parameters_permission<allow_copies>().assert_policy<
                mpl::identity<c2>
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_ptr) {
    BOOST_CHECK_NO_THROW(
        (
            parameters_permission<allow_ptrs>().assert_policy<
                mpl::identity<c2*>
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_const_ref) {
    BOOST_CHECK_NO_THROW(
        (
            parameters_permission<allow_const_refs>().assert_policy<
                mpl::identity<const c2&>
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_ref) {
    BOOST_CHECK_NO_THROW(
        (
            parameters_permission<allow_refs>().assert_policy<
                mpl::identity<c2&>
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_rvalue_ref) {
    BOOST_CHECK_NO_THROW(
        (
            parameters_permission<allow_rvalue_refs>().assert_policy<
                mpl::identity<c2&&>
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_smart_ptr) {
    BOOST_CHECK_NO_THROW(
        (
            parameters_permission<allow_smart_ptrs>().assert_policy<
                mpl::identity<aux::shared_ptr<c2>>
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_many_order) {
    BOOST_CHECK_NO_THROW(
        (
            parameters_permission<allow_refs, allow_ptrs>().assert_policy<
                mpl::identity<const c2*>
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_nested_type) {
    BOOST_CHECK_NO_THROW(
        (
            parameters_permission<allow_copies, allow_ptrs>().assert_policy<
                mpl::identity<std::vector<int*>>
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(disallow_ref_from_const_ref) {
    BOOST_REQUIRE_EXCEPTION(
        (
            parameters_permission<allow_refs>().assert_policy<
                mpl::identity<const c2&>
            >()
        )
      , assert_exception
      , verify_assert_exception<const c2&>
    );
}

BOOST_AUTO_TEST_CASE(disallow_nested_type_copy) {
    BOOST_REQUIRE_EXCEPTION(
        (
            parameters_permission<>().assert_policy<
                mpl::identity<std::vector<int*>>
            >()
        )
      , assert_exception
      , verify_assert_exception<std::vector<int*>>
    );
}

BOOST_AUTO_TEST_CASE(disallow_nested_type_nested_ptr) {
    BOOST_REQUIRE_EXCEPTION(
        (
            parameters_permission<allow_copies>().assert_policy<
                mpl::identity<std::vector<int*>>
            >()
        )
      , assert_exception
      , verify_assert_exception<std::vector<int*>>
    );
}

} // namespace policies
} // namespace di
} // namespace boost

