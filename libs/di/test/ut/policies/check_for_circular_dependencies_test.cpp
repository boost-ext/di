//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#define BOOST_DI_STATIC_ASSERT(cond, expr, types) typedef void _

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include "di/back/policies/check_for_circular_dependencies.hpp"
#include "test/common/data.hpp"

namespace di
{
namespace back
{
namespace policies
{
namespace ut
{

using namespace test::common;

BOOST_AUTO_TEST_CASE(Nocircular_dependencies)
{
    BOOST_CHECK((
        !is_base_of
        <
            mpl::false_,
            check_for_circular_dependencies::verify
            <
                vector0<>,
                c8
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(Direct)
{
    BOOST_CHECK((
        is_base_of
        <
            mpl::false_,
            check_for_circular_dependencies::verify
            <
                vector0<>,
                cd1
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(InDirect)
{
    BOOST_CHECK((
        is_base_of
        <
            mpl::false_,
            check_for_circular_dependencies::verify
            <
                vector0<>,
                cd5
            >::type
        >::value
    ));
}

} // namespace ut
} // namespace policies
} // namespace back
} // namespace di

