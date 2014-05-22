//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/unique.hpp"

#include <string>
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>

#include "boost/di/aux_/memory.hpp"
#include "common/fakes/fake_wrapper.hpp"
#include "common/fakes/fake_create_policy.hpp"
#include "common/data.hpp"

namespace boost {
namespace di {
namespace scopes {

struct double_value
{
    static double value;
};
double double_value::value = 42.0;

BOOST_AUTO_TEST_CASE(create) {
    unique<>::scope<int> unique_;

    BOOST_CHECK((
        (unique_.create<fake_create_policy>())(type<aux::shared_ptr<int>>())
        !=
        (unique_.create<fake_create_policy>())(type<aux::shared_ptr<int>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_args) {
    unique<>::scope<c2> unique_;

    fake_wrapper<int> i(0);
    fake_wrapper<double> d(0.0);
    fake_wrapper<char> c('0');

    BOOST_CHECK((
        (unique_.create<fake_create_policy, decltype(i), decltype(d), decltype(c)>(i, d, c))(type<aux::shared_ptr<c2>>())
        !=
        (unique_.create<fake_create_policy, decltype(i), decltype(d), decltype(c)>(i, d, c))(type<aux::shared_ptr<c2>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_value_mpl_int) {
    const int i = 42;
    auto i_ = unique<>::scope<int, mpl::int_<i>>().create<fake_create_policy>();
    BOOST_CHECK_EQUAL(i, i_(type<int>()));
}

BOOST_AUTO_TEST_CASE(create_value_mpl_string) {
    auto s_ = unique<>::scope<std::string, mpl::string<'s'>>().create<fake_create_policy>();
    BOOST_CHECK_EQUAL("s", s_(type<std::string>()));
}

BOOST_AUTO_TEST_CASE(create_value_has_value_type) {
    const double d = 42.0;
    double_value::value = d;
    auto d_ = unique<>::scope<double, double_value>().create<fake_create_policy>();
    BOOST_CHECK_EQUAL(d, d_(type<double>()));
}

} // namespace scopes
} // namespace di
} // namespace boost

