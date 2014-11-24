//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/cpp_0x/detail/module.hpp"

#include <boost/test/unit_test.hpp>

#include "boost/di/cpp_0x/policies/creation_permission.hpp"
#include "common/data.hpp"

namespace boost {
namespace di {
namespace detail {

BOOST_AUTO_TEST_CASE(module_policy_fail) {
    module<>().create<const c1&>(policies::creation_permission());
}

} // namespace detail
} // namespace di
} // namespace boost

