//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/type_traits/priority.hpp"

#include <boost/test/unit_test.hpp>

#include "common/fakes/fake_scope.hpp"

namespace boost {
namespace di {
namespace concepts {
namespace type_traits {

BOOST_AUTO_TEST_CASE(scope_priority) {
    BOOST_CHECK_EQUAL(1, (priority::apply<void, void, fake_scope<0>>::type::value));
    BOOST_CHECK_EQUAL(2, (priority::apply<void, void, fake_scope<1>>::type::value));
}

} // namespace type_traits
} // namespace concepts
} // namespace di
} // namespace boost

