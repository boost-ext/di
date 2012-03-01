//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/pair.hpp>
#include "QDeps/Utility/Scopes/PerRequest.hpp"
#include "QDeps/Utility/Scopes/Singleton.hpp"
#include "QDeps/Front/Generic/Detail/Deps.hpp"

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
using namespace Utility::Scopes;

struct A
{
    template<typename T> struct Apply
    {
        typedef pair<A, T> type;
    };
};

struct B
{
    template<typename T> struct Apply
    {
        typedef pair<B, T> type;
    };
};

struct C : boost::mpl::vector<A, B>
{
    template<typename T> struct Apply
    {
        typedef pair<C, T> type;
    };
};

struct D
{
    template<typename T> struct Apply
    {
        typedef pair<D, T> type;
    };
};

TEST(Deps, Empty)
{
    EXPECT_TRUE((
        equal
        <
            vector0<>,
            Deps
            <
                vector0<>
            >
        >::value
    ));
}

TEST(Deps, One)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                pair<A, PerRequest>
            >,
            Deps
            <
                vector
                <
                    A
                >
            >
        >::value
    ));
}

TEST(Deps, Many)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                pair<B, PerRequest>,
                pair<A, PerRequest>
            >,
            Deps
            <
                vector
                <
                    A, B
                >
            >
        >::value
    ));
}

TEST(Deps, Seq)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                pair<D, PerRequest>,
                pair<B, PerRequest>,
                pair<A, PerRequest>
            >,
            Deps
            <
                vector
                <
                    C, D
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

