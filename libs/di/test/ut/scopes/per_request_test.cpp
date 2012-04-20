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
namespace test {
namespace ut {
namespace scopes {

BOOST_AUTO_TEST_CASE(per_requestCreate)
{
    per_request::scope<int> pr;

    BOOST_CHECK((pr.create().get() != pr.create().get()));
}

BOOST_AUTO_TEST_CASE(per_requestCreateArgs)
{
    per_request::scope<c2> pr;

    BOOST_CHECK((pr.create<int, double, char>(0, 0.0, '0').get() != pr.create<int, double, char>(0, 0.0, '0').get()));
}

} // namespace ut
} // namespace test
} // namespace scopes
} // namespace di
} // namespace boost

