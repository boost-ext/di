//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/type_traits/is_required_name.hpp"

#include <boost/test/unit_test.hpp>

#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace concepts {
namespace type_traits {

struct simple_name { };

BOOST_AUTO_TEST_CASE(no_name) {
    BOOST_CHECK((!is_required_name<void>::apply<int, void, void>::type::value));
    BOOST_CHECK((!is_required_name<int>::apply<int, void, void>::type::value));
}

BOOST_AUTO_TEST_CASE(name_not_match) {
    BOOST_CHECK((!is_required_name<simple_name>::apply<named<int>, void, void>::type::value));
}

BOOST_AUTO_TEST_CASE(name_match) {
    BOOST_CHECK((is_required_name<simple_name>::apply<named<int, simple_name>, void, void>::type::value));
    BOOST_CHECK((is_required_name<simple_name>::apply<named<char, simple_name>, void, void>::type::value));
    BOOST_CHECK((is_required_name<simple_name>::apply<const named<int, simple_name>&, void, void>::type::value));
    BOOST_CHECK((is_required_name<simple_name>::apply<named<int, simple_name>*, void, void>::type::value));
    BOOST_CHECK((is_required_name<simple_name>::apply<named<int, simple_name>*, void, void>::type::value));
}

} // namespace type_traits
} // namespace concepts
} // namespace di
} // namespace boost

