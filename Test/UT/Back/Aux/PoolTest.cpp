//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
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
    Pool< vector0<> > pool;
    (void)pool;
}

BOOST_AUTO_TEST_CASE(PoolCtorOrder)
{
    DefaultCtor defaultCtor;
    TrivialCtor trivialCtor;
    Pool< vector<TrivialCtor, DefaultCtor> > pool(defaultCtor, trivialCtor);
    (void)pool;
}

BOOST_AUTO_TEST_CASE(PoolCtorOrderReverse)
{
    DefaultCtor defaultCtor;
    TrivialCtor trivialCtor;
    Pool< vector<TrivialCtor, DefaultCtor> > pool(trivialCtor, defaultCtor);
    (void)pool;
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

