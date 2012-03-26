//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "QDeps/Back/Dependency.hpp"

namespace QDeps
{
namespace Back
{
namespace Detail
{
namespace UT
{

using namespace boost;
using namespace boost::mpl;

template<typename T> class FakeValue { };
class FakePool { };
class FakeScope { };
class OtherFakeScope { };

TEST(Dependency, Apply)
{ 
    typedef Dependency
    <
        FakeScope,
        int,
        int,
        boost::mpl::vector0<>,
        boost::is_same<boost::mpl::_1, int>,
        FakeValue
    > Dep;

    std::cout << abi::__cxa_demangle(typeid(Dep::Apply<FakeScope>::type).name(), 0, 0, 0) << std::endl;

    EXPECT_TRUE((
        equal
        <
            Dependency<OtherFakeScope, int, int, boost::mpl::vector0<>, boost::is_same<boost::mpl::_1, int>, FakeValue>,
            Dep::Apply<OtherFakeScope>::type
        >::value
    ));
}

} // namespace UT
} // namespace Detail
} // namespace Back
} // namespace QDeps

