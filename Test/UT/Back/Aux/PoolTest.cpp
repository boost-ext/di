//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <string>
#include <boost/make_shared.hpp>
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

class T1 { };
class T2 { };
class T3 { };
class T4 { };

BOOST_AUTO_TEST_CASE(PoolEmpty)
{
    Pool< vector0<> > pool;
    (void)pool;
}

#if 0
BOOST_AUTO_TEST_CASE(Pool, BasicWithDefaultCtor)
{
    typedef Pool< vector<TrivialCtor, DefaultCtor> > PoolType;

    PoolType l_pool;
    (void)l_pool;
}

BOOST_AUTO_TEST_CASE(Pool, BasicWithDefaultCtorPlusObjects)
{
    typedef Pool< vector<TrivialCtor, DefaultCtor> > PoolType;
    shared_ptr<TrivialCtor> l_trivialCtor(new TrivialCtor);
    shared_ptr<DefaultCtor> l_defaultCtor(new DefaultCtor);

    PoolType l_pool(l_trivialCtor, l_defaultCtor);
    (void)l_pool;
}

BOOST_AUTO_TEST_CASE(Pool, BasicWithDefaultCtorPlusObjectsOrder)
{
    typedef Pool< vector<TrivialCtor, DefaultCtor> > PoolType;
    shared_ptr<TrivialCtor> l_trivialCtor(new TrivialCtor);
    shared_ptr<DefaultCtor> l_defaultCtor(new DefaultCtor);

    PoolType l_pool(l_defaultCtor, l_trivialCtor);
    (void)l_pool;
}

BOOST_AUTO_TEST_CASE(Pool, BasicWithCustomCtor)
{
    typedef Pool< vector<TrivialCtor, DefaultCtor, CustomCtor> > PoolType;
    shared_ptr<CustomCtor> l_customCtor(new CustomCtor(0));

    PoolType l_pool(l_customCtor);
    (void)l_pool;
}

BOOST_AUTO_TEST_CASE(Pool, BasicWithCustomCtorPlusOthers)
{
    typedef Pool< vector<TrivialCtor, DefaultCtor, CustomCtor> > PoolType;
    shared_ptr<CustomCtor> l_customCtor(new CustomCtor(0));
    shared_ptr<TrivialCtor> l_trivialCtor(new TrivialCtor);
    shared_ptr<DefaultCtor> l_defaultCtor(new DefaultCtor);

    PoolType l_pool(l_defaultCtor, l_customCtor, l_trivialCtor);
    (void)l_pool;
}

BOOST_AUTO_TEST_CASE(Pool, BasicStackAllocator)
{
    typedef Pool< vector<TrivialCtor>, Allocator::Stack> PoolType;

    PoolType l_pool;
    (void)l_pool;
}

BOOST_AUTO_TEST_CASE(Pool, BasicStackAllocatorMore)
{
    typedef Pool< vector<TrivialCtor, DefaultCtor, CustomCtor>, Allocator::Stack> PoolType;
    CustomCtor l_customCtor(0);
    DefaultCtor l_defaultCtor;
    TrivialCtor l_trivialCtor;

    PoolType l_pool(l_customCtor, l_defaultCtor, l_trivialCtor);
    (void)l_pool;
}

BOOST_AUTO_TEST_CASE(Pool, BasicHeapAllocator)
{
    typedef Pool< vector<TrivialCtor>, Allocator::Heap> PoolType;

    PoolType l_pool;
    (void)l_pool;
}

BOOST_AUTO_TEST_CASE(Pool, BasicSmartAllocator)
{
    typedef Pool< vector<TrivialCtor>, Allocator::Smart> PoolType;

    PoolType l_pool;
    (void)l_pool;
}

BOOST_AUTO_TEST_CASE(Pool, SmartAllocatorMix)
{
    typedef Pool< vector<TrivialCtor, DefaultCtor, CustomCtor>, Allocator::Smart> PoolType;

    TrivialCtor l_trivialCtor;
    const TrivialCtor& l_crTrivialCtor = l_trivialCtor;
    PoolType l_pool(CustomCtor(0), make_shared<DefaultCtor>(), l_crTrivialCtor);
    (void)l_pool;
}

BOOST_AUTO_TEST_CASE(Pool, Get)
{
    typedef Pool<vector<TrivialCtor, DefaultCtor, CustomCtor>, Allocator::Heap> PoolType;
    shared_ptr<CustomCtor> l_customCtor(new CustomCtor(0));
    shared_ptr<TrivialCtor> l_trivialCtor(new TrivialCtor);
    shared_ptr<DefaultCtor> l_defaultCtor(new DefaultCtor);
    PoolType l_pool(l_customCtor, l_defaultCtor, l_trivialCtor);

    BOOST_CHECK_EQUAL(l_trivialCtor, l_pool.get<TrivialCtor>());
    BOOST_CHECK_EQUAL(l_customCtor, l_pool.get<CustomCtor>());
    BOOST_CHECK_EQUAL(l_defaultCtor, l_pool.get<DefaultCtor>());
}

BOOST_AUTO_TEST_CASE(Pool, SmartAllocatorMixAcquire)
{
    typedef Pool< vector<DefaultCtor, CustomCtor, std::string, int, float>, Allocator::Smart> PoolType;

    const int i = 42;
    const float f = 87.0;
    const char* s= "string";
    shared_ptr<CustomCtor> l_customCtor(new CustomCtor(0));
    DefaultCtor l_defaultCtor;
    PoolType l_pool(i, l_customCtor, std::string(s), l_defaultCtor, make_shared<float>(f));

    BOOST_CHECK_EQUAL(l_customCtor.get(), &l_pool.acquire<CustomCtor>());
    BOOST_CHECK_EQUAL(i, l_pool.acquire<int>());
    BOOST_CHECK_EQUAL(f, l_pool.acquire<float>());
    BOOST_CHECK_EQUAL(&l_defaultCtor, &l_pool.acquire<DefaultCtor>());
    BOOST_CHECK_EQUAL(s, l_pool.acquire<std::string>());
}

BOOST_AUTO_TEST_CASE(Ctor, BasicDefault)
{
    Basic l_pool;

    BOOST_CHECK_EQUAL(0, l_pool.pool.acquire<int>());
    BOOST_CHECK_EQUAL("", l_pool.pool.acquire<std::string>());
}

BOOST_AUTO_TEST_CASE(Ctor, BasicWithParameters)
{
    const int Int = 42;
    const std::string String = "string";
    Basic l_pool(boost::make_shared<int>(Int), boost::make_shared<std::string>(String));

    BOOST_CHECK_EQUAL(Int, l_pool.pool.acquire<int>());
    BOOST_CHECK_EQUAL(String, l_pool.pool.acquire<std::string>());
}

BOOST_AUTO_TEST_CASE(Ctor, BasicWithParametersOrder)
{
    const int Int = 42;
    const std::string String = "string";
    Basic l_pool(boost::make_shared<std::string>(String), boost::make_shared<int>(Int));

    BOOST_CHECK_EQUAL(Int, l_pool.pool.acquire<int>());
    BOOST_CHECK_EQUAL(String, l_pool.pool.acquire<std::string>());
}

BOOST_AUTO_TEST_CASE(Ctor, Complex)
{
    const int Int = 0;
    const std::string String = "complex";
    Complex l_pool(boost::make_shared<std::string>(String), boost::make_shared<int>(Int));

    BOOST_CHECK_EQUAL(Int, l_pool.pool.acquire<int>());
    BOOST_CHECK_EQUAL(String, l_pool.pool.acquire<std::string>());
    BOOST_CHECK_EQUAL(42, l_pool.i);
    BOOST_CHECK_EQUAL("s", l_pool.s);
    BOOST_CHECK_EQUAL(42.0, l_pool.d);
}
#endif

} // namespace UT
} // namespace Aux
} // namespace Back
} // namespace QDeps

