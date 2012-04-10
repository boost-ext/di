//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include "QDeps/Utility/Named.hpp"

namespace QDeps
{
namespace Utility
{
namespace UT
{

using namespace boost;

class Name { };
class I { };
class Impl : I { };

BOOST_AUTO_TEST_CASE(BaseOf)
{
}

} // namespace UT
} // namespace Utility
} // namespace QDeps

