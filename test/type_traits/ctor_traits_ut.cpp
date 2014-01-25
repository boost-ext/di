//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/ctor_traits.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>

#include <boost/di/ctor.hpp>

namespace boost {
namespace di {
namespace type_traits {

struct empty
{
    static void BOOST_DI_CONSTRUCTOR();
};

struct int_double
{
    static void BOOST_DI_CONSTRUCTOR(int, double);
};

struct extensions
{
    BOOST_DI_CTOR_TRAITS(char*, const int&);
};

BOOST_AUTO_TEST_CASE(ctor) {
    BOOST_CHECK((mpl::equal<mpl::vector0<>, ctor_traits<void>::type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector0<>, ctor_traits<empty>::type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector2<int, double>, ctor_traits<int_double>::type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector2<char*, const int&>, ctor_traits<extensions>::type>::value));
}

} // namespace type_traits
} // namespace di
} // namespace boost

