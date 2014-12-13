//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/cpp_0x/scopes/external.hpp"

#include <functional>
#include <boost/test/unit_test.hpp>
#include <boost/ref.hpp>
#include <boost/type.hpp>

#include "boost/di/cpp_0x/aux_/memory.hpp"
#include "boost/di/cpp_0x/wrappers/reference.hpp"
#include "boost/di/cpp_0x/wrappers/shared.hpp"

#include "common/data.hpp"

namespace boost {
namespace di {
namespace scopes {

BOOST_AUTO_TEST_CASE(is_reference_wrapper_unwrap_reference) {
    BOOST_CHECK((is_reference_wrapper<std::reference_wrapper<int> >::value));
    BOOST_CHECK((is_same<unwrap_reference<std::reference_wrapper<int> >::type, int>::value));
    BOOST_CHECK((is_reference_wrapper<boost::reference_wrapper<int> >::value));
    BOOST_CHECK((is_same<unwrap_reference<boost::reference_wrapper<int> >::type, int>::value));
}

BOOST_AUTO_TEST_CASE(from_arithmetic) {
    const int i = 42;
    BOOST_CHECK_EQUAL(i, (external<>::scope<int>(i).create())(type<int>()));
}

BOOST_AUTO_TEST_CASE(from_string) {
    const std::string s = "string";
    BOOST_CHECK_EQUAL(s, (external<>::scope<std::string>(s).create())(type<std::string>()));
}

BOOST_AUTO_TEST_CASE(from_ref) {
    c c_;
    c& c_ref_ = (external<wrappers::reference>::scope<c>(boost::ref(c_)).create())(type<c&>());
    BOOST_CHECK_EQUAL(&c_, &c_ref_);
}

BOOST_AUTO_TEST_CASE(from_const_ref) {
    c c_;
    const c& const_c_ref_ = (external<wrappers::reference>::scope<const c>(boost::cref(c_)).create())(type<const c&>());
    BOOST_CHECK_EQUAL(&c_, &const_c_ref_);
}

BOOST_AUTO_TEST_CASE(from_shared_ptr) {
    std::shared_ptr<c> c_(new c);
    std::shared_ptr<c> sp_c =
        (external<wrappers::shared>::scope<c>(c_).create())(type<std::shared_ptr<c>>());

    BOOST_CHECK_EQUAL(c_, sp_c);
}

BOOST_AUTO_TEST_CASE(from_context) {
    std::shared_ptr<c> c1_(new c);
    std::shared_ptr<c> c2_(new c);

    BOOST_CHECK((
        (external<>::scope<int>(87).create())(type<int>())
        !=
        (external<>::scope<int>(42).create())(type<int>())
    ));

    BOOST_CHECK((
        (external<wrappers::shared>::scope<c>(c1_).create())(type<std::shared_ptr<c>>())
        !=
        (external<wrappers::shared>::scope<c>(c2_).create())(type<std::shared_ptr<c>>())
    ));
}

BOOST_AUTO_TEST_CASE(from_if_shared_ptr) {
    std::shared_ptr<c0if0> c0_(new c0if0);
    std::shared_ptr<if0> c1_ = external<wrappers::shared>::scope<if0>(c0_).create()(type<std::shared_ptr<if0>>());
    BOOST_CHECK_EQUAL(c0_, c1_);
}

BOOST_AUTO_TEST_CASE(from_function_expr) {
    bool flag = false;
    external<wrappers::shared>::scope<if0> external_(
        [&flag]() -> std::shared_ptr<if0> {
            if (!flag) {
                return std::shared_ptr<c0if0>(new c0if0);
            }

            return std::shared_ptr<c1if0>(new c1if0);
        }
    );

    BOOST_CHECK(dynamic_cast<c0if0*>(external_.create()(type<std::shared_ptr<if0>>()).get()));

    flag = true;
    BOOST_CHECK(dynamic_cast<c1if0*>(external_.create()(type<std::shared_ptr<if0>>()).get()));
}

BOOST_AUTO_TEST_CASE(from_function_expr_with_expected_function_expr) {
    const int i = 42;
    external<wrappers::value>::scope<std::function<int()>> external_([&]{ return i; });
    BOOST_CHECK_EQUAL(i, external_.create()(type<std::function<int()>>())());
}

} // namespace scopes
} // namespace di
} // namespace boost

