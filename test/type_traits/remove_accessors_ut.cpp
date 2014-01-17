//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/remove_accessors.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/type_traits/is_same.hpp>

#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace type_traits {

BOOST_AUTO_TEST_CASE(basic) {
    BOOST_CHECK((is_same<int, remove_accessors<int>::type>::value));
    BOOST_CHECK((is_same<named<int>, remove_accessors<named<int>>::type>::value));
    BOOST_CHECK((is_same<int, remove_accessors<int&>::type>::value));
    BOOST_CHECK((is_same<int, remove_accessors<int*>::type>::value));
    BOOST_CHECK((is_same<int, remove_accessors<const int*>::type>::value));
}

} // namespace type_traits
} // namespace di
} // namespace boost

