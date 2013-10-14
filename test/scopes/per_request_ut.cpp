//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/per_request.hpp"

#include <string>
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>

#include "boost/di/memory.hpp"
#include "fake_convertible.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace scopes {

struct double_value
{
    static double value;
};
double double_value::value = 42.0;

BOOST_AUTO_TEST_CASE(create) {
    per_request<>::scope<int> per_request_;

    BOOST_CHECK((
        (per_request_.create())(type<shared_ptr<int>>())
        !=
        (per_request_.create())(type<shared_ptr<int>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_args) {
    per_request<>::scope<c2> per_request_;

    fake_convertible<int> i(0);
    fake_convertible<double> d(0.0);
    fake_convertible<char> c('0');

    BOOST_CHECK((
        (per_request_.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<shared_ptr<c2>>())
        !=
        (per_request_.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<shared_ptr<c2>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_value_mpl_int) {
    const int i = 42;
    BOOST_CHECK_EQUAL(0, (per_request<>::scope<int, mpl::int_<0>>().create())(type<int>()));
    BOOST_CHECK_EQUAL(i, (per_request<>::scope<int, mpl::int_<i>>().create())(type<int>()));
}

BOOST_AUTO_TEST_CASE(create_value_mpl_string) {
    BOOST_CHECK_EQUAL("s", (per_request<>::scope<std::string, mpl::string<'s'>>().create())(type<std::string>()));
}

BOOST_AUTO_TEST_CASE(create_value_has_value_type) {
    const double d = 42.0;
    double_value::value = d;
    BOOST_CHECK_EQUAL(d, (per_request<>::scope<double, double_value>().create())(type<double>()));
}

} // namespace scopes
} // namespace di
} // namespace boost

