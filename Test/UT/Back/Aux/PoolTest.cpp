//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "QDeps/Back/Aux/Pool.hpp"

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

template<typename T>
struct Allocator
{
    typedef shared_ptr<T> ResultType;

    Allocator(T* p_object) // non explicit
        : object(p_object)
    { }

    ResultType get()
    {
        return object;
    }

    shared_ptr<T> object;
};

struct TrivialCtor
{
    typedef TrivialCtor ResultType;
};

struct DefaultCtor
{
    typedef DefaultCtor ResultType;
    explicit DefaultCtor(int = 0) { }
};

struct CustomCtor
{
    typedef CustomCtor ResultType;
    explicit CustomCtor(int) { }
};

BOOST_AUTO_TEST_CASE(PoolEmpty)
{
    typedef Pool< vector0<> > Pool;

    Pool pool;

    BOOST_CHECK((equal<vector0<>, Pool::Seq>::value));
    (void)pool;
}

BOOST_AUTO_TEST_CASE(PoolCtorOrder)
{
    typedef Pool< vector<TrivialCtor, DefaultCtor> > Pool;
    DefaultCtor defaultCtor;
    TrivialCtor trivialCtor;

    Pool pool(defaultCtor, trivialCtor);

    BOOST_CHECK((equal<vector<TrivialCtor, DefaultCtor>, Pool::Seq>::value));
    (void)pool;
}

BOOST_AUTO_TEST_CASE(PoolCtorOrderReverse)
{
    typedef Pool< vector<TrivialCtor, DefaultCtor> > Pool;
    DefaultCtor defaultCtor;
    TrivialCtor trivialCtor;

    Pool pool(trivialCtor, defaultCtor);

    BOOST_CHECK((equal<vector<TrivialCtor, DefaultCtor>, Pool::Seq>::value));
    (void)pool;
}

BOOST_AUTO_TEST_CASE(PoolOfPools)
{
    typedef Allocator<TrivialCtor> TrivialCtorType;
    typedef Allocator<DefaultCtor> DefaultCtorType;
    typedef Pool< vector<DefaultCtorType> > Pool1;
    typedef Pool< vector<TrivialCtorType> > Pool2;
    typedef Pool< vector<Pool1, Pool2> > Pool;
    DefaultCtorType defaultCtor(new DefaultCtor);
    TrivialCtorType trivialCtor(new TrivialCtor);

    Pool1 pool1(defaultCtor);
    Pool2 pool2(trivialCtor);
    Pool pool(pool1, pool2);

    BOOST_CHECK_EQUAL(trivialCtor.get(), pool.get<TrivialCtorType>());
    BOOST_CHECK_EQUAL(defaultCtor.get(), pool.get<DefaultCtorType>());
    BOOST_CHECK((equal<vector<DefaultCtorType, TrivialCtorType>, Pool::Seq>::value));
}

BOOST_AUTO_TEST_CASE(PoolGet)
{
    typedef Allocator<CustomCtor> CustomCtorType;
    typedef Allocator<TrivialCtor> TrivialCtorType;
    typedef Allocator<DefaultCtor> DefaultCtorType;

    CustomCtorType customCtor(new CustomCtor(0));
    TrivialCtorType trivialCtor(new TrivialCtor);
    DefaultCtorType defaultCtor(new DefaultCtor);

    Pool< vector<TrivialCtorType, DefaultCtorType, CustomCtorType> > pool(customCtor, trivialCtor, defaultCtor);

    BOOST_CHECK_EQUAL(trivialCtor.get(), pool.get<TrivialCtorType>());
    BOOST_CHECK_EQUAL(customCtor.get(), pool.get<CustomCtorType>());
    BOOST_CHECK_EQUAL(defaultCtor.get(), pool.get<DefaultCtorType>());
}

} // namespace UT
} // namespace Aux
} // namespace Back
} // namespace QDeps

