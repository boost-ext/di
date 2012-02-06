//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "QDeps/Back/Detail/Creator.hpp"

namespace QDeps
{
namespace Back
{
namespace Detail
{
namespace UT
{

template<typename T> struct Dep
{
    typedef T Dependency;
};

TEST(Creator, Empty)
{
    EXPECT_TRUE((
        boost::mpl::equal
        <
            boost::mpl::vector0<>,
            Creator<int, boost::mpl::vector0<> >::type
        >::value
    ));
}

TEST(Creator, Found)
{
    EXPECT_TRUE((
        boost::mpl::equal
        <
            boost::mpl::vector< Dep<float> >,
            Creator<float, boost::mpl::vector<Dep<int>, Dep<float>, Dep<double> > >::type
        >::value
    ));
}

TEST(Creator, FoundMany)
{
    EXPECT_TRUE((
        boost::mpl::equal
        <
            boost::mpl::vector< Dep<float>, Dep<float> >,
            Creator<float, boost::mpl::vector<Dep<int>, Dep<float>, Dep<float> > >::type
        >::value
    ));
}

TEST(Creator, NotFound)
{
    EXPECT_TRUE((
        boost::mpl::equal
        <
            boost::mpl::vector0<>,
            Creator<double, boost::mpl::vector<Dep<int>, Dep<float>, Dep<float> > >::type
        >::value
    ));
}

} // namespace UT
} // namespace Detail
} // namespace Back
} // namespace QDeps

