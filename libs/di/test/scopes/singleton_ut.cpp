//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/singleton.hpp"

#include <boost/test/unit_test.hpp>

#include "fake_allocator.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace scopes {

BOOST_AUTO_TEST_CASE(create) {
    singleton<>::scope<int> singleton_;

    BOOST_CHECK((singleton_.create() == singleton_.create()));
}

BOOST_AUTO_TEST_CASE(create_args) {
    singleton<>::scope<c2> singleton_;

    BOOST_CHECK((
        singleton_.create<int, double, char>(0, 0.0, '0')
        ==
        singleton_.create<int, double, char>(0, 0.0, '0'))
    );
}

BOOST_AUTO_TEST_CASE(create_allocator) {
    allocate_calls = 0;
    deallocate_calls = 0;

    struct deallocate
    {
        ~deallocate() {
            BOOST_CHECK_EQUAL(1, deallocate_calls);
        }
    } deallocate_;

    singleton<fake_allocator>::scope<int> singleton_;
    singleton_.create<int>(0);

    BOOST_CHECK_EQUAL(1, allocate_calls);
    BOOST_CHECK_EQUAL(0, deallocate_calls);

    (void)deallocate_;
}

} // namespace scopes
} // namespace di
} // namespace boost

