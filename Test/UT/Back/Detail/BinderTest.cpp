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
#include "QDeps/Back/Detail/Binder.hpp"

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

TEST(Binder, Empty)
{
    EXPECT_TRUE((
        equal
        <
            vector0<>,
            Binder
            <
                int,
                vector0<>,
                vector0<>
            >
        >::value
    ));
}

TEST(Binder, One)
{
    EXPECT_TRUE((
        equal
        <
            vector1< Dependency<int> >,
            Binder
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

TEST(Binder, Found)
{
    EXPECT_TRUE((
        equal
        <
            vector< Dependency<float> >,
            Binder
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

TEST(Binder, FoundMany)
{
    EXPECT_TRUE((
        equal
        <
            vector< Dependency<float>, Dependency<float> >,
            Binder
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

TEST(Binder, NotFound)
{
    EXPECT_TRUE((
        equal
        <
            vector0<>,
            Binder
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

TEST(Binder, Context)
{
    EXPECT_TRUE((
        equal
        <
            vector< Dependency<int, vector<A, B> > >,
            Binder
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

TEST(Binder, ContextMany)
{
    EXPECT_TRUE((
        equal
        <
            vector< Dependency<int, vector<A, B> >, Dependency<int> >,
            Binder
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

TEST(Binder, ContextManyEnd)
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
            Binder
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

TEST(Binder, ContextNotFound)
{
    EXPECT_TRUE((
        equal
        <
            vector0<>,
            Binder
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

TEST(Binder, ContextOtherTypes)
{
    EXPECT_TRUE((
        equal
        <
            vector< Dependency<int, vector<A, B> > >,
            Binder
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

TEST(Binder, ContextLongWithOrder)
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
            Binder
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

TEST(Binder, ContextLongWithOrderEmptyCallStack)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<int>
            >,
            Binder
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

TEST(Binder, ContextLongWithOrderDiffCallStack)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<int, vector<B> >,
                Dependency<int>
            >,
            Binder
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

TEST(Binder, ContextLongWithOrderShortCallStack)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<int, vector<C> >,
                Dependency<int>
            >,
            Binder
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

TEST(Binder, ContextLongWithOrderToLongCallStack)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Dependency<int>
            >,
            Binder
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

TEST(Binder, BaseOfFail)
{
    EXPECT_TRUE((
        equal
        <
            vector0<>,
            Binder
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

TEST(Binder, BaseOfSuccessful)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                DependencyBaseOf<Impl>
            >,
            Binder
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

