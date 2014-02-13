//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/detail/module.hpp"

#include <boost/test/unit_test.hpp>

#include "boost/di/policies/creation_ownership.hpp"
#include "common/data.hpp"

namespace boost {
namespace di {
namespace detail {

BOOST_AUTO_TEST_CASE(module_policy_fail) {
    module<
        mpl::vector<policies::creation_ownership>
    >().create<const c1&>();
}

} // namespace detail
} // namespace di
} // namespace boost

