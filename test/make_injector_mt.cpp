//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/make_injector.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

#include "boost/di/concepts.hpp"
#include "boost/di/policies/binding_correctness.hpp"
#include "boost/di/policies/circular_dependencies.hpp"

#include "data.hpp"
#include "contains_all.hpp"

namespace boost {
namespace di {

BOOST_AUTO_TEST_CASE(ctor) {
    using injector_c0 = injector<
        c0if0
    >;

    shared_ptr<c5> c5_ = injector_c0().create<shared_ptr<c5>>();
    BOOST_CHECK_EQUAL(0, c5_->c2_->i);
}

BOOST_AUTO_TEST_CASE(empty) {
    auto injector_empty = make_injector();

    using injector_t = decltype(injector_empty);
    BOOST_CHECK((
        contains_all<
            mpl::vector<>
          , injector_t::policies::type
        >::value
    ));

    BOOST_CHECK_EQUAL(0, injector_empty.create<c3>().i);
}

BOOST_AUTO_TEST_CASE(by_value) {
    const int i = 42;
    using injector_c0 = injector<
        c0if0
    >;

    auto injector_c0_int_ = make_injector(
        injector_c0()
      , bind<int>::to(i)
    );

    shared_ptr<c5> c5_ = injector_c0_int_.create<shared_ptr<c5>>();
    BOOST_CHECK_EQUAL(i, c5_->c2_->i);
}

BOOST_AUTO_TEST_CASE(with_policy) {
    const int i = 42;

    auto injector_ = make_injector(
        bind_int<i>()
      , policies::binding_correctness()
      , policies::circular_dependencies()
    );

    using injector_t = decltype(injector_);
    BOOST_CHECK((
        contains_all<
            mpl::vector<
                policies::binding_correctness
              , policies::circular_dependencies
            >
          , injector_t::policies::type
        >::value
    ));

    BOOST_CHECK_EQUAL(i, injector_.create<c3>().i);
}

BOOST_AUTO_TEST_CASE(with_policy_seperate) {
    const int i = 42;

    auto injector_ = make_injector(
        policies::binding_correctness()
      , bind_int<i>()
      , policies::circular_dependencies()
    );

    using injector_t = decltype(injector_);
    BOOST_CHECK((
        contains_all<
            mpl::vector<
                policies::binding_correctness
              , policies::circular_dependencies
            >
          , injector_t::policies::type
        >::value
    ));

    BOOST_CHECK_EQUAL(i, injector_.create<c3>().i);
}

BOOST_AUTO_TEST_CASE(mix) {
    const int i = 42;
    const double d = 42.0;

    using injector_c0 = injector<
        c0if0
      , bind_int<i>
    >;

    auto injector_c1 = make_injector(
        policies::circular_dependencies()
      , scoped<
            c1
        >()
    );

    auto injector_ = make_injector(
        injector_c0()
      , per_request<
            c2
        >()
      , policies::binding_correctness()
      , injector_c1
      , bind<double>::to(d)
    );

    auto c5_ = injector_.create<shared_ptr<c5>>();

    using injector_t = decltype(injector_);
    BOOST_CHECK((
        contains_all<
            mpl::vector<
                policies::binding_correctness
              , policies::circular_dependencies
            >
          , injector_t::policies::type
        >::value
    ));

    BOOST_CHECK(dynamic_cast<c0if0*>(c5_->if0_.get()));
    BOOST_CHECK_EQUAL(i, c5_->c2_->i);
    BOOST_CHECK_EQUAL(d, c5_->c2_->d);
    BOOST_CHECK_EQUAL(0, c5_->c2_->c);
}

BOOST_AUTO_TEST_CASE(factory) {
    auto injector_ = make_injector(
        bind<if0, if0_factory>()
      , bind<eid>::to(e0)
    );

    BOOST_CHECK(dynamic_cast<c0if0*>(injector_.create<c23>().if0_.get()));
}

} // namespace di
} // namespace boost

