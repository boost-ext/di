//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/aux_/module.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

#include "boost/di/aux_/instance.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace aux_ {

BOOST_AUTO_TEST_CASE(module_externals_fail) {
    instance<int> i_(42);

    module<
        mpl::vector0<>
      , mpl::vector<
            instance<int>
          , instance<double>
        >
    > module_(i_);

    module_.create<c14>();
}

} // namespace aux_
} // namespace di
} // namespace boost

