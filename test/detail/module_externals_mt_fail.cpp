//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/detail/module.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

#include "boost/di/policies/check_for_binding_correctness.hpp"
#include "boost/di/policies/check_for_circular_dependencies.hpp"
#include "boost/di/policies/check_for_creation_ownership.hpp"
#include "boost/di/scopes/external.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace detail {

BOOST_AUTO_TEST_CASE(module_externals_fail) {
    dependency<scopes::external, int> i_(42);

    module<
        mpl::vector<
            dependency<scopes::external, int>
          , dependency<scopes::external, double>
        >
    > module_(i_); // missing double external

    module_.create<c14>();
}

} // namespace detail
} // namespace di
} // namespace boost

