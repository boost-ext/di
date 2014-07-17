//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/bindings/type_traits/is_required_type.hpp"

#include <boost/test/unit_test.hpp>

#include <boost/mpl/vector.hpp>

namespace boost {
namespace di {
namespace bindings {
namespace type_traits {

struct i1 { };
struct i2 { };
struct impl : i1, i2 { };

template<typename T>
struct fake_data {
    typedef T type;
};

BOOST_AUTO_TEST_CASE(types) {
    BOOST_CHECK((!is_required_type<int>::apply<fake_data<void>>::type::value));
    BOOST_CHECK((!is_required_type<int>::apply<fake_data<const double&>>::type::value));

    BOOST_CHECK((is_required_type<int>::apply<fake_data<int>>::type::value));
    BOOST_CHECK((is_required_type<int>::apply<fake_data<const int&>>::type::value));
    BOOST_CHECK((is_required_type<int>::apply<fake_data<int*>>::type::value));
    BOOST_CHECK((is_required_type<impl>::apply<fake_data<i1>>::type::value));
    BOOST_CHECK((is_required_type<impl>::apply<fake_data<const i1&>>::type::value));
    BOOST_CHECK((is_required_type<impl>::apply<fake_data<i1*>>::type::value));
}

BOOST_AUTO_TEST_CASE(sequence_type) {
    BOOST_CHECK((!is_required_type<mpl::vector<int, double> >::apply<fake_data<void>>::type::value));

    BOOST_CHECK((is_required_type<mpl::vector<int, double> >::apply<fake_data<int>>::type::value));
    BOOST_CHECK((is_required_type<mpl::vector<int, double> >::apply<fake_data<double>>::type::value));
    BOOST_CHECK((is_required_type<mpl::vector<i1, i2> >::apply<fake_data<impl>>::type::value));
    BOOST_CHECK((is_required_type<mpl::vector<i1> >::apply<fake_data<impl>>::type::value));
    BOOST_CHECK((is_required_type<mpl::vector<int, i2> >::apply<fake_data<impl>>::type::value));
}

} // namespace type_traits
} // namespace bindings
} // namespace di
} // namespace boost

