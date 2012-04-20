//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include "boost/di/scopes/per_request.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace scopes {
namespace test {

BOOST_AUTO_TEST_CASE(per_request_create)
{
    per_request::scope<int> per_request_;

    BOOST_CHECK((per_request_.create().get() != per_request_.create().get()));
}

BOOST_AUTO_TEST_CASE(per_request_create_args)
{
    per_request::scope<c2> per_request_;

    BOOST_CHECK((per_request_.create<int, double, char>(0, 0.0, '0').get() != per_request_.create<int, double, char>(0, 0.0, '0').get()));
}

} // namespace test
} // namespace scopes
} // namespace di
} // namespace boost

