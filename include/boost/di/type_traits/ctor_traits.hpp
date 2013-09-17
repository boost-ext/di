//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/aux_/yes_no.hpp>

#include "boost/di/type_traits/parameter_types.hpp"
#include "boost/di/config.hpp"

namespace boost {
namespace di {

template<typename>
struct ctor_traits
{
    static void ctor(); //trivial ctor
};

namespace type_traits {

template<typename T>
class BOOST_PP_CAT(has_, BOOST_DI_CONSTRUCTOR)
{
    template<typename C> static mpl::aux::yes_tag test(BOOST_DI_TYPEOF(C::BOOST_DI_CONSTRUCTOR));
    template<typename>   static mpl::aux::no_tag  test(...);

public:
    BOOST_STATIC_CONSTANT(
        bool
      , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
    );
};

template<typename T>
class BOOST_PP_CAT(has_, BOOST_DI_CREATE)
{
    template<typename C> static mpl::aux::yes_tag test(BOOST_DI_TYPEOF(C::BOOST_DI_CREATE));
    template<typename>   static mpl::aux::no_tag  test(...);

public:
    BOOST_STATIC_CONSTANT(
        bool
      , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
    );
};

template<typename T, typename = void>
struct ctor_traits
    : parameter_types<BOOST_TYPEOF_TPL(&di::ctor_traits<T>::ctor)>::type
{ };

template<typename T>
struct ctor_traits<T, typename enable_if<BOOST_PP_CAT(has_, BOOST_DI_CONSTRUCTOR)<T> >::type>
    : parameter_types<BOOST_TYPEOF_TPL(&T::BOOST_DI_CONSTRUCTOR)>::type
{ };

template<typename T>
struct ctor_traits<T, typename enable_if<BOOST_PP_CAT(has_, BOOST_DI_CREATE)<T> >::type>
    : parameter_types<BOOST_TYPEOF_TPL(&T::BOOST_DI_CREATE)>::type
{ };

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

