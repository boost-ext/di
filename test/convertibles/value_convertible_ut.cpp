//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/convertibles/value.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/type.hpp>

#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace convertibles {

const int i = 42;

BOOST_AUTO_TEST_CASE(to_value) {
    int object = (value<int>(42))(type<int>());
    BOOST_CHECK_EQUAL(i, object);
}

BOOST_AUTO_TEST_CASE(to_named_value) {
    named<int> object = (value<int>(42))(type<int>());
    BOOST_CHECK_EQUAL(i, object);
}

BOOST_AUTO_TEST_CASE(to_move) {
    int&& object = (value<int>(42))(type<int>());
    BOOST_CHECK_EQUAL(i, object);
}

} // namespace convertibles
} // namespace di
} // namespace boost

