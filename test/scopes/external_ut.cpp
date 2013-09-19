//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/external.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

#include "boost/di/convertibles/convertible_ref.hpp"
#include "boost/di/convertibles/convertible_shared.hpp"

#include "data.hpp"

namespace boost {
namespace di {
namespace scopes {

BOOST_AUTO_TEST_CASE(from_arithmetic) {
    const int i = 42;
    BOOST_CHECK_EQUAL(i, static_cast<int>(external<>::scope<int>(i).create()));
}

BOOST_AUTO_TEST_CASE(from_string) {
    const std::string s = "string";
    BOOST_CHECK_EQUAL(s, static_cast<const std::string&>(external<>::scope<std::string>(s).create()));
}

BOOST_AUTO_TEST_CASE(from_ref) {
    c c_;
    c& c_ref_ = external<convertibles::convertible_ref>::scope<c>(c_).create();
    BOOST_CHECK_EQUAL(&c_, &c_ref_);
}

BOOST_AUTO_TEST_CASE(from_const_ref) {
    c c_;
    const c& const_c_ref_ = external<convertibles::convertible_ref_const>::scope<c>(c_).create();
    BOOST_CHECK_EQUAL(&c_, &const_c_ref_);
}

BOOST_AUTO_TEST_CASE(from_shared_ptr) {
    shared_ptr<c> c_(new c);
    shared_ptr<c> sp_c = external<convertibles::convertible_shared>::scope<c>(c_).create();
    BOOST_CHECK_EQUAL(c_, sp_c);
}

BOOST_AUTO_TEST_CASE(from_context) {
    shared_ptr<c> c1_(new c);
    shared_ptr<c> c2_(new c);

    BOOST_CHECK((
        static_cast<int>(external<>::scope<int, a>(87).create())
        !=
        static_cast<int>(external<>::scope<int, b>(42).create())
    ));

    BOOST_CHECK((
        static_cast<shared_ptr<c>>(external<convertibles::convertible_shared>::scope<c, a>(c1_).create())
        !=
        static_cast<shared_ptr<c>>(external<convertibles::convertible_shared>::scope<c, b>(c2_).create())
    ));
}

BOOST_AUTO_TEST_CASE(from_if_shared_ptr) {
    shared_ptr<c0if0> c0_(new c0if0);
    shared_ptr<if0> c1_ = external<convertibles::convertible_shared>::scope<if0>(c0_).create();
    BOOST_CHECK_EQUAL(c0_, c1_);
}

} // namespace scopes
} // namespace di
} // namespace boost

