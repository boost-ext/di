//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts.hpp"
#include <boost/mpl/int.hpp>

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {

class c{ };

BOOST_AUTO_TEST_CASE(create) {
    typedef per_request<
        int
      , bind<int, mpl::int_<42> >
      , bind<c>
    >::type concept;
}

} // namespace di
} // namespace boost

