//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/inject.hpp"
#include "boost/di/core/any_type.hpp"
#include "boost/di/type_traits/has_ctor.hpp"
#include "boost/di/type_traits/has_injector.hpp"
#include "boost/di/type_traits/function_traits.hpp"

#include <string>
#include <boost/preprocessor/cat.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/x11/fold.hpp>
#include <boost/mpl/x11/range_c.hpp>
#include <boost/mpl/x11/if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/x11/push_pop.hpp> // wknd

namespace boost {
namespace di {
namespace detail {

template<typename T>
struct get_value
    : mpl::int_<T::value>
{ };

template<typename T>
struct get_longest_ctor
    : mpl::x11::fold<
          mpl::x11::range_c<int, 1, BOOST_DI_CFG_CTOR_LIMIT_SIZE + 1>
        , mpl::x11::int_<0>
        , mpl::x11::if_<
              type_traits::has_ctor<T, get_value<mpl::x11::arg<1>> >
            , mpl::x11::arg<1>
            , mpl::x11::arg<0>
          >
      >::type
{ };

} // detail

template<typename T>
struct ctor_traits
   : mpl::x11::fold<
          mpl::x11::range_c<int, 0, detail::get_longest_ctor<T>::value>
        , mpl::x11::vector<>
        , mpl::x11::push_back<mpl::x11::arg<0>, core::any_type<> >
      >
{ };

template<typename T>
struct ctor_traits<std::basic_string<T> > // basic_string ctors are ambiguous
{
    static void BOOST_DI_INJECTOR();
};

namespace type_traits {

template<typename T, typename = void>
struct ctor_traits
    : di::ctor_traits<T>::type
{ };

template<typename T>
struct ctor_traits<T, typename enable_if<BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<di::ctor_traits<T> > >::type>
    : function_traits<BOOST_DI_FEATURE_DECLTYPE(&di::ctor_traits<T>::BOOST_DI_INJECTOR)>::type
{ };

template<typename T>
struct ctor_traits<T, typename enable_if<BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<T> >::type>
    : function_traits<BOOST_DI_FEATURE_DECLTYPE(&T::BOOST_DI_INJECTOR)>::type
{ };

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

