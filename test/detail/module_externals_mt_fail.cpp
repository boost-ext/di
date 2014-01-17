//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/detail/module.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

#include "boost/di/concepts/dependency.hpp"
#include "boost/di/scopes/external.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace detail {

BOOST_AUTO_TEST_CASE(injector_externals_fail) {
    concepts::dependency<scopes::external<>, int> i_(42);

    module<
        mpl::vector<
            concepts::dependency<scopes::external<>, int>
          , concepts::dependency<scopes::external<>, double>
        >
    > module_(i_); // missing double external

    module_.create<c14>();
}

} // namespace detail
} // namespace di
} // namespace boost

