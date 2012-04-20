//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include "boost/di/scopes/singleton.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace scopes {
namespace test {

BOOST_AUTO_TEST_CASE(singleton_create)
{
    singleton::scope<int> singleton_;

    BOOST_CHECK((singleton_.create() == singleton_.create()));
}

BOOST_AUTO_TEST_CASE(singleton_create_args)
{
    singleton::scope<c2> singleton_;

    BOOST_CHECK((singleton_.create<int, double, char>(0, 0.0, '0') == singleton_.create<int, double, char>(0, 0.0, '0')));
}

} // namespace test
} // namespace scopes
} // namespace di
} // namespace boost

