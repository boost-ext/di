//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/is_same.hpp>
#include "QDeps/Utility/Named.hpp"
#include "QDeps/Back/Aux/Utility.hpp"

namespace QDeps
{
namespace Back
{
namespace Aux
{
namespace UT
{

using namespace boost;
using namespace boost::mpl;
using namespace Utility;

class A { };

template<typename T> void MakePlainTest()
{
    EXPECT_TRUE((boost::is_same<T, typename MakePlain<T>::type>::value));
    EXPECT_TRUE((boost::is_same<T, typename MakePlain<T*>::type>::value));
    EXPECT_TRUE((boost::is_same<T, typename MakePlain<const T*>::type>::value));
    EXPECT_TRUE((boost::is_same<T, typename MakePlain<const T>::type>::value));
    EXPECT_TRUE((boost::is_same<T, typename MakePlain<const T&>::type>::value));
    EXPECT_TRUE((boost::is_same<T, typename MakePlain<T&>::type>::value));
    EXPECT_TRUE((boost::is_same<T, typename MakePlain<shared_ptr<T> >::type>::value));
    EXPECT_TRUE((boost::is_same<T, typename MakePlain<const shared_ptr<T>&>::type>::value));
    EXPECT_TRUE((boost::is_same<T, typename MakePlain<shared_ptr<T>&>::type>::value));
    EXPECT_TRUE((boost::is_same<T, typename MakePlain<volatile T>::type>::value));
    EXPECT_TRUE((boost::is_same<Named<T, _1>, typename MakePlain<Named<T, _1> >::type>::value));
    EXPECT_TRUE((boost::is_same<Named<T, _1>, typename MakePlain<Named<shared_ptr<T>, _1> >::type>::value));
    EXPECT_TRUE((boost::is_same<Named<T, _1>, typename MakePlain<Named<const shared_ptr<T>&, _1> >::type>::value));
    EXPECT_TRUE((boost::is_same<Named<T, _1>, typename MakePlain<shared_ptr< Named<const shared_ptr<T>&, _1> > >::type>::value));
    EXPECT_TRUE((boost::is_same<Named<T, _1>, typename MakePlain<const shared_ptr< Named<const shared_ptr<T>&, _1> >&>::type>::value));
}

BOOST_AUTO_TEST_CASE(MakePlain)
{
    MakePlainTest<int>();
    MakePlainTest<A>();
}

} // namespace UT
} // namespace Aux
} // namespace Back
} // namespace QDeps

