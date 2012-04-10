//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include "Test/Common/Data.hpp"
#include "QDeps/Back/Scopes/PerRequest.hpp"

namespace QDeps
{
namespace Back
{
namespace Scopes
{
namespace UT
{

using namespace Test::Common;

BOOST_AUTO_TEST_CASE(PerRequestCreate)
{
    PerRequest l_pr;

    BOOST_CHECK((l_pr.create<int>().get() != l_pr.create<int>().get()));
}

BOOST_AUTO_TEST_CASE(PerRequestCreateArgs)
{
    PerRequest l_pr;

    BOOST_CHECK((l_pr.create<C2, int, double, char>(0, 0.0, '0').get() != l_pr.create<C2, int, double, char>(0, 0.0, '0').get()));
}

} // namespace UT
} // namespace Scope
} // namespace Back
} // namespace QDeps

