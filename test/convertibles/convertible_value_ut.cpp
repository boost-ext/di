//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/convertibles/convertible_value.hpp"

#include <boost/test/unit_test.hpp>

#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace convertibles {

BOOST_AUTO_TEST_CASE(to_value) {
    const int i = 42;
    int object = convertible_value<int>(42);
    BOOST_CHECK_EQUAL(i, object);
}

BOOST_AUTO_TEST_CASE(to_named_value) {
    const int i = 42;
    named<int> object = convertible_value<int>(42);
    BOOST_CHECK_EQUAL(i, object);
}

} // namespace convertibles
} // namespace di
} // namespace boost

