//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/or.hpp>
#include "Test/Common/Data.hpp"
#include "QDeps/Back/Dependency.hpp"
#include "QDeps/Front/Base/Module.hpp"
#include "QDeps/Utility/Named.hpp"

namespace QDeps
{
namespace Front
{
namespace Fusion
{
namespace UT
{

using namespace Test::Common;
using namespace Back;
using namespace Utility;
using namespace boost::mpl;
using namespace boost;

TEST(FusionModule, Empty)
{
}

} // namespace UT
} // namespace Fusion
} // namespace Front
} // namespace QDeps

