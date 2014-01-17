//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP

#include "boost/di/type_traits/parameter_types.hpp"
#include "boost/di/ctor.hpp"

#include <boost/preprocessor/cat.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/non_type.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/aux_/yes_no.hpp>

namespace boost {
namespace di {

template<typename>
struct ctor_traits
{
    static void BOOST_DI_CONSTRUCTOR(); //trivial ctor
};

namespace type_traits {

template<typename T>
class BOOST_PP_CAT(has_, BOOST_DI_CONSTRUCTOR)
{
    struct base_impl { void BOOST_DI_CONSTRUCTOR(...) { } };
    struct base
        : base_impl
        , mpl::if_<is_class<T>, T, mpl::void_>::type
    { base() { } };

    template<typename U>
    static mpl::aux::no_tag test(
        U*
      , non_type<void (base_impl::*)(...), &U::BOOST_DI_CONSTRUCTOR>* = 0
    );

    static mpl::aux::yes_tag test(...);

public:
    BOOST_STATIC_CONSTANT(
        bool
      , value = sizeof(test((base*)(0))) == sizeof(mpl::aux::yes_tag)
    );
};

template<typename T>
class BOOST_PP_CAT(has_, BOOST_DI_CREATE)
{
    struct base_impl { void BOOST_DI_CREATE(...) { } };
    struct base
        : base_impl
        , mpl::if_<is_class<T>, T, mpl::void_>::type
    { base() { } };

    template<typename U>
    static mpl::aux::no_tag test(
        U*
      , non_type<void (base_impl::*)(...), &U::BOOST_DI_CREATE>* = 0
    );

    static mpl::aux::yes_tag test(...);

public:
    BOOST_STATIC_CONSTANT(
        bool
      , value = sizeof(test((base*)(0))) == sizeof(mpl::aux::yes_tag)
    );
};

template<typename T, typename = void>
struct ctor_traits
    : parameter_types<BOOST_TYPEOF_TPL(&di::ctor_traits<T>::BOOST_DI_CONSTRUCTOR)>::type
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

