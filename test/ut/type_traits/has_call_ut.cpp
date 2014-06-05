//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/has_call.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {
namespace type_traits {

struct empty { };

struct call_int
{
    void call(const int&) { }
};

struct call_derived
    : call_int
{ };

struct call_template
{
    template<typename T>
    void call(const T&) { }
};

BOOST_AUTO_TEST_CASE(calls) {
    BOOST_CHECK((!has_call<empty, int>::value));
    BOOST_CHECK((!has_call<call_int, empty>::value));
    BOOST_CHECK((!has_call<call_derived, empty>::value));

    BOOST_CHECK((has_call<call_int, int>::value));
    BOOST_CHECK((has_call<call_derived, int>::value));
    BOOST_CHECK((has_call<call_template, int>::value));
    BOOST_CHECK((has_call<call_template, double>::value));

    BOOST_CHECK((has_call<call_int, double>::value)); // int is convertible to double
    BOOST_CHECK((has_call<call_derived, double>::value)); // int is convertible to double
}

} // namespace type_traits
} // namespace di
} // namespace boost

