//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include "di/named.hpp"

namespace di
{
namespace utility
{
namespace ut
{

using namespace boost;

class name { };
class I { };
class Impl : I { };

BOOST_AUTO_TEST_CASE(baseOf)
{
}

} // namespace ut
} // namespace utility
} // namespace di

