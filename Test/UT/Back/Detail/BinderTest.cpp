//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>
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
using namespace Aux;

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
        is_same
        <
            Dependency<void, int>,
            Binder
            <
                int,
                vector0<>,
                vector0<>,
                Dependency<void, _1>
            >::type
        >::value
    ));
}

TEST(Binder, One)
{
    EXPECT_TRUE((
        is_same
        <
            Dependency<int>,
            Binder
            <
                int,
                vector0<>,
                vector
                <
                    Dependency<int>
                >,
                Dependency<void, _1>
            >::type
        >::value
    ));
}

TEST(Binder, Found)
{
    EXPECT_TRUE((
        is_same
        <
            Dependency<float>,
            Binder
            <
                float,
                vector0<>,
                vector
                <
                    Dependency<int>,
                    Dependency<float>,
                    Dependency<double>
                >,
                Dependency<void, _1>
            >::type
        >::value
    ));
}

TEST(Binder, FoundMany)
{
    EXPECT_TRUE((
        is_same
        <
            Dependency<float>,
            Binder
            <
                float,
                vector0<>,
                vector
                <
                    Dependency<int>,
                    Dependency<float>,
                    Dependency<float>
                >,
                Dependency<void, _1>
            >::type
        >::value
    ));
}

TEST(Binder, NotFound)
{
    EXPECT_TRUE((
        is_same
        <
            Dependency<void, double>,
            Binder
            <
                double,
                vector0<>,
                vector
                <
                    Dependency<int>,
                    Dependency<float>,
                    Dependency<float>
                >,
                Dependency<void, _1>
            >::type
        >::value
    ));
}

TEST(Binder, Context)
{
    EXPECT_TRUE((
        is_same
        <
            Dependency<int, vector<A, B> >,
            Binder
            <
                int,
                vector<A, B>,
                vector
                <
                    Dependency<int, vector<A> >,
                    Dependency<int, vector<A, B> >
                >,
                Dependency<void, _1>
            >::type
        >::value
    ));
}

TEST(Binder, ContextMany)
{
    EXPECT_TRUE((
        is_same
        <
            Dependency<int, vector<A, B> >,
            Binder
            <
                int,
                vector<A, B>,
                vector
                <
                    Dependency<int>,
                    Dependency<int, vector<A> >,
                    Dependency<int, vector<A, B> >
                >,
                Dependency<void, _1>
            >::type
        >::value
    ));
}

TEST(Binder, ContextManyEnd)
{
    EXPECT_TRUE((
        is_same
        <
            Dependency<int, vector<A, B> >,
            Binder
            <
                int,
                vector<A, B>,
                vector
                <
                    Dependency<int>,
                    Dependency<int, vector<B> >,
                    Dependency<int, vector<A, B> >
                >,
                Dependency<void, _1>
            >::type
        >::value
    ));
}

TEST(Binder, ContextNotFound)
{
    EXPECT_TRUE((
        is_same
        <
            Dependency<void, int>,
            Binder
            <
                int,
                vector<A>,
                vector
                <
                    Dependency<int, vector<B> >,
                    Dependency<int, vector<A, B> >
                >,
                Dependency<void, _1>
            >::type
        >::value
    ));
}

TEST(Binder, ContextOtherTypes)
{
    EXPECT_TRUE((
        is_same
        <
            Dependency<int, vector<A, B> >,
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
                >,
                Dependency<void, _1>
            >::type
        >::value
    ));
}

TEST(Binder, ContextLongWithOrder)
{
    EXPECT_TRUE((
        is_same
        <
            Dependency<int, vector<A, B, C> >,
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
                >,
                Dependency<void, _1>
            >::type
        >::value
    ));
}

TEST(Binder, ContextLongWithOrderEmptyCallStack)
{
    EXPECT_TRUE((
        is_same
        <
            Dependency<int>,
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
                >,
                Dependency<void, _1>
            >::type
        >::value
    ));
}

TEST(Binder, ContextLongWithOrderDiffCallStack)
{
    EXPECT_TRUE((
        is_same
        <
            Dependency<int, vector<B> >,
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
                >,
                Dependency<void, _1>
            >::type
        >::value
    ));
}

TEST(Binder, ContextLongWithOrderShortCallStack)
{
    EXPECT_TRUE((
        is_same
        <
            Dependency<int, vector<C> >,
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
                >,
                Dependency<void, _1>
            >::type
        >::value
    ));
}

TEST(Binder, ContextLongWithOrderToLongCallStack)
{
    EXPECT_TRUE((
        is_same
        <
            Dependency<int>,
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
                >,
                Dependency<void, _1>
            >::type
        >::value
    ));
}

TEST(Binder, BaseOfFail)
{
    EXPECT_TRUE((
        is_same
        <
            Dependency<void, I>,
            Binder
            <
                I,
                vector0<>,
                vector
                <
                    DependencyBaseOf<A>
                >,
                Dependency<void, _1>
            >::type
        >::value
    ));
}

TEST(Binder, BaseOfSuccessful)
{
    EXPECT_TRUE((
        is_same
        <
            DependencyBaseOf<Impl>,
            Binder
            <
                I,
                vector0<>,
                vector
                <
                    DependencyBaseOf<Impl>
                >,
                Dependency<void, _1>
            >::type
        >::value
    ));
}

} // namespace UT
} // namespace Detail
} // namespace Back
} // namespace QDeps

