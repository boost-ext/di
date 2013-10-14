//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/dependency.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/make_shared.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost {
namespace di {
namespace concepts {

template<int value = 0>
struct fake_scope
{
    template<typename T, typename U>
    struct scope
    {
        typedef shared_ptr<T> result_type;

        result_type create() {
            return result_type(make_shared<T>(value));
        }
    };
};

struct other_fake_scope { };

template<typename T, typename>
struct fake_fixed_value
{ };

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
    BOOST_CHECK((is_same<
        mpl::lambda<
            mpl::and_<
                is_same<int, mpl::_1>
              , is_same<void, mpl::_2>
            >
        >::type
      , dependency_t::bind
    >::value));
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
            >
          , dependency<
                mpl::_1
              , int
              , int
              , mpl::vector0<>
              , is_same<mpl::_1, int>
            >::rebind<other_fake_scope>::other
        >::value
    ));
}

} // namespace concepts
} // namespace di
} // namespace boost

