//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/named.hpp"

#include <memory>
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/type_traits/is_same.hpp>

#include "data.hpp"

namespace boost {
namespace di {

BOOST_AUTO_TEST_CASE(named_int_value) {
    const int i = 42;
    typedef named<int, a> named_type;
    named_type named_(i);

    BOOST_CHECK_EQUAL(i, named_);
}

BOOST_AUTO_TEST_CASE(named_int_ref) {
    const int i = 42;
    typedef named<int, a> named_type;
    named_type named_(i);

    int& ref = static_cast<int&>(named_);
    ref++;

    BOOST_CHECK_EQUAL(i + 1, named_);
}

BOOST_AUTO_TEST_CASE(named_shared_ptr) {
    const int i = 42;
    typedef named<shared_ptr<int>, a> named_type;
    shared_ptr<int> i_(new int(i));
    named_type named_(i_);

    BOOST_CHECK_EQUAL(i, *named_);
    BOOST_CHECK_EQUAL(i_.get(), named_.get());
}

BOOST_AUTO_TEST_CASE(named_smart_ptr) {
    const int i = 42;
    int* ptr = new int(i);
    named<std::unique_ptr<int>> named_(ptr);
    BOOST_CHECK_EQUAL(i, *named_);
    BOOST_CHECK_EQUAL(ptr, named_.get());
}

BOOST_AUTO_TEST_CASE(named_smart_ptr_reset) {
    const int i = 42;
    int* ptr = new int(i);
    named<std::unique_ptr<int>> named_(ptr);
    named_.reset();
    BOOST_CHECK(nullptr == named_.get());
}

BOOST_AUTO_TEST_CASE(named_smart_ptr_reset_ptr) {
    const int i = 42;
    int* ptr = new int(i);
    named<std::unique_ptr<int>> named_(nullptr);
    named_.reset(ptr);
    BOOST_CHECK_EQUAL(i, *named_);
    BOOST_CHECK_EQUAL(ptr, named_.get());
}

} // namespace di
} // namespace boost

