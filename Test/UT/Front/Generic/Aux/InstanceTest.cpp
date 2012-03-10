//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "QDeps/Front/Generic/Aux/Instance.hpp"

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

TEST(Instance, One)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Detail::InstanceDependency<int, double, vector<A> >
            >,
            Instance<int, double>::Bind<A>
        >::value
    ));
}

TEST(Instance, Many)
{
    EXPECT_TRUE((
        equal
        <
            vector
            <
                Detail::InstanceDependency<int, double, vector<A> >,
                Detail::InstanceDependency<int, double, vector<B> >,
                Detail::InstanceDependency<int, double, vector<C> >,
                Detail::InstanceDependency<int, double, vector<D> >
            >,
            Instance<int, double>::Bind<A, B, C, D>
        >::value
    ));
}

} // namespace UT
} // namespace Aux
} // namespace Generic
} // namespace Front
} // namespace QDeps

