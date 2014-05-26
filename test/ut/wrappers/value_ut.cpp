//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/wrappers/value.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/type.hpp>

namespace boost {
namespace di {
namespace wrappers {

const int i = 42;

BOOST_AUTO_TEST_CASE(to_value) {
    int object((value<int>(42))(type<int>()));
    BOOST_CHECK_EQUAL(i, object);
}

BOOST_AUTO_TEST_CASE(to_rvalue) {
    int&& object((value<int>(42))(type<int>()));
    BOOST_CHECK_EQUAL(i, object);
}

BOOST_AUTO_TEST_CASE(to_shared_ptr) {
    aux::shared_ptr<int> object((value<int>(42))(type<aux::shared_ptr<int>>()));
    BOOST_CHECK_EQUAL(i, *object);
}

BOOST_AUTO_TEST_CASE(to_shared_ptr_other) {
    aux_::shared_ptr<int> object((value<int>(42))(type<aux_::shared_ptr<int>>()));
    BOOST_CHECK_EQUAL(i, *object);
}

} // namespace wrappers
} // namespace di
} // namespace boost

