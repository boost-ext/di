//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/cpp_0x/policies/parameters_permission.hpp"

#include <boost/test/unit_test.hpp>
#include "boost/di.hpp"

#include "common/data.hpp"

namespace boost {
namespace di {
namespace policies {

BOOST_AUTO_TEST_CASE(parameters_permission_fail) {
    injector<>().create<c24>(
        parameters_permission<allow_ptrs, allow_refs, allow_copies>() // allow_smart_ptrs
    );
}

} // namespace policies
} // namespace di
} // namespace boost

