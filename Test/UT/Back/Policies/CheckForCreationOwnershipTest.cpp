//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#define QDEPS_STATIC_ASSERT(cond, expr, types) typedef void _

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include "QDeps/Back/Policies/CheckForCreationOwnership.hpp"

namespace QDeps
{
namespace Back
{
namespace Policies
{
namespace UT
{

using namespace boost;
using namespace boost::mpl;

BOOST_AUTO_TEST_CASE(ClearCreationOwnership)
{
    BOOST_CHECK((boost::is_base_of<boost::mpl::true_, CheckForCreationOwnership::Assert<vector0<>, int>::type>::value));
    BOOST_CHECK((boost::is_base_of<boost::mpl::true_, CheckForCreationOwnership::Assert<vector0<>, int*>::type>::value));
    BOOST_CHECK((boost::is_base_of<boost::mpl::true_, CheckForCreationOwnership::Assert<vector0<>, const int*>::type>::value));
    BOOST_CHECK((boost::is_base_of<boost::mpl::true_, CheckForCreationOwnership::Assert<vector0<>, shared_ptr<int> >::type>::value));
}

BOOST_AUTO_TEST_CASE(NotClearCreationOwnership)
{
    BOOST_CHECK((boost::is_base_of<boost::mpl::false_, CheckForCreationOwnership::Assert<vector0<>, int&>::type>::value));
    BOOST_CHECK((boost::is_base_of<boost::mpl::false_, CheckForCreationOwnership::Assert<vector0<>, const int&>::type>::value));
}

} // namespace UT
} // namespace Policies
} // namespace Back
} // namespace QDeps

