//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS__HPP
#define BOOST_DI_CONCEPTS__HPP

#include <type_traits>
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/pool.hpp"

namespace boost { namespace di { namespace concepts {

template<class ignore>
struct lookup;

template <std::size_t ...ignore>
struct lookup<std::index_sequence<ignore...>> {
    template <class... ts>
    static aux::type_list<ts...>
    apply(decltype(ignore, (void*)nullptr)..., aux::no_decay<ts>*...);
};

template <std::size_t index, class... xs>
using eat = decltype(
    lookup<std::make_index_sequence<index>>::apply(
        (aux::no_decay<xs>*)nullptr...
    )
);

template <class x, class>
struct is_in;

template <class x, class... xs>
using is_in_impl = std::integral_constant<bool, !std::is_same<
    aux::bool_list<std::is_same<xs, x>{}...>,
    aux::bool_list<aux::never<xs>{}...>
>{}>;

template <class x, class... ts>
struct is_in<x, aux::type_list<ts...>> : is_in_impl<x, ts...> {};

template<class...>
struct unique_impl;

template<class... ts, std::size_t... s>
struct unique_impl<std::index_sequence<s...>, ts...>
    : std::is_same<aux::bool_list<aux::never<ts>{}...>, aux::bool_list<typename is_in<ts, typename eat<s + 1, ts...>::type>::type{}...>>
{ };

template<class... ts>
using unique = typename unique_impl<std::make_index_sequence<sizeof...(ts)>, ts...>::type;

template<class T>
struct expected {
    using type = aux::pair<
        aux::pair<typename T::expected, typename T::name>
      , std::integral_constant<bool, T::scope::priority>
    >;
};

std::false_type boundable(...);

template<class... Ts>
auto boundable(aux::type_list<Ts...>&&) -> unique<typename expected<Ts>::type...>;

template<class I, class T>
auto boundable(I&&, T&&) -> std::integral_constant<bool,
    std::is_convertible<T, I>{} || std::is_base_of<I, T>{}
>;

template<class T, class... Ts>
auto boundable(aux::type_list<Ts...>&&, T&&) ->
    std::integral_constant<bool, !std::is_same<aux::bool_list<aux::never<Ts>{}...>, aux::bool_list<std::is_base_of<Ts, T>{}...>>{}>;

}}} // boost::di::concepts

#endif

