//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "common/fakes/fake_assert.hpp" // has to be first
#include "boost/di/cpp_0x/policies/creation_permission.hpp"

#include "boost/di/cpp_0x/aux_/memory.hpp"
#include <boost/test/test_case_template.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/type_traits/is_pod.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/bool.hpp>

namespace boost {
namespace di {
namespace policies {

template<typename T>
bool verify_assert_exception(const assert_exception& e) {
    return e.what() == std::string("TYPE_HAS_TO_BE_EXPLICITLY_BINDED") &&
           e.get_type() == typeid(T);
}

template<typename T, typename TResolve = mpl::false_>
struct fake_dependency
{
    typedef T type;

    struct resolved
    {
        typedef resolved type;

        template<typename>
        struct rebind
        {
            typedef resolved other;
        };
    };

    struct binder {
        template<
            typename
          , typename
          , typename TDefault
        >
        struct resolve
            : mpl::if_<TResolve, resolved, TDefault>::type
        { };
    };
};

BOOST_AUTO_TEST_CASE(none) {
    BOOST_REQUIRE_EXCEPTION(
        (
            creation_permission<>().assert_policy<
                fake_dependency<int>
            >()
        )
      , assert_exception
      , verify_assert_exception<int>
    );
}

BOOST_AUTO_TEST_CASE(resolve) {
    BOOST_CHECK_NO_THROW(
        (
            creation_permission<>().assert_policy<
                fake_dependency<int, mpl::true_>
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_type_not_resolve) {
    BOOST_CHECK_NO_THROW(
        (
            creation_permission<allow_type<int>>().assert_policy<
                fake_dependency<int>
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_type_and_resolve) {
    BOOST_CHECK_NO_THROW(
        (
            creation_permission<allow_type<int>>().assert_policy<
                fake_dependency<int, mpl::true_>
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_type_expr_not_resolve) {
    BOOST_CHECK_NO_THROW(
        (
            creation_permission<allow_type_expr<is_pod<mpl::_>>>().assert_policy<
                fake_dependency<int>
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_type_expr_and_resolve) {
    BOOST_CHECK_NO_THROW(
        (
            creation_permission<allow_type_expr<is_pod<mpl::_>>>().assert_policy<
                fake_dependency<int, mpl::true_>
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_type_and_type_expr_not_resolve) {
    BOOST_CHECK_NO_THROW(
        (
            creation_permission<allow_type<int>, allow_type_expr<is_pod<mpl::_>>>().assert_policy<
                fake_dependency<int, mpl::false_>
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_type_and_type_expr_and_resolve) {
    BOOST_CHECK_NO_THROW(
        (
            creation_permission<allow_type<int>, allow_type_expr<is_pod<mpl::_>>>().assert_policy<
                fake_dependency<int, mpl::true_>
            >()
        )
    );
}

} // namespace policies
} // namespace di
} // namespace boost

