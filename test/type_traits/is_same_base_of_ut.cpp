//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/is_same_base_of.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {
namespace type_traits {

struct i { };
struct c : i { };
struct a { };

BOOST_AUTO_TEST_CASE(basic) {
    BOOST_CHECK((is_same_base_of<int, int>::value));
    BOOST_CHECK((is_same_base_of<c, c>::value));
    BOOST_CHECK((is_same_base_of<c, i>::value));
    BOOST_CHECK((!is_same_base_of<c, a>::value));
}

} // namespace type_traits
} // namespace di
} // namespace boost

