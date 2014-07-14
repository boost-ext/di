//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/has_call_operator.hpp"

#include <boost/test/unit_test.hpp>

#include <boost/config.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace boost {
namespace di {
namespace type_traits {

struct empty { };

struct call_operator_void_const
{
    void operator()() const { }
};

struct call_operator_int_arg
{
    int operator()(int i) { return i; }
};

struct call_operator_derived
    : call_operator_void_const
{ };

BOOST_AUTO_TEST_CASE(none) {
    BOOST_CHECK(!has_call_operator<int>::value);
    BOOST_CHECK(!has_call_operator<void>::value);
    BOOST_CHECK(!has_call_operator<empty>::value);
}

BOOST_AUTO_TEST_CASE(class_with_call_operator) {
    BOOST_CHECK(has_call_operator<call_operator_void_const>::value);
    BOOST_CHECK(has_call_operator<call_operator_int_arg>::value);
}

BOOST_AUTO_TEST_CASE(class_with_derived_call_operator) {
    BOOST_CHECK(has_call_operator<call_operator_derived>::value);
}

BOOST_AUTO_TEST_CASE(function) {
    BOOST_CHECK(has_call_operator<boost::function<int()> >::value);
}

BOOST_AUTO_TEST_CASE(bind_with_class) {
    typedef BOOST_TYPEOF(boost::bind<void>(call_operator_void_const())) bind_t;
    BOOST_CHECK(has_call_operator<bind_t>::value);
}

int return_int(int i) { return i; }

BOOST_AUTO_TEST_CASE(bind_with_function) {
    typedef BOOST_TYPEOF(boost::bind(&return_int, 0)) bind_t;
    BOOST_CHECK(has_call_operator<bind_t>::value);
}

#if !defined(BOOST_NO_CXX11_LAMBDAS)
    template<typename T>
    void test(const T&) {
        BOOST_CHECK(has_call_operator<T>::value);
    }

    BOOST_AUTO_TEST_CASE(lambda) {
        test([]{});
        test([]{ return 42; });
        test([]() -> int { return 42; });
    }
#endif

} // namespace type_traits
} // namespace di
} // namespace boost

