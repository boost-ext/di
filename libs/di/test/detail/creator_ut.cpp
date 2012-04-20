//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/mpl/mpl::vector.hpp>
#include <boost/type_traits/is_same.hpp>
#include "boost/di/detail/creator.hpp"

namespace boost {
namespace di {
namespace detail {
namespace test {

class fake_pool { };

template<typename T, T value>
struct dependency
{
    typedef T given;
    typedef T expected;
    typedef mpl::vector0<> context;
    typedef void scope;
    typedef is_same<_1, T> bind;
    typedef mpl::vector0<> ctor;

    template<typename TPool>
    struct result_type
    {
        typedef T type;
    };

    T create(const fake_pool&) { return value; }
};

template<typename T>
struct entries
    : T
{ };

BOOST_AUTO_TEST_CASE(creator_simple)
{
    const int i = 42;
    typedef dependency<int, i> dependency_t;
    fake_pool pool_;
    entries<dependency_t> entries_;

    BOOST_CHECK_EQUAL(i, (
        creator<
            mpl::vector<dependency_t>
          , fake_pool
        >::execute<int, mpl::vector0<> >(entries_, pool_)
    ));
}

} // namespace test
} // namespace detail
} // namespace di
} // namespace boost

