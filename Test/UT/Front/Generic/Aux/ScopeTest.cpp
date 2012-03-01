//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "QDeps/Front/Generic/Aux/Scope.hpp"

namespace QDeps
{
namespace Front
{
namespace Generic
{
namespace Aux
{
namespace UT
{

using namespace boost::mpl;

class A { };
class B { };
class C { };
class D { };

TEST(Scope, One)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Detail::ScopeDependency<int, A>
            >,
            Scope<int>::Bind
            <
                A
            >
        >::value
    ));
}

TEST(Scope, Many)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Detail::ScopeDependency<int, A>,
                Detail::ScopeDependency<int, B>,
                Detail::ScopeDependency<int, C>,
                Detail::ScopeDependency<int, D>
            >,
            Scope<int>::Bind
            <
                A, B, C, D
            >
        >::value
    ));
}

} // namespace UT
} // namespace Aux
} // namespace Generic
} // namespace Front
} // namespace QDeps

