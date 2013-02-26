//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/external.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/function.hpp>

#include "boost/di/named.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace scopes {

template<
    typename T
  , typename TName = void
>
struct named
{
    typedef T value_type;

    explicit named(T i)
        : i(i)
    { }

    T i;
};

BOOST_AUTO_TEST_CASE(arithmetic_value) {
    const int i = 42;
    BOOST_CHECK_EQUAL(i, static_cast<int>(convertible_any<int>(i)));
}

BOOST_AUTO_TEST_CASE(arithmetic_with_value_type) {
    const int i = 42;
    BOOST_CHECK_EQUAL(i, static_cast<int>(convertible_any<named<int> >(i)));
}

BOOST_AUTO_TEST_CASE(string_value) {
    const std::string s = "string";
    BOOST_CHECK_EQUAL(s, static_cast<const std::string&>(convertible_any<std::string>(s)));
}

BOOST_AUTO_TEST_CASE(string_with_value_type) {
    const std::string s = "string";
    BOOST_CHECK_EQUAL(s, static_cast<const std::string&>(convertible_any<named<std::string> >(s)));
}

BOOST_AUTO_TEST_CASE(variant_ref) {
    c c_;
    c& c_ref_ = convertible_any<c>(c_);
    BOOST_CHECK_EQUAL(&c_, &c_ref_);
}

BOOST_AUTO_TEST_CASE(variant_const_ref) {
    c c_;
    const c& const_c_ref_ = convertible_any<c>(c_);
    BOOST_CHECK_EQUAL(&c_, &const_c_ref_);
}

BOOST_AUTO_TEST_CASE(variant_shared_ptr) {
    shared_ptr<c> c_(new c);
    shared_ptr<c> sp_c = convertible_any<c>(c_);
    BOOST_CHECK_EQUAL(c_, sp_c);
}

BOOST_AUTO_TEST_CASE(variant_function) {
    const int i = 42;
    typedef function<int()> f_type;
    struct c
    {
        static int f() { return i;}
    };
    f_type f_(&c::f);
    BOOST_CHECK_EQUAL(i, static_cast<const f_type&>(convertible_any<f_type>(f_))());
}

BOOST_AUTO_TEST_CASE(named_int) {
    const int i = 42;
    convertible_any<named<int> > variant_(i);

    BOOST_CHECK_EQUAL(i, static_cast<int>(variant_));
}

BOOST_AUTO_TEST_CASE(named_shared_ptr) {
    typedef named<shared_ptr<int>, a> c1_t;
    typedef named<shared_ptr<int>, b> c2_t;

    shared_ptr<c1_t> c1_(new c1_t(make_shared<int>(42)));
    shared_ptr<c2_t> c2_(new c2_t(make_shared<int>(87)));

    BOOST_CHECK((
        *static_cast<shared_ptr<c1_t> >(convertible_any<c1_t>(c1_))->i
        !=
        *static_cast<shared_ptr<c2_t> >(convertible_any<c2_t>(c2_))->i
    ));
}

BOOST_AUTO_TEST_CASE(context) {
    shared_ptr<c> c1_(new c);
    shared_ptr<c> c2_(new c);

    BOOST_CHECK((
        static_cast<int>(convertible_any<int, a>(87))
        !=
        static_cast<int>(convertible_any<int, b>(42))
    ));

    BOOST_CHECK((
        static_cast<shared_ptr<c> >(convertible_any<c, a>(c1_))
        !=
        static_cast<shared_ptr<c> >(convertible_any<c, b>(c2_))
    ));
}

BOOST_AUTO_TEST_CASE(if_shared_ptr) {
    shared_ptr<c0if0> c0_(new c0if0);
    shared_ptr<if0> c1_ = convertible_any<if0>(c0_);
    BOOST_CHECK_EQUAL(c0_, c1_);
}

} // namespace scopes
} // namespace di
} // namespace boost

