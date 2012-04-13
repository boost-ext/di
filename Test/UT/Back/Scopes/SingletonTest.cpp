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
    Singleton::Scope<int> l_s;

    BOOST_CHECK((l_s.create() == l_s.create()));
}

BOOST_AUTO_TEST_CASE(SingletonCreateArgs)
{
    Singleton::Scope<C2> l_s;

    BOOST_CHECK((l_s.create<int, double, char>(0, 0.0, '0') == l_s.create<int, double, char>(0, 0.0, '0')));
}

} // namespace UT
} // namespace Scope
} // namespace Back
} // namespace QDeps

