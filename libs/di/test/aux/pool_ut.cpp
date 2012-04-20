//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "boost/di/aux/pool.hpp"

namespace boost {
namespace di {
namespace aux {
namespace test {

template<typename T>
struct allocator
{
    typedef shared_ptr<T> result_type;

    allocator(T* object) // non explicit
        : object(object)
    { }

    result_type get() const
    {
        return object;
    }

    shared_ptr<T> object;
};

struct trivial_ctor
{
    typedef trivial_ctor result_type;
};

struct default_ctor
{
    typedef default_ctor result_type;
    explicit default_ctor(int = 0) { }
};

struct custom_ctor
{
    typedef custom_ctor result_type;
    explicit custom_ctor(int) { }
};

BOOST_AUTO_TEST_CASE(pool_empty)
{
    typedef pool< mpl::vector0<> > pool_t;

    pool_t pool_;

    BOOST_CHECK((equal<mpl::vector0<>, pool_t::sequence>::value));
    (void)pool_;
}

BOOST_AUTO_TEST_CASE(pool_ctor_order)
{
    typedef pool< mpl::vector<trivial_ctor, default_ctor> > pool_t;
    default_ctor default_ctor_;
    trivial_ctor trivial_ctor_;

    pool_t pool_(default_ctor_, trivial_ctor_);

    BOOST_CHECK((equal<mpl::vector<trivial_ctor, default_ctor>, pool_t::sequence>::value));
    (void)pool_;
}

BOOST_AUTO_TEST_CASE(pool_ctor_order_reverse)
{
    typedef pool< mpl::vector<trivial_ctor, default_ctor> > pool_t;
    default_ctor default_ctor_;
    trivial_ctor trivial_ctor_;

    pool_t pool_(trivial_ctor_, default_ctor_);

    BOOST_CHECK((equal<mpl::vector<trivial_ctor, default_ctor>, pool_t::sequence>::value));
    (void)pool_;
}

BOOST_AUTO_TEST_CASE(pool_of_pools)
{
    typedef allocator<trivial_ctor> trivial_ctor_t;
    typedef allocator<default_ctor> default_ctor_t;
    typedef pool< mpl::vector<default_ctor_t> > pool_1_t;
    typedef pool< mpl::vector<trivial_ctor_t> > pool_2_t;
    typedef pool< mpl::vector<pool_1_t, pool_2_t> > pool_t;
    default_ctor_t default_ctor_(new default_ctor);
    trivial_ctor_t trivial_ctor_(new trivial_ctor);

    pool_t pool_(pool_1_t(default_ctor_), pool_2_t(trivial_ctor_));

    BOOST_CHECK_EQUAL(trivial_ctor_.get(), pool_.get<trivial_ctor_t>());
    BOOST_CHECK_EQUAL(default_ctor_.get(), pool_.get<default_ctor_t>());
    BOOST_CHECK((equal<mpl::vector<default_ctor_t, trivial_ctor_t>, pool_t::sequence>::value));
}

BOOST_AUTO_TEST_CASE(pool_get)
{
    typedef allocator<custom_ctor> custom_ctor_t;
    typedef allocator<trivial_ctor> trivial_ctor_t;
    typedef allocator<default_ctor> default_ctor_t;
    typedef pool< mpl::vector<trivial_ctor_t, default_ctor_t, custom_ctor_t> > pool_t;

    custom_ctor_t custom_ctor_(new custom_ctor(0));
    trivial_ctor_t trivial_ctor_(new trivial_ctor);
    default_ctor_t default_ctor_(new default_ctor);

    pool_t pool_(custom_ctor_, trivial_ctor_, default_ctor_);

    BOOST_CHECK_EQUAL(trivial_ctor_.get(), pool_.get<trivial_ctor_t>());
    BOOST_CHECK_EQUAL(custom_ctor_.get(), pool_.get<custom_ctor_t>());
    BOOST_CHECK_EQUAL(default_ctor_.get(), pool_.get<default_ctor_t>());
}

} // namespace test
} // namespace aux
} // namespace di
} // namespace boost

