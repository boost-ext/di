//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/convertibles/convertible.hpp"

#include <boost/test/unit_test.hpp>

#include "boost/di/named.hpp"
#include "common/fakes/fake_convertible.hpp"

namespace boost {
namespace di {
namespace convertibles {

BOOST_AUTO_TEST_CASE(convert_to_basic_type) {
    const int i = 42;
    convertible<int> c((fake_convertible<int>(i)));
    BOOST_CHECK_EQUAL(i, static_cast<int>(c));
}

BOOST_AUTO_TEST_CASE(convert_to_named_type) {
    const int i = 42;
    convertible<named<int>> c((fake_convertible<int>(i)));
    BOOST_CHECK_EQUAL(i, c.operator int());
    BOOST_CHECK_EQUAL(i, c.operator named<int>());
}

} // namespace convertibles
} // namespace di
} // namespace boost

