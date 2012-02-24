//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "QDeps/Back/Creator.hpp"

namespace QDeps
{
namespace Back
{
namespace Detail
{
namespace UT
{

template<typename T, typename TContext = boost::mpl::vector0<> > struct Dep
{
    typedef T Dependency;
    typedef TContext Context;
};

class A { };
class B { };
class C { };
class D { };

TEST(Creator, Empty)
{
    EXPECT_TRUE((
        boost::mpl::equal
        <
            boost::mpl::vector0<>,
            Creator
            <
                int,
                boost::mpl::vector0<>,
                boost::mpl::vector0<>
            >::type
        >::value
    ));
}

TEST(Creator, One)
{
    EXPECT_TRUE((
        boost::mpl::equal
        <
            boost::mpl::vector1< Dep<int> >,
            Creator
            <
                int,
                boost::mpl::vector0<>,
                boost::mpl::vector
                <
                    Dep<int>
                >
            >::type
        >::value
    ));
}

TEST(Creator, Found)
{
    EXPECT_TRUE((
        boost::mpl::equal
        <
            boost::mpl::vector< Dep<float> >,
            Creator
            <
                float,
                boost::mpl::vector0<>,
                boost::mpl::vector
                <
                    Dep<int>,
                    Dep<float>,
                    Dep<double>
                >
            >::type
        >::value
    ));
}

TEST(Creator, FoundMany)
{
    EXPECT_TRUE((
        boost::mpl::equal
        <
            boost::mpl::vector< Dep<float>, Dep<float> >,
            Creator
            <
                float,
                boost::mpl::vector0<>,
                boost::mpl::vector
                <
                    Dep<int>,
                    Dep<float>,
                    Dep<float>
                >
            >::type
        >::value
    ));
}

TEST(Creator, NotFound)
{
    EXPECT_TRUE((
        boost::mpl::equal
        <
            boost::mpl::vector0<>,
            Creator
            <
                double,
                boost::mpl::vector0<>,
                boost::mpl::vector
                <
                    Dep<int>,
                    Dep<float>,
                    Dep<float>
                >
            >::type
        >::value
    ));
}

TEST(Creator, Context)
{
    EXPECT_TRUE((
        boost::mpl::equal
        <
            boost::mpl::vector< Dep<int, boost::mpl::vector<A, B> > >,
            Creator
            <
                int,
                boost::mpl::vector<A, B>,
                boost::mpl::vector
                <
                    Dep<int, boost::mpl::vector<A> >,
                    Dep<int, boost::mpl::vector<A, B> >
                >
            >::type
        >::value
    ));
}

TEST(Creator, ContextMany)
{
    EXPECT_TRUE((
        boost::mpl::equal
        <
            boost::mpl::vector< Dep<int, boost::mpl::vector<A, B> >, Dep<int> >,
            Creator
            <
                int,
                boost::mpl::vector<A, B>,
                boost::mpl::vector
                <
                    Dep<int>,
                    Dep<int, boost::mpl::vector<A> >,
                    Dep<int, boost::mpl::vector<A, B> >
                >
            >::type
        >::value
    ));
}

TEST(Creator, ContextManyEnd)
{
    EXPECT_TRUE((
        boost::mpl::equal
        <
            boost::mpl::vector
            <
                Dep<int, boost::mpl::vector<A, B> >,
                Dep<int, boost::mpl::vector<B> >,
                Dep<int>
            >,
            Creator
            <
                int,
                boost::mpl::vector<A, B>,
                boost::mpl::vector
                <
                    Dep<int>,
                    Dep<int, boost::mpl::vector<B> >,
                    Dep<int, boost::mpl::vector<A, B> >
                >
            >::type
        >::value
    ));
}

TEST(Creator, ContextNotFound)
{
    EXPECT_TRUE((
        boost::mpl::equal
        <
            boost::mpl::vector0<>,
            Creator
            <
                int,
                boost::mpl::vector<A>,
                boost::mpl::vector
                <
                    Dep<int, boost::mpl::vector<B> >,
                    Dep<int, boost::mpl::vector<A, B> >
                >
            >::type
        >::value
    ));
}

TEST(Creator, ContextOtherTypes)
{
    EXPECT_TRUE((
        boost::mpl::equal
        <
            boost::mpl::vector< Dep<int, boost::mpl::vector<A, B> > >,
            Creator
            <
                int,
                boost::mpl::vector<A, B>,
                boost::mpl::vector
                <
                    Dep<int, boost::mpl::vector<A> >,
                    Dep<int, boost::mpl::vector<A, B> >,
                    Dep<float, boost::mpl::vector<A, B> >,
                    Dep<double>
                >
            >::type
        >::value
    ));
}

TEST(Creator, ContextLongWithOrder)
{
    EXPECT_TRUE((
        boost::mpl::equal
        <
            boost::mpl::vector
            <
                Dep<int, boost::mpl::vector<A, B, C> >,
                Dep<int, boost::mpl::vector<B, C> >,
                Dep<int, boost::mpl::vector<C> >,
                Dep<int>
            >,
            Creator
            <
                int,
                boost::mpl::vector<A, B, C>,
                boost::mpl::vector
                <
                    Dep<int, boost::mpl::vector<A, B, C> >,
                    Dep<int, boost::mpl::vector<A> >,
                    Dep<int, boost::mpl::vector<B> >,
                    Dep<int>,
                    Dep<int, boost::mpl::vector<B, C> >,
                    Dep<int, boost::mpl::vector<B, A, C> >,
                    Dep<int, boost::mpl::vector<B, C, C> >,
                    Dep<int, boost::mpl::vector<A, A, A> >,
                    Dep<int, boost::mpl::vector<C> >
                >
            >::type
        >::value
    ));
}

TEST(Creator, ContextLongWithOrderEmptyCallStack)
{
    EXPECT_TRUE((
        boost::mpl::equal
        <
            boost::mpl::vector
            <
                Dep<int>
            >,
            Creator
            <
                int,
                boost::mpl::vector<>,
                boost::mpl::vector
                <
                    Dep<int, boost::mpl::vector<A, B, C> >,
                    Dep<int, boost::mpl::vector<A> >,
                    Dep<int, boost::mpl::vector<B> >,
                    Dep<int>,
                    Dep<int, boost::mpl::vector<B, C> >,
                    Dep<int, boost::mpl::vector<B, A, C> >,
                    Dep<int, boost::mpl::vector<B, C, C> >,
                    Dep<int, boost::mpl::vector<A, A, A> >,
                    Dep<int, boost::mpl::vector<C> >
                >
            >::type
        >::value
    ));
}

TEST(Creator, ContextLongWithOrderDiffCallStack)
{
    EXPECT_TRUE((
        boost::mpl::equal
        <
            boost::mpl::vector
            <
                Dep<int, boost::mpl::vector<B> >,
                Dep<int>
            >,
            Creator
            <
                int,
                boost::mpl::vector<C, A, B>,
                boost::mpl::vector
                <
                    Dep<int, boost::mpl::vector<A, B, C> >,
                    Dep<int, boost::mpl::vector<A> >,
                    Dep<int, boost::mpl::vector<B> >,
                    Dep<int>,
                    Dep<int, boost::mpl::vector<B, C> >,
                    Dep<int, boost::mpl::vector<B, A, C> >,
                    Dep<int, boost::mpl::vector<B, C, C> >,
                    Dep<int, boost::mpl::vector<A, A, A> >,
                    Dep<int, boost::mpl::vector<C> >
                >
            >::type
        >::value
    ));
}

TEST(Creator, ContextLongWithOrderShortCallStack)
{
    EXPECT_TRUE((
        boost::mpl::equal
        <
            boost::mpl::vector
            <
                Dep<int, boost::mpl::vector<C> >,
                Dep<int>
            >,
            Creator
            <
                int,
                boost::mpl::vector<C>,
                boost::mpl::vector
                <
                    Dep<int, boost::mpl::vector<A, B, C> >,
                    Dep<int, boost::mpl::vector<A> >,
                    Dep<int, boost::mpl::vector<B> >,
                    Dep<int>,
                    Dep<int, boost::mpl::vector<B, C> >,
                    Dep<int, boost::mpl::vector<B, A, C> >,
                    Dep<int, boost::mpl::vector<B, C, C> >,
                    Dep<int, boost::mpl::vector<A, A, A> >,
                    Dep<int, boost::mpl::vector<C> >
                >
            >::type
        >::value
    ));
}

TEST(Creator, ContextLongWithOrderToLongCallStack)
{
    EXPECT_TRUE((
        boost::mpl::equal
        <
            boost::mpl::vector
            <
                Dep<int>
            >,
            Creator
            <
                int,
                boost::mpl::vector<A, B, C, D>,
                boost::mpl::vector
                <
                    Dep<int, boost::mpl::vector<A, B, C> >,
                    Dep<int, boost::mpl::vector<A> >,
                    Dep<int, boost::mpl::vector<B> >,
                    Dep<int>,
                    Dep<int, boost::mpl::vector<B, C> >,
                    Dep<int, boost::mpl::vector<B, A, C> >,
                    Dep<int, boost::mpl::vector<B, C, C> >,
                    Dep<int, boost::mpl::vector<A, A, A> >,
                    Dep<int, boost::mpl::vector<C> >
                >
            >::type
        >::value
    ));
}

} // namespace UT
} // namespace Detail
} // namespace Back
} // namespace QDeps

