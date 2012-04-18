//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/make_shared.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_same.hpp>
#include "di/back/aux/instance.hpp"
#include "di/back/aux/dependency.hpp"

namespace di
{
namespace back
{
namespace aux
{
namespace ut
{

using namespace boost;
using namespace boost::mpl;

template<typename T, typename> struct FakeResult { };

template<typename TSeq, int value = 0> struct Fakepool
{
    typedef TSeq seq;

    template<typename T> struct result_type
    {
        typedef shared_ptr<int> type;
    };

    template<typename T> typename result_type<int>::type get() const
    {
       return make_shared<int>(value);
    }
};

template<int value = 0>
struct fake_scope
{
    template<typename T>
    struct scope
    {
        typedef shared_ptr<T> result_type;
        result_type create() { return make_shared<T>(value); }
    };
};

struct Otherfake_scope { };

BOOST_AUTO_TEST_CASE(Default)
{
    typedef dependency<fake_scope<>, int> Dep;

    BOOST_CHECK((is_same<vector0<>, Dep::context>::value));
    BOOST_CHECK((is_same<is_same<_1, int>, Dep::bind>::value));
}

BOOST_AUTO_TEST_CASE(rebindscope)
{
    BOOST_CHECK((
        is_same
        <
            dependency
            <
                Otherfake_scope,
                int,
                int,
                vector0<>,
                is_same<_1, int>,
                FakeResult
            >,
            dependency
            <
                _1,
                int,
                int,
                vector0<>,
                is_same<_1, int>,
                FakeResult
            >::rebind<Otherfake_scope>::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(rebindType)
{
    BOOST_CHECK((
        is_same
        <
            dependency
            <
                void,
                double,
                int,
                vector0<>,
                is_same<_1, int>,
                FakeResult
            >,
            dependency
            <
                void,
                _1,
                _2,
                vector0<>,
                is_same<_1, int>,
                FakeResult
            >::rebind<double, int>::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(CreateBypool)
{
    const int i = 42;
    dependency< fake_scope<>, int > dep;
    Fakepool< vector<instance<int> >, i > pool;

    BOOST_CHECK_EQUAL(i, *dep.create(pool));
}

BOOST_AUTO_TEST_CASE(CreateByvalue)
{
    const int i = 42;
    dependency< fake_scope<>, int, int_<i> > dep;
    Fakepool< vector0<> > pool;

    BOOST_CHECK_EQUAL(i, dep.create(pool));
}

BOOST_AUTO_TEST_CASE(CreateByscope)
{
    const int i = 42;
    dependency<fake_scope<i>, int> dep;
    Fakepool< vector0<> > pool;

    BOOST_CHECK_EQUAL(i, *dep.create(pool));
}

} // namespace ut
} // namespace aux
} // namespace back
} // namespace di

