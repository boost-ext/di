//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/cpp_0x/named.hpp"

#include <memory>
#include <boost/test/unit_test.hpp>
#include <boost/make_shared.hpp>
#include <boost/type_traits/is_same.hpp>

#include "common/data.hpp"

namespace boost {
namespace di {

BOOST_AUTO_TEST_CASE(named_int_value) {
    const int i = 42;
    typedef named<int, a> named_type;
    named_type named_(i);

    BOOST_CHECK_EQUAL(i, named_);
}

BOOST_AUTO_TEST_CASE(named_int_ref) {
    int i = 42;
    typedef named<int&, a> named_type;
    named_type named_(i);

    int& ref = named_;
    ref++;

    BOOST_CHECK_EQUAL(i, static_cast<int&>(named_));
}

BOOST_AUTO_TEST_CASE(named_const_ref) {
    const int i = 42;
    typedef named<const int&, a> named_type;
    named_type named_(i);

    const int& ref = static_cast<const int&>(named_);
    BOOST_CHECK_EQUAL(i, ref);
}

BOOST_AUTO_TEST_CASE(named_ptr) {
    const int i = 42;
    aux::unique_ptr<int> ptr(new int(i));
    named<int*> named_(ptr.get());

    BOOST_CHECK_EQUAL(ptr.get(), static_cast<int*>(named_));
}

BOOST_AUTO_TEST_CASE(named_const_ptr) {
    const int i = 42;
    aux::unique_ptr<int> ptr(new int(i));
    named<const int*> named_(ptr.get());

    BOOST_CHECK_EQUAL(ptr.get(), static_cast<const int*>(named_));
}

BOOST_AUTO_TEST_CASE(named_rvalue_ref) {
    int i = 42;
    named<int&&> named_(std::move(i));
    int&& irvalue = named_;

    BOOST_CHECK_EQUAL(i, irvalue);
}

BOOST_AUTO_TEST_CASE(named_shared_ptr) {
    const int i = 42;
    typedef named<aux::shared_ptr<int>, a> named_type;
    aux::shared_ptr<int> i_(new int(i));
    named_type named_(i_);

    BOOST_CHECK_EQUAL(i, *static_cast<aux::shared_ptr<int>>(named_));
    BOOST_CHECK_EQUAL(i_.get(), static_cast<aux::shared_ptr<int>>(named_).get());
}

BOOST_AUTO_TEST_CASE(named_unique_ptr) {
    const int i = 42;
    aux::unique_ptr<int> ptr(new int(i));
    named<aux::unique_ptr<int>> named_(std::move(ptr));

    BOOST_CHECK_EQUAL(i, *static_cast<aux::unique_ptr<int>>(named_));
    BOOST_CHECK_EQUAL(ptr.get(), static_cast<aux::unique_ptr<int>>(named_).get());
}

} // namespace di
} // namespace boost

