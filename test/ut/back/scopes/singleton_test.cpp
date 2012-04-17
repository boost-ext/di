//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include "test/common/data.hpp"
#include "di/back/scopes/singleton.hpp"

namespace di
{
namespace back
{
namespace scopes
{
namespace ut
{

using namespace test::common;

BOOST_AUTO_TEST_CASE(singletonCreate)
{
    singleton::scope<int> l_s;

    BOOST_CHECK((l_s.create() == l_s.create()));
}

BOOST_AUTO_TEST_CASE(singletonCreateArgs)
{
    singleton::scope<c2> l_s;

    BOOST_CHECK((l_s.create<int, double, char>(0, 0.0, '0') == l_s.create<int, double, char>(0, 0.0, '0')));
}

} // namespace ut
} // namespace scope
} // namespace back
} // namespace di

