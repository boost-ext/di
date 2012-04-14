//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_same.hpp>
#include "QDeps/Back/Aux/Instance.hpp"
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
using namespace boost::mpl;

template<typename T> struct FakeValue { };

template<typename TSeq, int Value = 0> struct FakePool
{
    typedef TSeq Seq;

    template<typename T> struct ResultType
    {
        typedef shared_ptr<int> type;
    };

    template<typename T> typename ResultType<int>::type get()
    {
       return make_shared<int>(Value);
    }
};

template<int Value = 0>
struct FakeScope
{
    template<typename T>
    struct Scope
    {
        typedef shared_ptr<T> ResultType;
        ResultType create() { return make_shared<T>(Value); }
    };
};

struct OtherFakeScope { };

BOOST_AUTO_TEST_CASE(Default)
{
    typedef Dependency<FakeScope<>, int> Dep;

    BOOST_CHECK((is_same<vector0<>, Dep::Context>::value));
    BOOST_CHECK((is_same<is_same<_1, int>, Dep::Bind>::value));
}

BOOST_AUTO_TEST_CASE(RebindScope)
{
    BOOST_CHECK((
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
            >::Rebind<OtherFakeScope>::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(RebindType)
{
    BOOST_CHECK((
        is_same
        <
            Dependency
            <
                void,
                double,
                int,
                vector0<>,
                is_same<_1, int>,
                FakeValue
            >,
            Dependency
            <
                void,
                _1,
                _2,
                vector0<>,
                is_same<_1, int>,
                FakeValue
            >::Rebind<double, int>::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(CreateByPool)
{
    const int i = 42;
    Dependency< FakeScope<>, int > dep;
    FakePool< vector<Instance<int> >, i > pool;

    BOOST_CHECK_EQUAL(i, *dep.create(pool));
}

BOOST_AUTO_TEST_CASE(CreateByValue)
{
    const int i = 42;
    Dependency< FakeScope<>, int, int_<i> > dep;
    FakePool< vector0<> > pool;

    BOOST_CHECK_EQUAL(i, dep.create(pool));
}

BOOST_AUTO_TEST_CASE(CreateByScope)
{
    const int i = 42;
    Dependency<FakeScope<i>, int> dep;
    FakePool< vector0<> > pool;

    BOOST_CHECK_EQUAL(i, *dep.create(pool));
}

} // namespace UT
} // namespace Aux
} // namespace Back
} // namespace QDeps

