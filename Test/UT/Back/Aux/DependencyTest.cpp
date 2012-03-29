//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_same.hpp>
#include "QDeps/Back/Scope.hpp"
#include "QDeps/Back/Aux/Dependency.hpp"

namespace QDeps
{
namespace Back
{
namespace Aux
{
namespace UT
{

using namespace boost;
using namespace mpl;

template<typename T> struct FakeValue { };

template<typename TSeq, int Value = 0> struct FakePool
{
    typedef TSeq Seq;

    template<typename T> struct ResultType
    {
        typedef shared_ptr<T> type;
    };

    template<typename T> shared_ptr<T> get() { return make_shared<T>(Value); }
};

template<int Value = 0>
struct FakeScope : Scope< boost::shared_ptr<boost::mpl::_1> >
{
    template<typename T> shared_ptr<T> create() { return make_shared<T>(Value); }
};

struct OtherFakeScope { };

TEST(Dependency, Default)
{
    typedef Dependency<FakeScope<>, int> Dep;

    EXPECT_TRUE((is_same<vector0<>, Dep::Context>::value));
    EXPECT_TRUE((is_same<is_same<_1, int>, Dep::Bind>::value));
}

TEST(Dependency, Apply)
{
    EXPECT_TRUE((
        is_same
        <
            Dependency
            <
                OtherFakeScope,
                int,
                int,
                vector0<>,
                is_same<_1, int>,
                FakeValue
            >,
            Dependency
            <
                _1,
                int,
                int,
                vector0<>,
                is_same<_1, int>,
                FakeValue
            >::Apply<OtherFakeScope>::type
        >::value
    ));
}

TEST(Dependency, createByPool)
{
    const int i = 42;
    Dependency< FakeScope<>, int > dep;
    FakePool< vector<int>, i > pool;

    EXPECT_EQ(i, *dep.create(pool));
}

TEST(Dependency, createByValue)
{
    const int i = 42;
    Dependency< FakeScope<>, int, int_<i> > dep;
    FakePool< vector0<> > pool;

    EXPECT_EQ(i, *dep.create(pool));
}

TEST(Dependency, createByScope)
{
    const int i = 42;
    Dependency<FakeScope<i>, int> dep;
    FakePool< vector0<> > pool;

    EXPECT_EQ(i, *dep.create(pool));
}

} // namespace UT
} // namespace Aux
} // namespace Back
} // namespace QDeps

