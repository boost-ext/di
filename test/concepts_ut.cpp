//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts.hpp"
#include <boost/mpl/int.hpp>

#include <boost/test/unit_test.hpp>

#include "data.hpp"

#include <boost/units/detail/utility.hpp>

namespace boost {
namespace di {

BOOST_AUTO_TEST_CASE(create) {
    typedef per_request<
        int
      , bind<int, mpl::int_<42> >
      , bind<c1>
    >::type concept;
}

BOOST_AUTO_TEST_CASE(def_scope) {
    typedef per_request<
        int
      , bind<c1>::in_call<c2>
    >::type concept;

    std::cout << boost::units::detail::demangle(typeid(concept).name()) << std::endl;

    //BOOST_CHECK(0);
}

} // namespace di
} // namespace boost

