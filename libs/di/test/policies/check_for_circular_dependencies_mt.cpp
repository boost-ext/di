//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include "boost/di/policies/check_for_circular_dependencies.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace policies {

BOOST_AUTO_TEST_CASE(circular_dependencies_compile_fail)
{
}

} // namespace policies
} // namespace di
} // namespace boost

