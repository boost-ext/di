//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/per_request.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include <cstdlib>

#include "fake_allocator.hpp"
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
        static_cast<shared_ptr<int> >(
            per_request_.create())
        !=
        static_cast<shared_ptr<int> >(
            per_request_.create())
    ));
}

BOOST_AUTO_TEST_CASE(create_args) {
    per_request<>::scope<c2> per_request_;

    BOOST_CHECK((
        static_cast<shared_ptr<c2> >(
            per_request_.create<int, double, char>(0, 0.0, '0'))
        !=
        static_cast<shared_ptr<c2> >(
            per_request_.create<int, double, char>(0, 0.0, '0'))
    ));
}

BOOST_AUTO_TEST_CASE(create_value_mpl_int) {
    const int i = 42;
    BOOST_CHECK_EQUAL(0, (per_request<>::scope<int, mpl::int_<0> >().create()));
    BOOST_CHECK_EQUAL(i, (per_request<>::scope<int, mpl::int_<i> >().create()));
}

BOOST_AUTO_TEST_CASE(create_value_mpl_string) {
    BOOST_CHECK_EQUAL("s", static_cast<std::string>(
        (per_request<>::scope<std::string, mpl::string<'s'> >().create())));
}

BOOST_AUTO_TEST_CASE(create_value_has_value_type) {
    const double d = 42.0;
    double_value::value = d;
    BOOST_CHECK_EQUAL(d, (per_request<>::scope<double, double_value>().create()));
}

} // namespace scopes
} // namespace di
} // namespace boost

