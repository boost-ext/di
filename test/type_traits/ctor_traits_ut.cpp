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
#include "boost/mpl/string.hpp"

#include "boost/di/inject.hpp"
#include "boost/di/named.hpp"

#include <boost/units/detail/utility.hpp>
namespace boost {
namespace di {
namespace type_traits {

struct empty
{
    static void BOOST_DI_INJECTOR();
};

struct int_double
{
    static void BOOST_DI_INJECTOR(int, double);
};

struct extensions
{
    BOOST_DI_INJECT_TRAITS(char*, const int&);
};

struct ctor1
{
	ctor1(int);
};

struct ctor2
{
	ctor2(char*, const int&);
};

struct ctor2_ref
{
	ctor2_ref(int, int&);
};

struct ctor_inject_named
{
    BOOST_DI_INJECT_TRAITS(
        const named<int, mpl::string<'1'>>&
      , const named<int, mpl::string<'2'>>&
    );

    ctor_inject_named(int, int);
};

BOOST_AUTO_TEST_CASE(ctor) {
    BOOST_CHECK((mpl::equal<mpl::vector0<>, ctor_traits<void>::type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector0<>, ctor_traits<empty>::type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector2<int, double>, ctor_traits<int_double>::type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector2<char*, const int&>, ctor_traits<extensions>::type>::value));
	BOOST_CHECK((mpl::equal<mpl::vector1<any_type>, ctor_traits<ctor1>::type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector2<any_type, any_type>, ctor_traits<ctor2>::type>::value));
	BOOST_CHECK((mpl::equal<mpl::vector2<any_type, any_type>, ctor_traits<ctor2_ref>::type>::value));
    BOOST_CHECK((mpl::equal<
		mpl::vector2<
			const named<int, mpl::string<'1'>>&
		  , const named<int, mpl::string<'2'>>&
		>
	  , ctor_traits<ctor_inject_named>::type>::value
	));
}

} // namespace type_traits
} // namespace di
} // namespace boost

