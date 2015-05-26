//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP

#include "boost/di/aux_/compiler_specific.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/inject.hpp"
#include "boost/di/fwd.hpp"

namespace boost { namespace di { inline namespace v1 { namespace type_traits {

template<class, class>
struct named { };
struct direct { };
struct uniform { };

BOOST_DI_CALL(BOOST_DI_HAS_TYPE, BOOST_DI_INJECTOR);

template<class T, std::size_t>
struct get {
    using type = T;
};

template<class T, std::size_t N>
using get_t = typename get<T, N>::type;

template<template<class...> class, class, class, class = void>
struct ctor_impl;

template<template<class...> class TIsConstructible, class T, std::size_t... TArgs>
struct ctor_impl<TIsConstructible, T, std::index_sequence<TArgs...>
    , BOOST_DI_REQUIRES_T((sizeof...(TArgs) > 0) && !TIsConstructible<T, get_t<core::any_type_fwd<T>, TArgs>...>::value)>
    : std::conditional<
           TIsConstructible<T, get_t<core::any_type_ref_fwd<T>, TArgs>...>::value
         , aux::type_list<get_t<core::any_type_ref_fwd<T>, TArgs>...>
         , typename ctor_impl<
               TIsConstructible
             , T
             , std::make_index_sequence<sizeof...(TArgs) - 1>
           >::type
      >
{ };

template<template<class...> class TIsConstructible, class T, std::size_t... TArgs>
struct ctor_impl<TIsConstructible, T, std::index_sequence<TArgs...>,
      BOOST_DI_REQUIRES_T((sizeof...(TArgs) > 0) && TIsConstructible<T, get_t<core::any_type_fwd<T>, TArgs>...>::value)>
    : aux::type_list<get_t<core::any_type_fwd<T>, TArgs>...>
{ };

template<template<class...> class TIsConstructible, class T>
struct ctor_impl<TIsConstructible, T, std::index_sequence<>>
    : aux::type_list<>
{ };

template<template<class...> class TIsConstructible, class T>
using ctor_impl_t =
    typename ctor_impl<
        TIsConstructible
      , T
      , std::make_index_sequence<BOOST_DI_CFG_CTOR_LIMIT_SIZE>
    >::type;

template<class...>
struct ctor;

template<class T>
struct ctor<T, aux::type_list<>>
    : aux::pair<uniform, ctor_impl_t<aux::is_braces_constructible, T>>
{ };

template<class T, class... TArgs>
struct ctor<T, aux::type_list<TArgs...>>
    : aux::pair<direct, aux::type_list<TArgs...>>
{ };

} // type_traits

template<class T>
struct ctor_traits
    : type_traits::ctor<T, type_traits::ctor_impl_t<std::is_constructible, T>>
{ };

namespace type_traits {

template<class>
struct parse_args;

template<class T>
struct arg {
    using type = T;
};

template<class>
struct arg_impl;

template<class T>
struct arg_impl<aux::type_list<T>> {
    using type = T;
};

template<class T>
using arg_impl_t = typename arg_impl<T>::type;

template<class T>
struct arg<aux::type<T, std::true_type>> {
    using type = named<
        typename aux::function_traits<
            decltype(T::BOOST_DI_CAT(BOOST_DI_INJECTOR, name))
        >::result_type
      , arg_impl_t<typename aux::function_traits<
            decltype(T::BOOST_DI_CAT(BOOST_DI_INJECTOR, arg))
        >::args>
    >;
};

template<class T>
struct arg<aux::type<T, std::false_type>> {
    using type = arg_impl_t<typename aux::function_traits<
        decltype(T::BOOST_DI_CAT(BOOST_DI_INJECTOR, arg))
    >::args>;
};

template<class... Ts>
struct parse_args<aux::type_list<Ts...>>
    : aux::type_list<typename arg<Ts>::type...>
{ };

template<class... Ts>
using parse_args_t = typename parse_args<Ts...>::type;

template<
    class T
  , class = typename BOOST_DI_CAT(has_, BOOST_DI_INJECTOR)<T>::type
> struct ctor_traits;

template<
    class T
  , class = typename BOOST_DI_CAT(has_, BOOST_DI_INJECTOR)<di::ctor_traits<T>>::type
> struct ctor_traits_impl;

template<class T>
struct ctor_traits<T, std::true_type>
    : aux::pair<direct, parse_args_t<typename T::BOOST_DI_INJECTOR::type>>
{ };

template<class T>
struct ctor_traits<T, std::false_type>
    : ctor_traits_impl<T>
{ };

template<class T>
struct ctor_traits_impl<T, std::true_type>
    : aux::pair<
          direct
        , parse_args_t<typename di::ctor_traits<T>::BOOST_DI_INJECTOR::type>
      >
{ };

template<class T>
struct ctor_traits_impl<T, std::false_type>
    : di::ctor_traits<T>
{ };

}}}} // boost::di::v1::type_traits

#include <string>

namespace boost { namespace di { inline namespace v1 {
    template<
        class T
      , class Traits
      , class TAllocator
    > struct ctor_traits<std::basic_string<T, Traits, TAllocator>> {
        BOOST_DI_INJECT_TRAITS();
    };
}}} // boost::di::v1

namespace std {
    template<class>
    class initializer_list;

    #if defined(BOOST_DI_MSVC)
        template<class>
        class function;
    #endif
} // std

namespace boost { namespace di { inline namespace v1 {
    template<class T>
    struct ctor_traits<std::initializer_list<T>> {
        BOOST_DI_INJECT_TRAITS();
    };

    #if defined(BOOST_DI_MSVC)
        template<class T>
        struct ctor_traits<std::function<T>> {
            BOOST_DI_INJECT_TRAITS();
        };
    #endif
}}} // boost::di::v1

#endif

