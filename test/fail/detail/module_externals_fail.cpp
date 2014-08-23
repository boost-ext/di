//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/core/module.hpp"

#include <boost/test/unit_test.hpp>

#include "boost/di/bindings/dependency.hpp"
#include "boost/di/scopes/external.hpp"
#include "common/data.hpp"

namespace boost {
namespace di {
namespace core {

BOOST_AUTO_TEST_CASE(module_externals_fail) {
    bindings::dependency<scopes::external<>, int> i_(42);

    module<
        type_list<
            bindings::dependency<scopes::external, int>
          , bindings::dependency<scopes::external, double>
        >
    > module_(i_); // missing double external

    module_.create<c14>();
}

} // namespace core
} // namespace di
} // namespace boost

