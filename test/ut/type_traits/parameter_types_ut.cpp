//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/parameter_types.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>

namespace boost {
namespace di {
namespace type_traits {

void f1() { }
int f2(int) { return {}; }
int f3(int, const double&) { return {}; }

struct c1
{
    void f1() { }
    int f2(int) { return {}; }
    int f3(int, const double&) { return {}; }
};

struct c2
{
    void f1() const { }
    int f2(int) const { return {}; }
    int f3(int, const double&) const { return {}; }
};

BOOST_AUTO_TEST_CASE(parameters_type_functions) {
    BOOST_CHECK((is_same<void, typename parameter_types<decltype(&f1)>::result_type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector<>, typename parameter_types<decltype(&f1)>::type>::value));

    BOOST_CHECK((is_same<int, typename parameter_types<decltype(&f2)>::result_type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector<int>, typename parameter_types<decltype(&f2)>::type>::value));

    BOOST_CHECK((is_same<int, typename parameter_types<decltype(&f3)>::result_type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector<int, const double&>, typename parameter_types<decltype(&f3)>::type>::value));
}

BOOST_AUTO_TEST_CASE(parameters_type_methods) {
    BOOST_CHECK((is_same<void, typename parameter_types<decltype(&c1::f1)>::result_type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector<>, typename parameter_types<decltype(&c1::f1)>::type>::value));

    BOOST_CHECK((is_same<int, typename parameter_types<decltype(&c1::f2)>::result_type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector<int>, typename parameter_types<decltype(&c1::f2)>::type>::value));

    BOOST_CHECK((is_same<int, typename parameter_types<decltype(&c1::f3)>::result_type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector<int, const double&>, typename parameter_types<decltype(&c1::f3)>::type>::value));
}

BOOST_AUTO_TEST_CASE(parameters_type_const_methods) {
    BOOST_CHECK((is_same<void, typename parameter_types<decltype(&c2::f1)>::result_type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector<>, typename parameter_types<decltype(&c2::f1)>::type>::value));

    BOOST_CHECK((is_same<int, typename parameter_types<decltype(&c2::f2)>::result_type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector<int>, typename parameter_types<decltype(&c2::f2)>::type>::value));

    BOOST_CHECK((is_same<int, typename parameter_types<decltype(&c2::f3)>::result_type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector<int, const double&>, typename parameter_types<decltype(&c2::f3)>::type>::value));
}

} // namespace type_traits
} // namespace di
} // namespace boost

