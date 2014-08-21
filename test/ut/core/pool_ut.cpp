//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/core/pool.hpp"

#include "boost/di/aux_/memory.hpp"
#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {
namespace core {

template<typename T>
struct allocator {
    allocator(T* object) // non explicit
        : object(object)
    { }

    aux::shared_ptr<T> object;
};

struct trivial_ctor {
    trivial_ctor()
        : i(0)
    { }

    int i = 0;
};

struct default_ctor {
    explicit default_ctor(int i = 0)
        : i(i)
    { }

    int i = 0;
};

struct custom_ctor {
    explicit custom_ctor(int i)
        : i(i)
    { }

    int i = 0;
};

struct custom_ctor_other {
    explicit custom_ctor_other(int i)
        : i(i)
    { }

    int i = 0;
};

BOOST_AUTO_TEST_CASE(empty) {
    using pool_type = pool<type_list<>>;

    pool_type pool_;
    (void)pool_;
}

BOOST_AUTO_TEST_CASE(ctor_order) {
    using pool_type = pool<
        type_list<
            default_ctor
          , trivial_ctor
        >
    >;

    default_ctor default_ctor_;
    trivial_ctor trivial_ctor_;

    pool_type pool_(default_ctor_, trivial_ctor_);
    (void)pool_;
}

BOOST_AUTO_TEST_CASE(default_ctor_base) {
    using pool_type = pool<
        type_list<
            trivial_ctor
          , default_ctor
        >
    >;

    pool_type pool_;
    (void)pool_;
}

BOOST_AUTO_TEST_CASE(get) {
    using custom_ctor_type = allocator<custom_ctor>;
    using trivial_ctor_type = allocator<trivial_ctor>;
    using default_ctor_type = allocator<default_ctor>;
    using pool_type = pool<
        type_list<
            custom_ctor_type
          , trivial_ctor_type
          , default_ctor_type
        >
    >;

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
    using trivial_ctor_type = allocator<trivial_ctor>;
    using default_ctor_type = allocator<default_ctor>;
    using pool_1_type = pool<type_list<default_ctor_type>>;
    using pool_2_type = pool<type_list<trivial_ctor_type>>;
    using pool_type = pool<type_list<pool_1_type, pool_2_type>>;
    default_ctor_type default_ctor_(new default_ctor);
    trivial_ctor_type trivial_ctor_(new trivial_ctor);

    pool_1_type pool_1_(default_ctor_);
    pool_2_type pool_2_(trivial_ctor_);

    pool_type pool_(pool_1_, pool_2_);

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
    pool<type_list<>> pool_empty_;
    pool<type_list<default_ctor>> pool_(pool_empty_, init());

    BOOST_CHECK_EQUAL(0, pool_.get<default_ctor>().i);
}

BOOST_AUTO_TEST_CASE(init_pool_from_other_subset_pool) {
    using pool_all_type = pool<
        type_list<
            trivial_ctor
          , default_ctor
          , custom_ctor
        >
    >;

    using pool_subset_type = pool<
        type_list<
            trivial_ctor
          , custom_ctor
        >
    >;

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

BOOST_AUTO_TEST_CASE(pool_from_pool_of_pools) {
    using pool_type = pool<type_list<custom_ctor>>;
    using pool_pool_type = pool<type_list<pool_type>> ;
    const int i = 42;

    custom_ctor ctor(i);
    pool_type p1(ctor);
    pool_pool_type p2(p1);

    BOOST_CHECK_EQUAL(i, p2.get<custom_ctor>().i);
}

BOOST_AUTO_TEST_CASE(pool_from_pool_of_pools_many) {
    using pool_type = pool<type_list<custom_ctor, default_ctor>>;
    using pool_pool_type = pool<type_list<pool_type>>;
    const int i = 42;

    custom_ctor c(i);
    default_ctor d;
    pool_type p1(c, d);
    pool_pool_type p2(p1);

    BOOST_CHECK_EQUAL(i, p2.get<custom_ctor>().i);
    BOOST_CHECK_EQUAL(0, p2.get<default_ctor>().i);
}

template<typename... Ts>
struct base : Ts...
{ };

BOOST_AUTO_TEST_CASE(pool_flatten) {
    using pool_flatten_type = pool<type_list<trivial_ctor, custom_ctor, default_ctor>>;
    using pool_type = pool<type_list<custom_ctor, base<trivial_ctor, default_ctor>>>;
    const int i = 42;

    custom_ctor c(i);
    default_ctor d;
    trivial_ctor t;
    base<trivial_ctor, default_ctor> b;

    pool_type p1(c, b);
    pool_flatten_type p2(p1, init());

    BOOST_CHECK_EQUAL(i, p2.get<custom_ctor>().i);
    BOOST_CHECK_EQUAL(0, p2.get<default_ctor>().i);
    BOOST_CHECK_EQUAL(0, p2.get<trivial_ctor>().i);
}

} // namespace core
} // namespace di
} // namespace boost

