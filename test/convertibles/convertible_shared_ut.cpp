//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/convertibles/convertible_shared.hpp"

#include <boost/test/unit_test.hpp>
#include <memory>

#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace convertibles {

BOOST_AUTO_TEST_CASE(to_shared_ptr) {
    shared_ptr<int> i(new int(42));
    shared_ptr<int> object = convertible_shared<int>(i);
    BOOST_CHECK_EQUAL(i, object);
}

} // namespace convertibles
} // namespace di
} // namespace boost

