//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP

#include "boost/di/aux_/meta.hpp"
#include "boost/di/inject.hpp"
#include "boost/di/type_traits/parameter_types.hpp"
#include "boost/di/type_traits/has_ctor.hpp"

#include <boost/preprocessor/cat.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/non_type.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/aux_/yes_no.hpp>

namespace boost {
namespace di {

struct any_type { };

template<typename T>
struct get_int
{
	typedef mpl::int_<T::value> type;
};

template<typename T>
struct get_
	: mpl::fold<
		mpl::range_c<int, 1, BOOST_DI_CTOR_LIMIT_SIZE + 1>
	  , mpl::int_<0>
	  , mpl::if_<
			type_traits::has_ctor<T, get_int<mpl::_2> >
		  , mpl::_2
		  , mpl::_1
		>
	  >::type
{ };

template<typename T>
struct ctor_traits
	: mpl::fold<
		  mpl::range_c<int, 0, get_<T>::value>
		, mpl::vector0<>
		, mpl::push_back<mpl::_1, any_type>
	  >
{ };

namespace type_traits {

template<typename T>
class BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)
{
    struct base_impl { void BOOST_DI_INJECTOR(...) { } };
    struct base
        : base_impl
        , mpl::if_<is_class<T>, T, mpl::void_>::type
    { base() { } };

    template<typename U>
    static mpl::aux::no_tag test(
        U*
      , non_type<void (base_impl::*)(...), &U::BOOST_DI_INJECTOR>* = 0
    );

    static mpl::aux::yes_tag test(...);

public:
    typedef BOOST_PP_CAT(has_, BOOST_DI_INJECTOR) type;

    BOOST_STATIC_CONSTANT(
        bool
      , value = sizeof(test((base*)(0))) == sizeof(mpl::aux::yes_tag)
    );
};

template<typename T, typename = void>
struct ctor_traits
	: di::ctor_traits<T>::type
{ };

template<typename T>
struct ctor_traits<T, typename enable_if<BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<di::ctor_traits<T> > >::type>
	: parameter_types<BOOST_TYPEOF_TPL(&di::ctor_traits<T>::BOOST_DI_INJECTOR)>::type
{ };

template<typename T>
struct ctor_traits<T, typename enable_if<BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<T> >::type>
	: parameter_types<BOOST_TYPEOF_TPL(&T::BOOST_DI_INJECTOR)>::type
{ };

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

