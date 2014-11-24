//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "common/fakes/fake_assert.hpp" // has to be first
#include "boost/di/cpp_0x/policies/circular_dependencies.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/vector.hpp>

#include "common/data.hpp"

namespace boost {
namespace di {
namespace policies {

template<typename T>
struct fake_call_stack
{
    typedef T call_stack;
};

using no_circular_dependencies_types = mpl::vector<
    mpl::vector<>
  , mpl::vector<int>
  , mpl::vector<int, double>
  , mpl::vector<c1>
  , mpl::vector<c1, c2>
  , mpl::vector<c1, c2, c3>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(no_circular_dependency, T, no_circular_dependencies_types) {
    BOOST_CHECK_NO_THROW(
        (
            circular_dependencies().assert_policy<
                fake_call_stack<T>
            >()
        )
    );
}

using circular_dependencies_types = mpl::vector<
    mpl::vector<c1, c2, c1>
  , mpl::vector<c1, c2, c3, c2>
  , mpl::vector<c1, c1>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(circular_dependency, T, circular_dependencies_types) {
    BOOST_REQUIRE_EXCEPTION(
        (
            circular_dependencies().assert_policy<
                fake_call_stack<T>
            >()
        )
      , assert_exception
      , [](const assert_exception& e) {
            return e.what() == std::string("CIRCULAR_DEPENDENCIES_ARE_NOT_ALLOWED") &&
                   e.get_type() == typeid(T);
        }
    );
}

} // namespace policies
} // namespace di
} // namespace boost

