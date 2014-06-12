//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/type_traits/is_required_priority.hpp"

#include <boost/test/unit_test.hpp>

#include "common/fakes/fake_scope.hpp"

namespace boost {
namespace di {
namespace concepts {
namespace type_traits {

template<typename TScope>
struct fake_data {
    typedef TScope scope;
};

BOOST_AUTO_TEST_CASE(scope_priority) {
    BOOST_CHECK_EQUAL(1, (is_required_priority::apply<fake_data<fake_scope<0>>>::type::value));
    BOOST_CHECK_EQUAL(2, (is_required_priority::apply<fake_data<fake_scope<1>>>::type::value));
}

} // namespace type_traits
} // namespace concepts
} // namespace di
} // namespace boost

