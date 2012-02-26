//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "QDeps/Back/Binding.hpp"

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

TEST(Binding, Empty)
{
    EXPECT_TRUE((
        equal
        <
            vector0<>,
            Binding
            <
                int,
                vector0<>,
                vector0<>
            >::type
        >::value
    ));
}

TEST(Binding, One)
{
    EXPECT_TRUE((
        equal
        <
            vector1< Dep<int> >,
            Binding
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

TEST(Binding, Found)
{
    EXPECT_TRUE((
        equal
        <
            vector< Dep<float> >,
            Binding
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

TEST(Binding, FoundMany)
{
    EXPECT_TRUE((
        equal
        <
            vector< Dep<float>, Dep<float> >,
            Binding
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

TEST(Binding, NotFound)
{
    EXPECT_TRUE((
        equal
        <
            vector0<>,
            Binding
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

TEST(Binding, Context)
{
    EXPECT_TRUE((
        equal
        <
            vector< Dep<int, vector<A, B> > >,
            Binding
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

TEST(Binding, ContextMany)
{
    EXPECT_TRUE((
        equal
        <
            vector< Dep<int, vector<A, B> >, Dep<int> >,
            Binding
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

TEST(Binding, ContextManyEnd)
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
            Binding
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

TEST(Binding, ContextNotFound)
{
    EXPECT_TRUE((
        equal
        <
            vector0<>,
            Binding
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

TEST(Binding, ContextOtherTypes)
{
    EXPECT_TRUE((
        equal
        <
            vector< Dep<int, vector<A, B> > >,
            Binding
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

TEST(Binding, ContextLongWithOrder)
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
            Binding
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

TEST(Binding, ContextLongWithOrderEmptyCallStack)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dep<int>
            >,
            Binding
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

TEST(Binding, ContextLongWithOrderDiffCallStack)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dep<int, vector<B> >,
                Dep<int>
            >,
            Binding
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

TEST(Binding, ContextLongWithOrderShortCallStack)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dep<int, vector<C> >,
                Dep<int>
            >,
            Binding
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

TEST(Binding, ContextLongWithOrderToLongCallStack)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dep<int>
            >,
            Binding
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

