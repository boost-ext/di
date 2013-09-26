//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/policies/arguments_allowance.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

/*#include "fake_dependency.hpp"*/
//#include "fake_dependency_base_of.hpp"
//#include "contains_all.hpp"
#include "data.hpp"

#include <boost/units/detail/utility.hpp>
namespace boost {
namespace di {
namespace policies {

BOOST_AUTO_TEST_CASE(empty) {

    std::cout << boost::units::detail::demangle(typeid(
    arguments_allowance<>::verify<
                    mpl::vector<>
                  , c8
                  , false
                >::type
                ).name()) << std::endl;

    BOOST_CHECK(false);
}

} // namespace policies
} // namespace di
} // namespace boost

