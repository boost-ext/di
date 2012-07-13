//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/detail/dependency.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/make_shared.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_same.hpp>

#include "boost/di/scopes/external.hpp"

namespace boost {
namespace di {
namespace detail {

template<int value = 0>
struct fake_scope
{
    template<typename T, typename U>
    struct scope
    {
        typedef scopes::convertible_any<U> result_type;

        result_type create() {
            return result_type(make_shared<T>(value));
        }
    };
};

struct other_fake_scope { };

template<typename T, typename>
struct fake_fixed_value
{ };

template<
    typename T
  , typename TContext = mpl::vector0<>
  , typename = void
>
class fake_external { };

template<typename TTypes, int value = 0>
struct fake_pool
{
    typedef TTypes types;

    template<typename>
    int get() const {
        return value;
    }
};

BOOST_AUTO_TEST_CASE(default_value) {
    typedef dependency<fake_scope<>, int> dependency_t;

    BOOST_CHECK((is_same<mpl::vector0<>, dependency_t::context>::value));
    BOOST_CHECK((is_same<is_same<mpl::_1, int>, dependency_t::bind>::value));
}

BOOST_AUTO_TEST_CASE(rebind_scope) {
    BOOST_CHECK((
        is_same<
            dependency<
                other_fake_scope
              , int
              , int
              , mpl::vector0<>
              , is_same<mpl::_1, int>
              , fake_external
            >
          , dependency<
                mpl::_1
              , int
              , int
              , mpl::vector0<>
              , is_same<mpl::_1, int>
              , fake_external
            >::rebind<other_fake_scope>::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(rebind_type) {
    BOOST_CHECK((
        is_same<
            dependency<
                void
              , double
              , int
              , mpl::vector0<>
              , is_same<mpl::_1, int>
              , fake_external
            >
          , dependency<
                void
              , mpl::_1
              , mpl::_2
              , mpl::_3
              , is_same<mpl::_1, int>
              , fake_external
            >::rebind<double, int, mpl::vector0<> >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(create_by_pool) {
    const int i = 42;
    dependency<fake_scope<>, int> dependency_;
    fake_pool<mpl::vector<scopes::convertible_any<int> >, i> pool_;

    BOOST_CHECK_EQUAL(i, static_cast<int>(dependency_.create(pool_)));
}

BOOST_AUTO_TEST_CASE(create_by_scope) {
    const int i = 42;
    dependency<fake_scope<i>, int> dependency_;
    fake_pool<mpl::vector0<> > pool_;

    BOOST_CHECK_EQUAL(i, static_cast<int>(dependency_.create(pool_)));
}

} // namespace detail
} // namespace di
} // namespace boost

