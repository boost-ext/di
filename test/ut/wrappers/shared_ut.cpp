//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/wrappers/shared.hpp"

#include <boost/test/unit_test.hpp>
#include <memory>
#include <boost/type.hpp>

#include "boost/di/aux_/memory.hpp"

namespace boost {
namespace di {
namespace wrappers {

BOOST_AUTO_TEST_CASE(ctor_reset) {
    aux::shared_ptr<int> i(new int(42));
    shared<int> convertible(i);
    BOOST_CHECK_EQUAL(false, !convertible);
    convertible.reset();
    BOOST_CHECK_EQUAL(true, !convertible);
}

BOOST_AUTO_TEST_CASE(to_shared_ptr) {
    aux::shared_ptr<int> i(new int(42));
    aux::shared_ptr<int> object((shared<int>(i))(type<aux::shared_ptr<int>>()));
    BOOST_CHECK_EQUAL(i, object);
}

BOOST_AUTO_TEST_CASE(to_shared_ptr_ref) {
    aux::shared_ptr<int> i(new int(42));
    aux::shared_ptr<int> object((shared<int>(i))(type<const aux::shared_ptr<int>&>()));
    BOOST_CHECK_EQUAL(i, object);
}

BOOST_AUTO_TEST_CASE(to_other_shared_ptr) {
    aux::shared_ptr<int> i(new int(42));
    aux_::shared_ptr<int> object((shared<int>(i))(type<aux_::shared_ptr<int>>()));
    BOOST_CHECK_EQUAL(i.get(), object.get());
}

BOOST_AUTO_TEST_CASE(to_other_shared_ptr_ref) {
    aux::shared_ptr<int> i(new int(42));
    aux_::shared_ptr<int> object((shared<int>(i))(type<const aux_::shared_ptr<int>&>()));
    BOOST_CHECK_EQUAL(i.get(), object.get());
}

BOOST_AUTO_TEST_CASE(to_weak_ptr) {
    aux::weak_ptr<int> object;

    {
    aux::shared_ptr<int> i(new int(42));
    object = (shared<int>(i))(type<aux::weak_ptr<int>>());
    auto object_ = object.lock();
    BOOST_CHECK_EQUAL(i, object_);
    }

    BOOST_CHECK(!object.lock());
}

} // namespace wrappers
} // namespace di
} // namespace boost

