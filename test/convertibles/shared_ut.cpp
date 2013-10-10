//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/convertibles/shared.hpp"

#include <boost/test/unit_test.hpp>
#include <memory>

#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace convertibles {

BOOST_AUTO_TEST_CASE(to_shared_ptr) {
    shared_ptr<int> i(new int(42));
    shared_ptr<int> object = shared<int>(i);
    BOOST_CHECK_EQUAL(i, object);
}

BOOST_AUTO_TEST_CASE(to_named_shared_ptr) {
    shared_ptr<int> i(new int(42));
    named<shared_ptr<int>> object = shared<int>(i);
    BOOST_CHECK_EQUAL(i, static_cast<shared_ptr<int>>(object));
}

BOOST_AUTO_TEST_CASE(to_weak_ptr) {
    weak_ptr<int> object;

    {
    shared_ptr<int> i(new int(42));
    object = shared<int>(i);
    auto object_ = object.lock();
    BOOST_CHECK_EQUAL(i, object_);
    }

    BOOST_CHECK(!object.lock());
}

BOOST_AUTO_TEST_CASE(to_named_weak_ptr) {
    named<weak_ptr<int>> object;

    {
    shared_ptr<int> i(new int(42));
    object = shared<int>(i);
    auto object_ = static_cast<weak_ptr<int>>(object).lock();
    BOOST_CHECK_EQUAL(i, object_);
    }

    BOOST_CHECK(!static_cast<weak_ptr<int>>(object).lock());
}

} // namespace convertibles
} // namespace di
} // namespace boost

