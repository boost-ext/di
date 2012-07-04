//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/per_request.hpp"

#include <boost/test/unit_test.hpp>
#include <cstdlib>

#include "fake_allocator.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace scopes {

BOOST_AUTO_TEST_CASE(create) {
    per_request<>::scope<int> per_request_;

    BOOST_CHECK((
        static_cast<int*>(per_request_.create())
        !=
        static_cast<int*>(per_request_.create())
    ));
}

BOOST_AUTO_TEST_CASE(create_args) {
    per_request<>::scope<c2> per_request_;

    BOOST_CHECK((
        static_cast<c2*>(per_request_.create<int, double, char>(0, 0.0, '0'))
        !=
        static_cast<c2*>(per_request_.create<int, double, char>(0, 0.0, '0'))
    ));
}

BOOST_AUTO_TEST_CASE(create_allocator) {
    allocate_calls = 0;
    deallocate_calls = 0;

    per_request<fake_allocator>::scope<int> per_request_;
    per_request_.create<int>(0);

    BOOST_CHECK_EQUAL(1, allocate_calls);
    BOOST_CHECK_EQUAL(1, deallocate_calls);
}

} // namespace scopes
} // namespace di
} // namespace boost

