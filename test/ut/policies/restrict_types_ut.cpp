//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/policies/restrict_types.hpp"

#include <boost/test/unit_test.hpp>

#include "boost/di/scopes/unique.hpp"
#include "common/fakes/fake_data.hpp"

namespace boost {
namespace di {
namespace policies {

BOOST_AUTO_TEST_CASE(dummy) {
    restrict_types<allow_copies, allow_ptrs>().assert_policy<fake_data<int*, type_list<>, scopes::unique<>>>();
}

} // namespace policies
} // namespace di
} // namespace boost

