//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/make_shared.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_same.hpp>
#include "boost/di/aux_/instance.hpp"
#include "boost/di/aux_/dependency.hpp"

namespace boost {
namespace di {
namespace aux_ {

template<int value = 0>
struct fake_scope
{
    template<typename T>
    struct scope
    {
        shared_ptr<T> create() { return make_shared<T>(value); }
    };
};

struct other_fake_scope { };

template<typename T, typename>
struct fake_result
{ };

template<
    typename T
  , typename TContext = mpl::vector0<>
  , typename = void
>
class fake_instance { };

template<typename TScope, typename TDest>
class fake_converter
{
public:
    template<typename TSrc>
    static TDest execute(TSrc src) { return src; }
};

template<typename TExternals, int value = 0>
struct fake_pool
{
    typedef TExternals externals;

    template<typename T>
    shared_ptr<int> get() const {
        return make_shared<int>(value);
    }
};

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
              , fake_instance
              , fake_converter
            >
          , dependency<
                mpl::_1
              , int
              , int
              , mpl::vector0<>
              , is_same<mpl::_1, int>
              , fake_result
              , fake_instance
              , fake_converter
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
              , fake_instance
              , fake_converter
            >
          , dependency<
                void
              , mpl::_1
              , mpl::_2
              , mpl::vector0<>
              , is_same<mpl::_1, int>
              , fake_result
              , fake_instance
              , fake_converter
            >::rebind<double, int>::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(dependency_create_by_pool)
{
    const int i = 42;
    dependency<fake_scope<>, int > dependency_;
    fake_pool<mpl::vector<instance<int> >, i > pool_;

    BOOST_CHECK_EQUAL(i, dependency_.create<int>(pool_));
}

BOOST_AUTO_TEST_CASE(dependency_create_by_value)
{
    const int i = 42;
    dependency<fake_scope<>, int, mpl::int_<i> > dependency_;
    fake_pool<mpl::vector0<> > pool_;

    BOOST_CHECK_EQUAL(i, dependency_.create<int>(pool_));
}

BOOST_AUTO_TEST_CASE(dependency_create_by_scope)
{
    const int i = 42;
    dependency<fake_scope<i>, int> dependency_;
    fake_pool<mpl::vector0<> > pool_;

    BOOST_CHECK_EQUAL(i, dependency_.create<int>(pool_));
}

} // namespace aux_
} // namespace di
} // namespace boost

