//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/detail/creator.hpp"

#include <vector>
#include <boost/test/unit_test.hpp>
#include <boost/function.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>

#include "boost/di/aux_/memory.hpp"
#include "fake_binder.hpp"
#include "fake_visitor.hpp"

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
    typedef T result_type;
    typedef void name;

    T create() { return value; }
};

template<typename T>
struct entries
    : T
{
    typedef mpl::vector<T> types;
};

BOOST_AUTO_TEST_CASE(creator_simple) {
    const int i = 42;

    typedef fake_dependency<int, i> dependency_type;
    entries<dependency_type> entries_;
    std::vector<aux::shared_ptr<void>> cleanup_;
    std::vector<aux::shared_ptr<void>> refs_;

    BOOST_CHECK_EQUAL(i, (
        creator<
            fake_binder<dependency_type>
          , mpl::vector0<>
        >::execute<int, void, mpl::vector0<>>(entries_, cleanup_, refs_, fake_visitor<>())
    ));
}

} // namespace detail
} // namespace di
} // namespace boost

