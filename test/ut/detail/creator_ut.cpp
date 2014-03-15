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
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_same.hpp>

#include "boost/di/aux_/memory.hpp"
#include "common/fakes/fake_dependency.hpp"
#include "common/fakes/fake_binder.hpp"
#include "common/fakes/fake_visitor.hpp"

namespace boost {
namespace di {
namespace detail {

template<typename T>
struct entries
    : T
{
    typedef mpl::vector<T> types;
};

BOOST_AUTO_TEST_CASE(creator_simple) {
    const int i = 42;

    typedef typename fake_dependency<scopes::unique<>, int, mpl::int_<i>>::type dependency_type;
    entries<dependency_type> entries_;
    std::vector<aux::shared_ptr<void>> refs_;

    BOOST_CHECK_EQUAL(i, (
        creator<mpl::vector<dependency_type>>().create_<
            int, int, mpl::vector0<>, mpl::vector0<>
        >(entries_, refs_, fake_visitor<mpl::vector<int>>())
    ));
}

} // namespace detail
} // namespace di
} // namespace boost

