//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/core/builder.hpp"

#include <boost/test/unit_test.hpp>

#include "common/fakes/fake_creator.hpp"
#include "common/fakes/fake_pool.hpp"
#include "common/fakes/fake_visitor.hpp"
#include "common/fakes/fake_dependency.hpp"
#include "common/fakes/fake_scope.hpp"

namespace boost {
namespace di {
namespace core {

BOOST_AUTO_TEST_CASE(build) {
    fake_creator<int> creator;
    fake_pool<> deps;
    fake_visitor<> visitor;
    fake_pool<> policies;
    std::vector<aux::shared_ptr<void>> refs;

    BOOST_CHECK_EQUAL(0, (builder().build<
        int
      , mpl::vector0<>
      , mpl::vector0<>
      , fake_dependency<fake_scope<> , int>
    >(creator, deps, refs, visitor, policies)));
}

} // namespace core
} // namespace di
} // namespace boost

