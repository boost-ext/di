//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include "QDeps/Back/Binding.hpp"

namespace QDeps
{
namespace Back
{
namespace Detail
{
namespace UT
{

using namespace boost;
using namespace boost::mpl;

template<typename T, typename TContext = vector0<> > struct Dependency
{
    typedef is_same<_1, T> Bind;
    typedef TContext Context;
};

template<typename T, typename TContext = vector0<> > struct DependencyBaseOf
{
    typedef is_base_of<_1, T> Bind;
    typedef TContext Context;
};

class A { };
class B { };
class C { };
class D { };
class I { };
class Impl : public I { };

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
            >
        >::value
    ));
}

TEST(Binding, One)
{
    EXPECT_TRUE((
        equal
        <
            vector1< Dependency<int> >,
            Binding
            <
                int,
                vector0<>,
                vector
                <
                    Dependency<int>
                >
            >
        >::value
    ));
}

TEST(Binding, Found)
{
    EXPECT_TRUE((
        equal
        <
            vector< Dependency<float> >,
            Binding
            <
                float,
                vector0<>,
                vector
                <
                    Dependency<int>,
                    Dependency<float>,
                    Dependency<double>
                >
            >
        >::value
    ));
}

TEST(Binding, FoundMany)
{
    EXPECT_TRUE((
        equal
        <
            vector< Dependency<float>, Dependency<float> >,
            Binding
            <
                float,
                vector0<>,
                vector
                <
                    Dependency<int>,
                    Dependency<float>,
                    Dependency<float>
                >
            >
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
                    Dependency<int>,
                    Dependency<float>,
                    Dependency<float>
                >
            >
        >::value
    ));
}

TEST(Binding, Context)
{
    EXPECT_TRUE((
        equal
        <
            vector< Dependency<int, vector<A, B> > >,
            Binding
            <
                int,
                vector<A, B>,
                vector
                <
                    Dependency<int, vector<A> >,
                    Dependency<int, vector<A, B> >
                >
            >
        >::value
    ));
}

TEST(Binding, ContextMany)
{
    EXPECT_TRUE((
        equal
        <
            vector< Dependency<int, vector<A, B> >, Dependency<int> >,
            Binding
            <
                int,
                vector<A, B>,
                vector
                <
                    Dependency<int>,
                    Dependency<int, vector<A> >,
                    Dependency<int, vector<A, B> >
                >
            >
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
                Dependency<int, vector<A, B> >,
                Dependency<int, vector<B> >,
                Dependency<int>
            >,
            Binding
            <
                int,
                vector<A, B>,
                vector
                <
                    Dependency<int>,
                    Dependency<int, vector<B> >,
                    Dependency<int, vector<A, B> >
                >
            >
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
                    Dependency<int, vector<B> >,
                    Dependency<int, vector<A, B> >
                >
            >
        >::value
    ));
}

TEST(Binding, ContextOtherTypes)
{
    EXPECT_TRUE((
        equal
        <
            vector< Dependency<int, vector<A, B> > >,
            Binding
            <
                int,
                vector<A, B>,
                vector
                <
                    Dependency<int, vector<A> >,
                    Dependency<int, vector<A, B> >,
                    Dependency<float, vector<A, B> >,
                    Dependency<double>
                >
            >
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
                Dependency<int, vector<A, B, C> >,
                Dependency<int, vector<B, C> >,
                Dependency<int, vector<C> >,
                Dependency<int>
            >,
            Binding
            <
                int,
                vector<A, B, C>,
                vector
                <
                    Dependency<int, vector<A, B, C> >,
                    Dependency<int, vector<A> >,
                    Dependency<int, vector<B> >,
                    Dependency<int>,
                    Dependency<int, vector<B, C> >,
                    Dependency<int, vector<B, A, C> >,
                    Dependency<int, vector<B, C, C> >,
                    Dependency<int, vector<A, A, A> >,
                    Dependency<int, vector<C> >
                >
            >
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
                Dependency<int>
            >,
            Binding
            <
                int,
                vector<>,
                vector
                <
                    Dependency<int, vector<A, B, C> >,
                    Dependency<int, vector<A> >,
                    Dependency<int, vector<B> >,
                    Dependency<int>,
                    Dependency<int, vector<B, C> >,
                    Dependency<int, vector<B, A, C> >,
                    Dependency<int, vector<B, C, C> >,
                    Dependency<int, vector<A, A, A> >,
                    Dependency<int, vector<C> >
                >
            >
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
                Dependency<int, vector<B> >,
                Dependency<int>
            >,
            Binding
            <
                int,
                vector<C, A, B>,
                vector
                <
                    Dependency<int, vector<A, B, C> >,
                    Dependency<int, vector<A> >,
                    Dependency<int, vector<B> >,
                    Dependency<int>,
                    Dependency<int, vector<B, C> >,
                    Dependency<int, vector<B, A, C> >,
                    Dependency<int, vector<B, C, C> >,
                    Dependency<int, vector<A, A, A> >,
                    Dependency<int, vector<C> >
                >
            >
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
                Dependency<int, vector<C> >,
                Dependency<int>
            >,
            Binding
            <
                int,
                vector<C>,
                vector
                <
                    Dependency<int, vector<A, B, C> >,
                    Dependency<int, vector<A> >,
                    Dependency<int, vector<B> >,
                    Dependency<int>,
                    Dependency<int, vector<B, C> >,
                    Dependency<int, vector<B, A, C> >,
                    Dependency<int, vector<B, C, C> >,
                    Dependency<int, vector<A, A, A> >,
                    Dependency<int, vector<C> >
                >
            >
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
                Dependency<int>
            >,
            Binding
            <
                int,
                vector<A, B, C, D>,
                vector
                <
                    Dependency<int, vector<A, B, C> >,
                    Dependency<int, vector<A> >,
                    Dependency<int, vector<B> >,
                    Dependency<int>,
                    Dependency<int, vector<B, C> >,
                    Dependency<int, vector<B, A, C> >,
                    Dependency<int, vector<B, C, C> >,
                    Dependency<int, vector<A, A, A> >,
                    Dependency<int, vector<C> >
                >
            >
        >::value
    ));
}

TEST(Binding, BaseOfFail)
{
    EXPECT_TRUE((
        equal
        <
            vector0<>,
            Binding
            <
                I,
                vector0<>,
                vector
                <
                    DependencyBaseOf<A>
                >
            >
        >::value
    ));
}

TEST(Binding, BaseOfSuccessful)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                DependencyBaseOf<Impl>
            >,
            Binding
            <
                I,
                vector0<>,
                vector
                <
                    DependencyBaseOf<Impl>
                >
            >
        >::value
    ));
}

} // namespace UT
} // namespace Detail
} // namespace Back
} // namespace QDeps

