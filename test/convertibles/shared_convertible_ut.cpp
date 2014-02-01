//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/convertibles/shared.hpp"

#include <boost/test/unit_test.hpp>
#include <memory>
#include <boost/type.hpp>

#include "boost/di/aux_/memory.hpp"
#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace convertibles {

BOOST_AUTO_TEST_CASE(to_shared_ptr) {
    aux::shared_ptr<int> i(new int(42));
    aux::shared_ptr<int> object((shared<int>(i))(type<aux::shared_ptr<int>>()));
    BOOST_CHECK_EQUAL(i, object);
}

BOOST_AUTO_TEST_CASE(to_named_shared_ptr) {
    aux::shared_ptr<int> i(new int(42));
    named<aux::shared_ptr<int>> object((shared<int>(i))(type<named<aux::shared_ptr<int>>>()));
    BOOST_CHECK_EQUAL(i, static_cast<aux::shared_ptr<int>>(object));
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

} // namespace convertibles
} // namespace di
} // namespace boost

