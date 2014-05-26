//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/wrappers/universal.hpp"

#include <boost/test/unit_test.hpp>
#include <vector>

#include "boost/di/aux_/memory.hpp"
#include "boost/di/named.hpp"
#include "common/fakes/fake_wrapper.hpp"

namespace boost {
namespace di {
namespace wrappers {

BOOST_AUTO_TEST_CASE(convert_to_basic_type) {
    const int i = 42;
    std::vector<aux::shared_ptr<void>> refs;
    universal<int> c(refs, fake_wrapper<int>(i));
    BOOST_CHECK_EQUAL(i, static_cast<int>(c));
}

BOOST_AUTO_TEST_CASE(convert_to_named_type) {
    const int i = 42;
    std::vector<aux::shared_ptr<void>> refs;
    universal<named<int>> c(refs, fake_wrapper<int>(i));
    BOOST_CHECK_EQUAL(i, c.operator int());
    BOOST_CHECK_EQUAL(i, c.operator named<int>());
}

} // namespace wrappers
} // namespace di
} // namespace boost

