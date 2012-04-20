//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/make_shared.hpp>
#include <boost/mpl/mpl::vector.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_same.hpp>
#include "boost/di/aux/instance.hpp"
#include "boost/di/aux/dependency.hpp"

namespace boost {
namespace di {
namespace aux {
namespace ut {

template<typename T, typename>
struct fake_result
{ };

template<typename TSequence, int value = 0>
struct fake_pool
{
    typedef TSequence sequence;

    template<typename T>
    struct result_type
    {
        typedef shared_ptr<int> type;
    };

    template<typename T>
    typename result_type<int>::type get() const
    {
       return make_shared<int>(value);
    }
};

template<int value = 0>
struct fake_scope
{
    template<typename T>
    struct scope
    {
        typedef shared_ptr<T> result_type;
        result_type create() { return make_shared<T>(value); }
    };
};

struct other_fake_scope { };

BOOST_AUTO_TEST_CASE(dependency_default)
{
    typedef dependency<fake_scope<>, int> dependency_t;

    BOOST_CHECK((is_same<mpl::vector0<>, dependency_t::context>::value));
    BOOST_CHECK((is_same<is_same<mpl::_1, int>, dependency_t::bind>::value));
}

BOOST_AUTO_TEST_CASE(dependency_rebind_scope)
{
    BOOST_CHECK((
        is_same<
            dependency<
                other_fake_scope
              , int
              , int
              , mpl::vector0<>
              , is_same<mpl::_1, int>
              , fake_result
            >
          , dependency<
                mpl::_1
              , int
              , int
              , mpl::vector0<>
              , is_same<mpl::_1, int>
              , fake_result
            >::rebind<other_fake_scope>::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(dependency_rebind_type)
{
    BOOST_CHECK((
        is_same<
            dependency<
                void
              , double
              , int
              , mpl::vector0<>
              , is_same<mpl::_1, int>
              , fake_result
            >
          , dependency<
                void
              , mpl::_1
              , mpl::_2
              , mpl::vector0<>
              , is_same<mpl::_1, int>
              , fake_result
            >::rebind<double, int>::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(dependency_create_by_pool)
{
    const int i = 42;
    dependency< fake_scope<>, int > dependency_;
    fake_pool< mpl::vector<instance<int> >, i > pool_;

    BOOST_CHECK_EQUAL(i, *dependency_.create(pool_));
}

BOOST_AUTO_TEST_CASE(dependency_create_by_value)
{
    const int i = 42;
    dependency< fake_scope<>, int, int_<i> > dependency_;
    fake_pool< mpl::vector0<> > pool_;

    BOOST_CHECK_EQUAL(i, dependency_.create(pool_));
}

BOOST_AUTO_TEST_CASE(dependency_create_by_scope)
{
    const int i = 42;
    dependency<fake_scope<i>, int> dependency_;
    fake_pool< mpl::vector0<> > pool_;

    BOOST_CHECK_EQUAL(i, *dependency_.create(pool_));
}

} // namespace ut
} // namespace aux
} // namespace di
} // namespace boost

