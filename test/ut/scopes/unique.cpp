//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/cpp_0x/scopes/unique.hpp"

#include <string>
#include <boost/test/unit_test.hpp>

#include "boost/di/cpp_0x/aux_/memory.hpp"

namespace boost {
namespace di {
namespace scopes {

struct double_value
{
    static double value;
};
double double_value::value = 0.0;

auto new_int = []{ return new int(); };

BOOST_AUTO_TEST_CASE(create) {
    unique<>::scope<int> unique_;

    BOOST_CHECK((
        (unique_.create(new_int))(type<aux::shared_ptr<int>>())
        !=
        (unique_.create(new_int))(type<aux::shared_ptr<int>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_value_mpl_int) {
    const int i = 42;
    auto i_ = unique<>::scope<int>().create([&]{return new int(i); });
    BOOST_CHECK_EQUAL(i, i_(type<int>()));
}

BOOST_AUTO_TEST_CASE(create_value_mpl_string) {
    const std::string s = "s";
    auto s_ = unique<>::scope<std::string>().create([&]{return new std::string(s);});
    BOOST_CHECK_EQUAL(s, s_(type<std::string>()));
}

BOOST_AUTO_TEST_CASE(create_value_has_value_type) {
    const double d = 42.0;
    double_value::value = d;
    auto d_ = unique<>::scope<double>().create([&]{return new double(double_value::value);});
    BOOST_CHECK_EQUAL(double_value::value, d_(type<double>()));
}

} // namespace scopes
} // namespace di
} // namespace boost

