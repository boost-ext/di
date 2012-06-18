//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di.hpp"

#include <boost/test/unit_test.hpp>

#include "data.hpp"

namespace boost {
namespace di {

BOOST_AUTO_TEST_CASE(policy_fail) {
    injector<>().create<const c1&>();
}

} // namespace di
} // namespace boost

