//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/bool.hpp>
#include "QDeps/Front/Generic/Detail/Keys.hpp"

namespace QDeps
{
namespace Front
{
namespace Generic
{
namespace Detail
{
namespace UT
{

using namespace boost::mpl;

struct A
{
    typedef boost::mpl::false_ HasValue;
    typedef void Dependency;
};

struct B
{
    typedef boost::mpl::true_ HasValue;
    typedef int Dependency;
};

TEST(Keys, Empty)
{
    EXPECT_TRUE((
        equal
        <
            vector0<>,
            Keys
            <
                vector0<>
            >
        >::value
    ));
}

TEST(Keys, HasValue)
{
    EXPECT_TRUE((
        equal
        <
            vector<void>,
            Keys
            <
                vector<A>
            >
        >::value
    ));
}

TEST(Keys, Mix)
{
    EXPECT_TRUE((
        equal
        <
            vector<void>,
            Keys
            <
                vector
                <
                    A, B
                >
            >
        >::value
    ));
}

} // namespace UT
} // namespace Detail
} // namespace Generic
} // namespace Front
} // namespace QDeps

