//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include "Test/Common/Data.hpp"
#include "QDeps/Back/Scopes/Singleton.hpp"

namespace QDeps
{
namespace Back
{
namespace Scopes
{
namespace UT
{

using namespace Test::Common;

BOOST_AUTO_TEST_CASE(SingletonCreate)
{
    Singleton l_s;

    BOOST_CHECK((l_s.create<int>() == l_s.create<int>()));
}

BOOST_AUTO_TEST_CASE(SingletonCreateArgs)
{
    Singleton l_s;

    BOOST_CHECK((l_s.create<C2, int, double, char>(0, 0.0, '0') == l_s.create<C2, int, double, char>(0, 0.0, '0')));
}

} // namespace UT
} // namespace Scope
} // namespace Back
} // namespace QDeps

