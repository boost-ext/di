//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts.hpp"

#include <boost/test/unit_test.hpp>

#include "boost/di/module.hpp"
#include "boost/di/concepts.hpp"
#include "boost/di/make_module.hpp"

#include "data.hpp"

namespace boost {
namespace di {

BOOST_AUTO_TEST_CASE(module_) {
    using m = module<
        c0if0
    >;

    auto fm = make_module(
        m()
      , bind<int>::to(42)
    );

    shared_ptr<c5> c5_ = fm.create<shared_ptr<c5>>();
    BOOST_CHECK_EQUAL(42, c5_->c2_->i);
}

} // namespace di
} // namespace boost

