//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/detail/pool.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>

namespace boost {
namespace di {
namespace detail {

template<typename T>
struct allocator
{
    allocator(T* object) // non explicit
        : object(object)
    { }

    shared_ptr<T> object;
};

struct trivial_ctor
{
    trivial_ctor()
        : i(0)
    { }

    int i;
};

struct default_ctor
{
    explicit default_ctor(int i = 0)
        : i(i)
    { }

    int i;
};

struct custom_ctor
{
    explicit custom_ctor(int i)
        : i(i)
    { }

    int i;
};

struct custom_ctor_other
{
    explicit custom_ctor_other(int i)
        : i(i)
    { }

    int i;
};

BOOST_AUTO_TEST_CASE(empty) {
    typedef pool<mpl::vector0<> > pool_type;

    pool_type pool_;

    BOOST_CHECK((
        mpl::equal<
            mpl::vector0<>
          , pool_type::types
        >::value
    ));

    (void)pool_;
}

BOOST_AUTO_TEST_CASE(ctor_order) {
    typedef pool<
        mpl::vector<
            trivial_ctor
          , default_ctor
        >
    > pool_type;

    default_ctor default_ctor_;
    trivial_ctor trivial_ctor_;

    pool_type pool_(default_ctor_, trivial_ctor_);

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                trivial_ctor
              , default_ctor
            >
          , pool_type::types
        >::value
    ));

    (void)pool_;
}

BOOST_AUTO_TEST_CASE(ctor_order_reverse) {
    typedef pool<
        mpl::vector<
            trivial_ctor
          , default_ctor
        >
    > pool_type;

    default_ctor default_ctor_;
    trivial_ctor trivial_ctor_;

    pool_type pool_(trivial_ctor_, default_ctor_);

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                trivial_ctor
              , default_ctor
            >
          , pool_type::types
        >::value
    ));

    (void)pool_;
}

BOOST_AUTO_TEST_CASE(default_ctor_base) {
    typedef pool<
        mpl::vector<
            trivial_ctor
          , default_ctor
        >
    > pool_type;

    pool_type pool_;

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                trivial_ctor
              , default_ctor
            >
          , pool_type::types
        >::value
    ));

    (void)pool_;
}

BOOST_AUTO_TEST_CASE(get) {
    typedef allocator<custom_ctor> custom_ctor_type;
    typedef allocator<trivial_ctor> trivial_ctor_type;
    typedef allocator<default_ctor> default_ctor_type;
    typedef pool<
        mpl::vector<
            trivial_ctor_type
          , default_ctor_type
          , custom_ctor_type
        >
    > pool_type;

    custom_ctor_type custom_ctor_(new custom_ctor(0));
    trivial_ctor_type trivial_ctor_(new trivial_ctor);
    default_ctor_type default_ctor_(new default_ctor);

    pool_type pool_(custom_ctor_, trivial_ctor_, default_ctor_);

    BOOST_CHECK_EQUAL(
        trivial_ctor_.object
      , pool_.get<trivial_ctor_type>().object
    );
    BOOST_CHECK_EQUAL(
        custom_ctor_.object
      , pool_.get<custom_ctor_type>().object
    );
    BOOST_CHECK_EQUAL(
        default_ctor_.object
      , pool_.get<default_ctor_type>().object
    );
}

BOOST_AUTO_TEST_CASE(pool_of_pools) {
    typedef allocator<trivial_ctor> trivial_ctor_type;
    typedef allocator<default_ctor> default_ctor_type;
    typedef pool<mpl::vector<default_ctor_type> > pool_1_type;
    typedef pool<mpl::vector<trivial_ctor_type> > pool_2_type;
    typedef pool<mpl::vector<pool_1_type, pool_2_type> > pool_type;
    default_ctor_type default_ctor_(new default_ctor);
    trivial_ctor_type trivial_ctor_(new trivial_ctor);

    pool_1_type pool_1_(default_ctor_);
    pool_2_type pool_2_(trivial_ctor_);

    pool_type pool_(pool_1_, pool_2_);

    BOOST_CHECK((
        mpl::equal<
            mpl::vector<
                default_ctor_type
              , trivial_ctor_type
            >
          , pool_type::types
        >::value
    ));
    BOOST_CHECK_EQUAL(
        trivial_ctor_.object
      , pool_.get<trivial_ctor_type>().object
    );
    BOOST_CHECK_EQUAL(
        default_ctor_.object
      , pool_.get<default_ctor_type>().object
    );
}

BOOST_AUTO_TEST_CASE(init_pool_from_other_empty_pool) {
    pool<mpl::vector<> > pool_empty_;
    pool<mpl::vector<default_ctor> > pool_(pool_empty_, init());

    BOOST_CHECK_EQUAL(0, pool_.get<default_ctor>().i);
}

BOOST_AUTO_TEST_CASE(init_pool_from_other_subset_pool) {
    typedef pool<
        mpl::vector<
            trivial_ctor
          , default_ctor
          , custom_ctor
        >
    > pool_all_type;

    typedef pool<
        mpl::vector<
            trivial_ctor
          , custom_ctor
        >
    > pool_subset_type;

    trivial_ctor trivial_ctor_;
    custom_ctor custom_ctor_(42);

    pool_subset_type pool_subset_(trivial_ctor_, custom_ctor_);
    pool_all_type pool_all_(pool_subset_, init());

    BOOST_CHECK_EQUAL(
        trivial_ctor_.i
      , pool_all_.get<trivial_ctor>().i
    );

    BOOST_CHECK_EQUAL(
        0
      , pool_all_.get<default_ctor>().i
    );

    BOOST_CHECK_EQUAL(
        custom_ctor_.i
      , pool_all_.get<custom_ctor>().i
    );
}

struct ignored
{
    typedef void ignore;
};

BOOST_MPL_HAS_XXX_TRAIT_DEF(ignore)

BOOST_AUTO_TEST_CASE(init_pool_from_other_subset_pool_with_ignore) {
    typedef pool<
        mpl::vector<
            trivial_ctor
          , default_ctor
          , custom_ctor
        >
    > pool_all_type;

    typedef pool<
        mpl::vector<
            trivial_ctor
          , custom_ctor
          , ignored
        >
      , has_ignore<mpl::_1>
    > pool_subset_type;

    pool_subset_type pool_subset_(trivial_ctor(), custom_ctor(0), ignored());
    pool_all_type pool_all_(pool_subset_, init());
}

BOOST_AUTO_TEST_CASE(pool_from_pool_of_pools) {
    typedef pool<mpl::vector<custom_ctor> > pool_type;
    typedef pool<mpl::vector<pool_type> > pool_pool_type;
    const int i = 42;

    custom_ctor ctor(i);
    pool_type p1(ctor);
    pool_pool_type p2(p1);

    BOOST_CHECK_EQUAL(i, p2.get<custom_ctor>().i);
}

BOOST_AUTO_TEST_CASE(pool_from_pool_of_pools_many) {
    typedef pool<mpl::vector<custom_ctor, default_ctor> > pool_type;
    typedef pool<mpl::vector<pool_type> > pool_pool_type;
    const int i = 42;

    custom_ctor c(i);
    default_ctor d;
    pool_type p1(c, d);
    pool_pool_type p2(p1);

    BOOST_CHECK_EQUAL(i, p2.get<custom_ctor>().i);
    BOOST_CHECK_EQUAL(0, p2.get<default_ctor>().i);
}

} // namespace detail
} // namespace di
} // namespace boost

