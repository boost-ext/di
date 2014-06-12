//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/type_traits/is_required_type.hpp"

#include <boost/test/unit_test.hpp>

#include <boost/mpl/vector.hpp>

namespace boost {
namespace di {
namespace concepts {
namespace type_traits {

struct i1 { };
struct i2 { };
struct impl : i1, i2 { };

BOOST_AUTO_TEST_CASE(types) {
    BOOST_CHECK((!is_required_type<int>::apply<void, void, void>::type::value));
    BOOST_CHECK((!is_required_type<int>::apply<const double&, void, void>::type::value));

    BOOST_CHECK((is_required_type<int>::apply<int, void, void>::type::value));
    BOOST_CHECK((is_required_type<int>::apply<const int&, void, void>::type::value));
    BOOST_CHECK((is_required_type<int>::apply<int*, void, void>::type::value));
    BOOST_CHECK((is_required_type<impl>::apply<i1, void, void>::type::value));
    BOOST_CHECK((is_required_type<impl>::apply<const i1&, void, void>::type::value));
    BOOST_CHECK((is_required_type<impl>::apply<i1*, void, void>::type::value));
}

BOOST_AUTO_TEST_CASE(sequence_type) {
    BOOST_CHECK((!is_required_type<mpl::vector<int, double> >::apply<void, void, void>::type::value));

    BOOST_CHECK((is_required_type<mpl::vector<int, double> >::apply<int, void, void>::type::value));
    BOOST_CHECK((is_required_type<mpl::vector<int, double> >::apply<double, void, void>::type::value));
    BOOST_CHECK((is_required_type<mpl::vector<i1, i2> >::apply<impl, void, void>::type::value));
    BOOST_CHECK((is_required_type<mpl::vector<i1> >::apply<impl, void, void>::type::value));
    BOOST_CHECK((is_required_type<mpl::vector<int, i2> >::apply<impl, void, void>::type::value));
}

} // namespace type_traits
} // namespace concepts
} // namespace di
} // namespace boost

