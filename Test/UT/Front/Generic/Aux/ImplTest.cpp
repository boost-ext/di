//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "QDeps/Front/Generic/Aux/Impl.hpp"

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

TEST(Impl, One)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Detail::ImplDependency<int, double, A>
            >,
            Impl<int, double>::Bind<A>
        >::value
    ));
}

TEST(Impl, Many)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Detail::ImplDependency<int, double, A>,
                Detail::ImplDependency<int, double, B>,
                Detail::ImplDependency<int, double, C>,
                Detail::ImplDependency<int, double, D>
            >,
            Impl<int, double>::Bind<A, B, C, D>
        >::value
    ));
}

} // namespace UT
} // namespace Aux
} // namespace Generic
} // namespace Front
} // namespace QDeps

