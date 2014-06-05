//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/is_convertible_to_ref.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {
namespace type_traits {

struct empty { };

struct convertible_to_int
{
    int operator()() { return 0; }
};

struct convertible_to_int_const
{
    int operator()() const { return 0; }
};

struct convertible_to_int_ref
{
    int& operator()(const boost::type<int&>&) const { static int i = 0; return i; }
};

struct convertible_to_const_int_ref
{
    const int& operator()(const boost::type<const int&>&) const { static int i = 0; return i; }
};

BOOST_AUTO_TEST_CASE(convertbiles) {
    BOOST_CHECK((detail::is_convertible<convertible_to_int, int(convertible_to_int::*)()>::value));
    BOOST_CHECK((detail::is_convertible<convertible_to_int_const, int(convertible_to_int_const::*)() const>::value));

    BOOST_CHECK((!detail::is_convertible<int, int(empty::*)()>::value));
    BOOST_CHECK((!detail::is_convertible<convertible_to_int, int(convertible_to_int::*)() const>::value));
    BOOST_CHECK((!detail::is_convertible<convertible_to_int, double(convertible_to_int::*)() const>::value));
    BOOST_CHECK((!detail::is_convertible<convertible_to_int_const, int(convertible_to_int_const::*)()>::value));
    BOOST_CHECK((!detail::is_convertible<convertible_to_int_const, double(convertible_to_int_const::*)() const>::value));
}

BOOST_AUTO_TEST_CASE(convertbiles_to_ref) {
    BOOST_CHECK((is_convertible_to_ref<convertible_to_int_ref, int>::value));
    BOOST_CHECK((is_convertible_to_ref<convertible_to_const_int_ref, int>::value));

    BOOST_CHECK((!is_convertible_to_ref<empty, int>::value));
    BOOST_CHECK((!is_convertible_to_ref<convertible_to_int_const, int>::value));
    BOOST_CHECK((!is_convertible_to_ref<convertible_to_int, int>::value));
}

} // namespace type_traits
} // namespace di
} // namespace boost

