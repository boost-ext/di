//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/any_type.hpp"
#include "boost/di/inject.hpp"

#include "boost/di/core/pool.hpp"

namespace boost { namespace di { namespace type_traits {

template<class, class>
struct named { };
struct direct { };
struct uniform { };

BOOST_DI_CALL(BOOST_DI_HAS_TYPE, BOOST_DI_INJECTOR);

template<class T, std::size_t>
struct get_type {
    using type = T;
};

template<template<class...> class, class, class, class>
struct ctor_args;

template<
    template<class...> class TIsConstructible
  , class T
  , class TAny
  , std::size_t... TArgs
> struct ctor_args<TIsConstructible, T, TAny, std::index_sequence<TArgs...>>
    : aux::pair<
          typename TIsConstructible<T, typename get_type<TAny, TArgs>::type...>::type
        , aux::type_list<typename get_type<TAny, TArgs>::type...>
      >
{ };

template<template<class...> class, template<class> class, class, class>
struct ctor_impl;

template<
    template<class...> class TIsConstructible
  , template<class> class TAny
  , class T
  , std::size_t... TArgs
> struct ctor_impl<TIsConstructible, TAny, T, std::index_sequence<TArgs...>>
    : aux::at_key_t<
          aux::type_list<>
        , std::true_type
        , ctor_args<
              TIsConstructible
            , T
            , TAny<T>
            , std::make_index_sequence<TArgs>
          >...
      >
{ };

template<
    template<class...> class TIsConstructible
  , template<class> class TAny
  , class T
> using ctor_impl_t =
    typename ctor_impl<
        TIsConstructible
      , TAny
      , T
      , std::make_index_sequence<BOOST_DI_CFG_CTOR_LIMIT_SIZE + 1>
    >::type;

template<class, template<class> class, class>
struct ctor;

template<class T, template<class> class TAny>
struct ctor<T, TAny, aux::type_list<>>
    : aux::pair<uniform, ctor_impl_t<aux::is_braces_constructible, TAny, T>>
{ };

template<class T, template<class> class TAny, class... TArgs>
struct ctor<T, TAny, aux::type_list<TArgs...>>
    : aux::pair<direct, aux::type_list<TArgs...>>
{ };

} // type_traits

template<class T, template<class> class TAny>
struct ctor_traits_
    : type_traits::ctor<T, TAny, type_traits::ctor_impl_t<std::is_constructible, TAny, T>>
{ };


template<class T>
struct ctor_traits
    : ctor_traits_<T, core::any_type_>
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
struct arg<const aux::type<T, std::true_type>&> {
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
struct arg<const aux::type<T, std::false_type>&> {
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
  , template<class> class
  , class = typename BOOST_DI_CAT(has_, BOOST_DI_INJECTOR)<T>::type
> struct ctor_traits;

template<
    class T
  , template<class> class
  , class = typename BOOST_DI_CAT(has_, BOOST_DI_INJECTOR)<di::ctor_traits<T>>::type
> struct ctor_traits_impl;

template<class T, template<class> class TAny>
struct ctor_traits<T, TAny, std::true_type>
    : aux::pair<direct, parse_args_t<typename T::BOOST_DI_INJECTOR::type>>
{ };

template<class T, template<class> class TAny>
struct ctor_traits<T, TAny, std::false_type>
    : ctor_traits_impl<T, TAny>
{ };

template<class T, template<class> class TAny>
struct ctor_traits_impl<T, TAny, std::true_type>
    : aux::pair<
          direct
        , parse_args_t<typename di::ctor_traits_<T, TAny>::BOOST_DI_INJECTOR::type>
      >
{ };

template<class T, template<class> class TAny>
struct ctor_traits_impl<T, TAny, std::false_type>
    : di::ctor_traits_<T, TAny>
{ };

}}} // boost::di::type_traits

#if (__has_include(<string>))
    #include <string>

    namespace boost { namespace di {
        template<
            class T
          , class Traits
          , class TAllocator
        > struct ctor_traits<std::basic_string<T, Traits, TAllocator>> {
            BOOST_DI_INJECT_TRAITS();
        };
    }} // boost::di

#endif

#if (__has_include(<initializer_list>))
    #include <initializer_list>

    namespace boost { namespace di {
        template<class T>
        struct ctor_traits<std::initializer_list<T>> {
            BOOST_DI_INJECT_TRAITS();
        };
    }} // boost::di

#endif

#endif

