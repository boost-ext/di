//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#define QDEPS_STATIC_ASSERT(cond, expr, types) typedef void type

#include <gtest/gtest.h>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include "QDeps/Back/Policies/CheckForCircularDependencies.hpp"
#include "Test/Common/Data.hpp"

namespace QDeps
{
namespace Back
{
namespace Policies
{
namespace UT
{

using namespace Test::Common;
using namespace boost::mpl;

TEST(CheckForCircularDependencies, NoCircularDependencies)
{
    EXPECT_FALSE((
        boost::is_base_of
        <
            boost::mpl::false_,
            CheckForCircularDependencies::Assert
            <
                vector0<>,
                C8
            >::type
        >::value
    ));
}

TEST(CheckForCircularDependencies, Direct)
{
    EXPECT_TRUE((
        boost::is_base_of
        <
            boost::mpl::false_,
            CheckForCircularDependencies::Assert
            <
                vector0<>,
                CD1
            >::type
        >::value
    ));
}

TEST(CheckForCircularDependencies, InDirect)
{
    EXPECT_TRUE((
        boost::is_base_of
        <
            boost::mpl::false_,
            CheckForCircularDependencies::Assert
            <
                vector0<>,
                CD5
            >::type
        >::value
    ));
}

} // namespace UT
} // namespace Policies
} // namespace Back
} // namespace QDeps

