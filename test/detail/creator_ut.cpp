//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/detail/creator.hpp"

#include <map>
#include <typeinfo>
#include <boost/any.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>

#include "fake_pool.hpp"
#include "fake_binder.hpp"

namespace boost {
namespace di {
namespace detail {

template<typename T, T value>
struct fake_dependency
{
    typedef fake_dependency type;
    typedef T given;
    typedef T expected;
    typedef mpl::vector0<> context;
    typedef void scope;
    typedef is_same<mpl::_1, T> bind;

    T create(const fake_pool&) {
        return value;
    }

    template<typename>
    struct result_type
    {
        typedef T type;
    };
};

template<typename T>
struct entries
    : T
{ };

BOOST_AUTO_TEST_CASE(creator_simple) {
    const int i = 42;

    typedef fake_dependency<int, i> dependency_type;
    fake_pool pool_;
    entries<dependency_type> entries_;
    std::map<const std::type_info*, boost::any> type_info_deps_;

    BOOST_CHECK_EQUAL(i, (
        creator<
            fake_binder<dependency_type>
        >::execute<int, mpl::vector0<> >(entries_, pool_, type_info_deps_)
    ));
}

} // namespace detail
} // namespace di
} // namespace boost

