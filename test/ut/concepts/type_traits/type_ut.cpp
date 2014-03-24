//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/type_traits/type.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {
namespace concepts {
namespace type_traits {

struct i { };
struct impl : i { };

BOOST_AUTO_TEST_CASE(not_match) {
    BOOST_CHECK((!type<int>::apply<void, void, void>::type::value));
    BOOST_CHECK((!type<int>::apply<const double&, void, void>::type::value));
}

BOOST_AUTO_TEST_CASE(match) {
    BOOST_CHECK((type<int>::apply<int, void, void>::type::value));
    BOOST_CHECK((type<int>::apply<const int&, void, void>::type::value));
    BOOST_CHECK((type<int>::apply<int*, void, void>::type::value));
    BOOST_CHECK((type<impl>::apply<i, void, void>::type::value));
    BOOST_CHECK((type<impl>::apply<const i&, void, void>::type::value));
    BOOST_CHECK((type<impl>::apply<i*, void, void>::type::value));
}

} // namespace type_traits
} // namespace concepts
} // namespace di
} // namespace boost

