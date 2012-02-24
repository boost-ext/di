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

using namespace boost::mpl;

template<typename T, typename TContext = vector0<> > struct Dep
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
        equal
        <
            vector0<>,
            Creator
            <
                int,
                vector0<>,
                vector0<>
            >::type
        >::value
    ));
}

TEST(Creator, One)
{
    EXPECT_TRUE((
        equal
        <
            vector1< Dep<int> >,
            Creator
            <
                int,
                vector0<>,
                vector
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
        equal
        <
            vector< Dep<float> >,
            Creator
            <
                float,
                vector0<>,
                vector
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
        equal
        <
            vector< Dep<float>, Dep<float> >,
            Creator
            <
                float,
                vector0<>,
                vector
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
        equal
        <
            vector0<>,
            Creator
            <
                double,
                vector0<>,
                vector
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
        equal
        <
            vector< Dep<int, vector<A, B> > >,
            Creator
            <
                int,
                vector<A, B>,
                vector
                <
                    Dep<int, vector<A> >,
                    Dep<int, vector<A, B> >
                >
            >::type
        >::value
    ));
}

TEST(Creator, ContextMany)
{
    EXPECT_TRUE((
        equal
        <
            vector< Dep<int, vector<A, B> >, Dep<int> >,
            Creator
            <
                int,
                vector<A, B>,
                vector
                <
                    Dep<int>,
                    Dep<int, vector<A> >,
                    Dep<int, vector<A, B> >
                >
            >::type
        >::value
    ));
}

TEST(Creator, ContextManyEnd)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dep<int, vector<A, B> >,
                Dep<int, vector<B> >,
                Dep<int>
            >,
            Creator
            <
                int,
                vector<A, B>,
                vector
                <
                    Dep<int>,
                    Dep<int, vector<B> >,
                    Dep<int, vector<A, B> >
                >
            >::type
        >::value
    ));
}

TEST(Creator, ContextNotFound)
{
    EXPECT_TRUE((
        equal
        <
            vector0<>,
            Creator
            <
                int,
                vector<A>,
                vector
                <
                    Dep<int, vector<B> >,
                    Dep<int, vector<A, B> >
                >
            >::type
        >::value
    ));
}

TEST(Creator, ContextOtherTypes)
{
    EXPECT_TRUE((
        equal
        <
            vector< Dep<int, vector<A, B> > >,
            Creator
            <
                int,
                vector<A, B>,
                vector
                <
                    Dep<int, vector<A> >,
                    Dep<int, vector<A, B> >,
                    Dep<float, vector<A, B> >,
                    Dep<double>
                >
            >::type
        >::value
    ));
}

TEST(Creator, ContextLongWithOrder)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dep<int, vector<A, B, C> >,
                Dep<int, vector<B, C> >,
                Dep<int, vector<C> >,
                Dep<int>
            >,
            Creator
            <
                int,
                vector<A, B, C>,
                vector
                <
                    Dep<int, vector<A, B, C> >,
                    Dep<int, vector<A> >,
                    Dep<int, vector<B> >,
                    Dep<int>,
                    Dep<int, vector<B, C> >,
                    Dep<int, vector<B, A, C> >,
                    Dep<int, vector<B, C, C> >,
                    Dep<int, vector<A, A, A> >,
                    Dep<int, vector<C> >
                >
            >::type
        >::value
    ));
}

TEST(Creator, ContextLongWithOrderEmptyCallStack)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dep<int>
            >,
            Creator
            <
                int,
                vector<>,
                vector
                <
                    Dep<int, vector<A, B, C> >,
                    Dep<int, vector<A> >,
                    Dep<int, vector<B> >,
                    Dep<int>,
                    Dep<int, vector<B, C> >,
                    Dep<int, vector<B, A, C> >,
                    Dep<int, vector<B, C, C> >,
                    Dep<int, vector<A, A, A> >,
                    Dep<int, vector<C> >
                >
            >::type
        >::value
    ));
}

TEST(Creator, ContextLongWithOrderDiffCallStack)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dep<int, vector<B> >,
                Dep<int>
            >,
            Creator
            <
                int,
                vector<C, A, B>,
                vector
                <
                    Dep<int, vector<A, B, C> >,
                    Dep<int, vector<A> >,
                    Dep<int, vector<B> >,
                    Dep<int>,
                    Dep<int, vector<B, C> >,
                    Dep<int, vector<B, A, C> >,
                    Dep<int, vector<B, C, C> >,
                    Dep<int, vector<A, A, A> >,
                    Dep<int, vector<C> >
                >
            >::type
        >::value
    ));
}

TEST(Creator, ContextLongWithOrderShortCallStack)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dep<int, vector<C> >,
                Dep<int>
            >,
            Creator
            <
                int,
                vector<C>,
                vector
                <
                    Dep<int, vector<A, B, C> >,
                    Dep<int, vector<A> >,
                    Dep<int, vector<B> >,
                    Dep<int>,
                    Dep<int, vector<B, C> >,
                    Dep<int, vector<B, A, C> >,
                    Dep<int, vector<B, C, C> >,
                    Dep<int, vector<A, A, A> >,
                    Dep<int, vector<C> >
                >
            >::type
        >::value
    ));
}

TEST(Creator, ContextLongWithOrderToLongCallStack)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dep<int>
            >,
            Creator
            <
                int,
                vector<A, B, C, D>,
                vector
                <
                    Dep<int, vector<A, B, C> >,
                    Dep<int, vector<A> >,
                    Dep<int, vector<B> >,
                    Dep<int>,
                    Dep<int, vector<B, C> >,
                    Dep<int, vector<B, A, C> >,
                    Dep<int, vector<B, C, C> >,
                    Dep<int, vector<A, A, A> >,
                    Dep<int, vector<C> >
                >
            >::type
        >::value
    ));
}

} // namespace UT
} // namespace Detail
} // namespace Back
} // namespace QDeps

