//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "QDeps/Front/Generic/Aux/Implementation.hpp"

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

TEST(Implementation, One)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Detail::ImplementationDependency<int, double, vector<A> >
            >,
            Implementation<int, double>::Bind<A>
        >::value
    ));
}

TEST(Implementation, Many)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Detail::ImplementationDependency<int, double, vector<A> >,
                Detail::ImplementationDependency<int, double, vector<B> >,
                Detail::ImplementationDependency<int, double, vector<C> >,
                Detail::ImplementationDependency<int, double, vector<D> >
            >,
            Implementation<int, double>::Bind<A, B, C, D>
        >::value
    ));
}

} // namespace UT
} // namespace Aux
} // namespace Generic
} // namespace Front
} // namespace QDeps

