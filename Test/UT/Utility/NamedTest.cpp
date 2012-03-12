//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include "QDeps/Utility/Named.hpp"

namespace QDeps
{
namespace Utility
{
namespace UT
{

class Name { };
class I { };
class Impl : I { };

TEST(Named, BaseOf)
{
    EXPECT_TRUE((boost::is_base_of<I, Named<Impl, Name> >::value));
    EXPECT_FALSE((boost::is_base_of<I, Named<int, Name> >::value));
    EXPECT_FALSE((boost::is_base_of<int, Named<Impl, Name> >::value));

    EXPECT_TRUE((boost::is_base_of<Named<I, Name>, Impl>::value));
    EXPECT_FALSE((boost::is_base_of<Named<int, Name>, Impl>::value));
    EXPECT_FALSE((boost::is_base_of<Named<I, Name>, int>::value));
}

} // namespace UT
} // namespace Utility
} // namespace QDeps

