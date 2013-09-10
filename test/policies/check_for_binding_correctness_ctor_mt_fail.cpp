//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/policies/check_for_binding_correctness.hpp"

#include <boost/test/unit_test.hpp>
#include "boost/di.hpp"

#include "data.hpp"

namespace boost {
namespace di {
namespace policies {

BOOST_AUTO_TEST_CASE(check_for_binding_correctness_fail) {
    injector<
        policy<check_for_binding_correctness>
      , module<
            bind_int<int, int> //arithmetic type without value
        >
    > injector;

    injector.create<c3>();
}

} // namespace policies
} // namespace di
} // namespace boost

