//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/cpp_0x/wrappers/shared.hpp"

#include <boost/test/unit_test.hpp>
#include <memory>
#include <boost/type.hpp>

#include "boost/di/cpp_0x/aux_/memory.hpp"

namespace boost {
namespace di {
namespace wrappers {

BOOST_AUTO_TEST_CASE(ctor_reset) {
    std::shared_ptr<int> i(new int(42));
    shared<int> convertible(i);
    BOOST_CHECK_EQUAL(false, !convertible);
    convertible.reset();
    BOOST_CHECK_EQUAL(true, !convertible);
}

BOOST_AUTO_TEST_CASE(to_shared_ptr) {
    std::shared_ptr<int> i(new int(42));
    std::shared_ptr<int> object((shared<int>(i))(type<std::shared_ptr<int>>()));
    BOOST_CHECK_EQUAL(i, object);
}

BOOST_AUTO_TEST_CASE(to_other_shared_ptr) {
    std::shared_ptr<int> i(new int(42));
    boost::shared_ptr<int> object((shared<int>(i))(type<boost::shared_ptr<int>>()));
    BOOST_CHECK_EQUAL(i.get(), object.get());
}

BOOST_AUTO_TEST_CASE(to_weak_ptr) {
    std::weak_ptr<int> object;

    {
    std::shared_ptr<int> i(new int(42));
    object = (shared<int>(i))(type<std::weak_ptr<int>>());
    auto object_ = object.lock();
    BOOST_CHECK_EQUAL(i, object_);
    }

    BOOST_CHECK(!object.lock());
}

} // namespace wrappers
} // namespace di
} // namespace boost

