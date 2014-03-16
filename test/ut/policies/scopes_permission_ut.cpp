//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "common/fakes/fake_assert.hpp" // has to be first
#include "boost/di/policies/scopes_permission.hpp"
#include "boost/di/scopes/shared.hpp"
#include "boost/di/scopes/session.hpp"
#include "boost/di/scopes/unique.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

#include "common/data.hpp"

namespace boost {
namespace di {
namespace policies {

template<typename T>
bool verify_assert_exception(const assert_exception& e) {
    return e.get_msg() == "SCOPE_NOT_PERMITTED" &&
           e.get_type() == typeid(T);
}

template<typename TScope>
struct fake_scope
{
    typedef TScope scope;
};

BOOST_AUTO_TEST_CASE(value) {
    BOOST_REQUIRE_EXCEPTION(
        (
            scopes_permission<>::assert_policy<
                mpl::vector0<>
              , fake_scope<scopes::unique<>>
            >()
        )
      , assert_exception
      , verify_assert_exception<scopes::unique<>>
    );
}

BOOST_AUTO_TEST_CASE(allow_none) {
    BOOST_REQUIRE_EXCEPTION(
        (
            scopes_permission<
                allow_scope<scopes::unique<>>
              , allow_scope<scopes::shared<>>
            >::assert_policy<
                mpl::vector0<>
              , fake_scope<scopes::session<>>
            >()
        )
      , assert_exception
      , verify_assert_exception<scopes::session<>>
    );
}

BOOST_AUTO_TEST_CASE(allow_unique) {
    BOOST_CHECK_NO_THROW(
        (
            scopes_permission<allow_scope<scopes::unique<>>>::assert_policy<
                mpl::vector0<>
              , fake_scope<scopes::unique<>>
            >()
        )
    );
}

BOOST_AUTO_TEST_CASE(allow_shared) {
    BOOST_CHECK_NO_THROW(
        (
            scopes_permission<allow_scope<scopes::shared<>>>::assert_policy<
                mpl::vector0<>
              , fake_scope<scopes::shared<>>
            >()
        )
    );
}

} // namespace policies
} // namespace di
} // namespace boost

