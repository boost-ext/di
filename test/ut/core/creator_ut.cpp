//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/core/creator.hpp"

#include <vector>
#include <boost/test/unit_test.hpp>
#include <boost/function.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_same.hpp>

#include "boost/di/aux_/memory.hpp"
#include "common/fakes/fake_allocator.hpp"
#include "common/fakes/fake_dependency.hpp"
#include "common/fakes/fake_binder.hpp"
#include "common/fakes/fake_visitor.hpp"
#include "common/fakes/fake_pool.hpp"
#include "common/data.hpp"

namespace boost {
namespace di {
namespace core {

BOOST_AUTO_TEST_CASE(create_pod) {
    const int i = 42;

    typedef fake_dependency<scopes::unique<>, int, aux::mpl::int_<i>>::type dependency_type;
    fake_allocator allocator;
    fake_pool<dependency_type> deps;
    std::vector<aux::shared_ptr<void>> refs;

    BOOST_CHECK_EQUAL(i, (
        creator<aux::mpl::vector<dependency_type>>().create<
            int, int, aux::mpl::vector0<>
        >(allocator, deps, refs, fake_visitor<aux::mpl::vector<int>>(), fake_pool<>())
    ));
}

} // namespace core
} // namespace di
} // namespace boost

