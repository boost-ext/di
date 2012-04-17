//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "di/back/aux/pool.hpp"

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

template<typename T>
struct Allocator
{
    typedef shared_ptr<T> result_type;

    Allocator(T* object) // non explicit
        : object(object)
    { }

    result_type get() const
    {
        return object;
    }

    shared_ptr<T> object;
};

struct Trivialctor
{
    typedef Trivialctor result_type;
};

struct Defaultctor
{
    typedef Defaultctor result_type;
    explicit Defaultctor(int = 0) { }
};

struct Customctor
{
    typedef Customctor result_type;
    explicit Customctor(int) { }
};

BOOST_AUTO_TEST_CASE(poolEmpty)
{
    typedef pool< vector0<> > pool_type;

    pool_type pool;

    BOOST_CHECK((equal<vector0<>, pool_type::seq>::value));
    (void)pool;
}

BOOST_AUTO_TEST_CASE(poolctorOrder)
{
    typedef pool< vector<Trivialctor, Defaultctor> > pool_type;
    Defaultctor defaultctor;
    Trivialctor trivialctor;

    pool_type pool(defaultctor, trivialctor);

    BOOST_CHECK((equal<vector<Trivialctor, Defaultctor>, pool_type::seq>::value));
    (void)pool;
}

BOOST_AUTO_TEST_CASE(poolctorOrderReverse)
{
    typedef pool< vector<Trivialctor, Defaultctor> > pool_type;
    Defaultctor defaultctor;
    Trivialctor trivialctor;

    pool_type pool(trivialctor, defaultctor);

    BOOST_CHECK((equal<vector<Trivialctor, Defaultctor>, pool_type::seq>::value));
    (void)pool;
}

BOOST_AUTO_TEST_CASE(poolOfpools)
{
    typedef Allocator<Trivialctor> TrivialctorType;
    typedef Allocator<Defaultctor> DefaultctorType;
    typedef pool< vector<DefaultctorType> > pool_type_1;
    typedef pool< vector<TrivialctorType> > pool_type_2;
    typedef pool< vector<pool_type_1, pool_type_2> > pool_type;
    DefaultctorType defaultctor(new Defaultctor);
    TrivialctorType trivialctor(new Trivialctor);

    pool_type_1 pool_1(defaultctor);
    pool_type_2 pool_2(trivialctor);
    pool_type pool(pool_1, pool_2);

    BOOST_CHECK_EQUAL(trivialctor.get(), pool.get<TrivialctorType>());
    BOOST_CHECK_EQUAL(defaultctor.get(), pool.get<DefaultctorType>());
    BOOST_CHECK((equal<vector<DefaultctorType, TrivialctorType>, pool_type::seq>::value));
}

BOOST_AUTO_TEST_CASE(poolGet)
{
    typedef Allocator<Customctor> CustomctorType;
    typedef Allocator<Trivialctor> TrivialctorType;
    typedef Allocator<Defaultctor> DefaultctorType;
    typedef pool< vector<TrivialctorType, DefaultctorType, CustomctorType> > pool_type;

    CustomctorType customctor(new Customctor(0));
    TrivialctorType trivialctor(new Trivialctor);
    DefaultctorType defaultctor(new Defaultctor);

    pool_type pool(customctor, trivialctor, defaultctor);

    BOOST_CHECK_EQUAL(trivialctor.get(), pool.get<TrivialctorType>());
    BOOST_CHECK_EQUAL(customctor.get(), pool.get<CustomctorType>());
    BOOST_CHECK_EQUAL(defaultctor.get(), pool.get<DefaultctorType>());
}

} // namespace ut
} // namespace aux
} // namespace back
} // namespace di

