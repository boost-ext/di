//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP

#include <string>

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/any_type.hpp"
#include "boost/di/inject.hpp"

namespace boost { namespace di { namespace type_traits {

struct direct { };
struct aggregate { };

BOOST_DI_CALL(BOOST_DI_HAS_METHOD_CALL, BOOST_DI_INJECTOR, BOOST_DI_INJECTOR);

template<class T, std::size_t>
struct get_type {
    using type = T;
};

template<template<class...> class, class, class, class>
struct ctor_impl;

template<template<class...> class Is, class T, class TAny, std::size_t... TArgs>
struct ctor_impl<Is, T, TAny, std::index_sequence<TArgs...>>
    : aux::pair<
          Is<T, typename get_type<TAny, TArgs>::type...>
        , aux::type_list<typename get_type<TAny, TArgs>::type...>
      >
{ };

template<template<class...> class, class, class>
struct ctor;

template<template<class...> class Is, class T, std::size_t... Args>
struct ctor<Is, T, std::index_sequence<Args...>>
    : aux::at_key_t<
          aux::type_list<>
        , std::true_type
        , ctor_impl<Is, T, core::any_type<T>, std::make_index_sequence<Args>>...
      >
{ };

} // namespace type_traits

template<class>
struct size;

template<class... Ts>
struct size<aux::type_list<Ts...>> {
    static constexpr auto value = sizeof...(Ts);
};

template<class T, class TR = typename type_traits::ctor<std::is_constructible, T, std::make_index_sequence<BOOST_DI_CFG_CTOR_LIMIT_SIZE + 1>>::type>
struct ctor_traits
    : std::conditional_t<
            (size<TR>::value > 0)
          , aux::pair<type_traits::direct, TR>
          , aux::pair<type_traits::aggregate, typename type_traits::ctor<aux::is_braces_constructible, T, std::make_index_sequence<BOOST_DI_CFG_CTOR_LIMIT_SIZE + 1>>::type>
      >
{ };

template<
    class T
  , class Traits
  , class TAllocator
>
struct ctor_traits<std::basic_string<T, Traits, TAllocator>> {
    BOOST_DI_INJECT_TRAITS();
};

namespace type_traits {

template<
    class T
  , class = typename BOOST_DI_CAT(has_, BOOST_DI_INJECTOR)<T>::type
>
struct ctor_traits;

template<
    class T
  , class = typename BOOST_DI_CAT(has_, BOOST_DI_INJECTOR)<di::ctor_traits<T>>::type
>
struct ctor_traits_impl;

template<class T>
struct ctor_traits<T, std::true_type>
    : aux::pair<direct, typename aux::function_traits<decltype(&T::BOOST_DI_INJECTOR)>::args>
{ };

template<class T>
struct ctor_traits<T, std::false_type>
    : ctor_traits_impl<T>
{ };

template<class T>
struct ctor_traits_impl<T, std::true_type>
    : aux::pair<direct, typename aux::function_traits<decltype(&di::ctor_traits<T>::BOOST_DI_INJECTOR)>::args>
{ };

template<class T>
struct ctor_traits_impl<T, std::false_type>
    : di::ctor_traits<T>
{ };

}}} // namespace boost::di::type_traits

#endif

