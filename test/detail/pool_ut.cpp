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
{ };

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

BOOST_AUTO_TEST_CASE(same_arg_for_all_types) {
    const int i = 42;
    pool<mpl::vector<custom_ctor, custom_ctor_other> > pool_(cref(i));

    BOOST_CHECK_EQUAL(i, pool_.get<custom_ctor>().i);
    BOOST_CHECK_EQUAL(i, pool_.get<custom_ctor_other>().i);
}

} // namespace detail
} // namespace di
} // namespace boost

