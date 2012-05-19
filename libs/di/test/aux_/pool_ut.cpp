//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/aux_/pool.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>

namespace boost {
namespace di {
namespace aux_ {

template<typename T>
struct allocator
{
    allocator(T* object) // non explicit
        : object(object)
    { }

    shared_ptr<T> get() const {
        return object;
    }

    shared_ptr<T> object;
};

struct trivial_ctor
{
    int get() const { return 0; }
};

struct default_ctor
{
    explicit default_ctor(int i = 0)
        : i(i)
    { }

    int get() const { return i; }

    int i;
};

struct custom_ctor
{
    explicit custom_ctor(int i)
        : i(i)
    { }

    int get() const { return i; }

    int i;
};

BOOST_AUTO_TEST_CASE(empty)
{
    typedef pool<mpl::vector0<> > pool_t;

    pool_t pool_;

    BOOST_CHECK((
        mpl::equal<
            mpl::vector0<>
          , pool_t::externals
        >::value
    ));

    (void)pool_;
}

BOOST_AUTO_TEST_CASE(ctor_order)
{
    typedef pool<mpl::vector<trivial_ctor, default_ctor> > pool_t;
    default_ctor default_ctor_;
    trivial_ctor trivial_ctor_;

    pool_t pool_(default_ctor_, trivial_ctor_);

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                trivial_ctor
              , default_ctor
            >
          , pool_t::externals
        >::value
    ));

    (void)pool_;
}

BOOST_AUTO_TEST_CASE(ctor_order_reverse)
{
    typedef pool<mpl::vector<trivial_ctor, default_ctor> > pool_t;
    default_ctor default_ctor_;
    trivial_ctor trivial_ctor_;

    pool_t pool_(trivial_ctor_, default_ctor_);

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                trivial_ctor
              , default_ctor
            >
          , pool_t::externals
        >::value
    ));

    (void)pool_;
}

BOOST_AUTO_TEST_CASE(default_ctor_base)
{
    typedef pool<mpl::vector<trivial_ctor, default_ctor> > pool_t;

    pool_t pool_;

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                trivial_ctor
              , default_ctor
            >
          , pool_t::externals
        >::value
    ));

    (void)pool_;
}

BOOST_AUTO_TEST_CASE(get)
{
    typedef allocator<custom_ctor> custom_ctor_t;
    typedef allocator<trivial_ctor> trivial_ctor_t;
    typedef allocator<default_ctor> default_ctor_t;
    typedef pool<mpl::vector<trivial_ctor_t, default_ctor_t, custom_ctor_t> > pool_t;

    custom_ctor_t custom_ctor_(new custom_ctor(0));
    trivial_ctor_t trivial_ctor_(new trivial_ctor);
    default_ctor_t default_ctor_(new default_ctor);

    pool_t pool_(custom_ctor_, trivial_ctor_, default_ctor_);

    BOOST_CHECK_EQUAL(trivial_ctor_.get(), pool_.get<trivial_ctor_t>());
    BOOST_CHECK_EQUAL(custom_ctor_.get(), pool_.get<custom_ctor_t>());
    BOOST_CHECK_EQUAL(default_ctor_.get(), pool_.get<default_ctor_t>());
}

BOOST_AUTO_TEST_CASE(of_pools)
{
    typedef allocator<trivial_ctor> trivial_ctor_t;
    typedef allocator<default_ctor> default_ctor_t;
    typedef pool<mpl::vector<default_ctor_t> > pool_1_t;
    typedef pool<mpl::vector<trivial_ctor_t> > pool_2_t;
    typedef pool<mpl::vector<pool_1_t, pool_2_t> > pool_t;
    default_ctor_t default_ctor_(new default_ctor);
    trivial_ctor_t trivial_ctor_(new trivial_ctor);

    pool_1_t pool_1_(default_ctor_);
    pool_2_t pool_2_(trivial_ctor_);

    pool_t pool_(pool_1_, pool_2_);

    BOOST_CHECK_EQUAL(trivial_ctor_.get(), pool_.get<trivial_ctor_t>());
    BOOST_CHECK_EQUAL(default_ctor_.get(), pool_.get<default_ctor_t>());
    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                default_ctor_t
              , trivial_ctor_t
            >
          , pool_t::externals
        >::value
    ));
}

} // namespace aux_
} // namespace di
} // namespace boost

