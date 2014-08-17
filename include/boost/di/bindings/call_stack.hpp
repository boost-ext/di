//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_BINDINGS_CALL_STACK_HPP
#define BOOST_DI_BINDINGS_CALL_STACK_HPP

#include <type_traits>
#include "boost/di/aux_/mpl.hpp"
#include "boost/di/type_traits/make_plain.hpp"

namespace boost {
namespace di {
namespace bindings {

template<typename, typename>
struct match;

template<typename T1, typename... Ts1, typename T2, typename... Ts2>
struct match<type_list<T1, Ts1...>, type_list<T2, Ts2...>>
    : bool_<std::is_same<T1, T2>::value && match<type_list<Ts1...>, type_list<Ts2...>>::value>
{ };

template<>
struct match<type_list<>, type_list<>>
    : std::true_type
{ };

template<typename... Ts>
struct match<type_list<>, type_list<Ts...>>
    : std::true_type
{ };

template<typename... Ts>
struct match<type_list<Ts...>, type_list<>>
    : std::false_type
{ };

template<typename T>
struct make_plain;

template<typename... Ts>
struct make_plain<type_list<Ts...>>
    : type_list<typename di::type_traits::make_plain<Ts>::type...>
{ };

template<typename, std::size_t>
struct take_last;

template<std::size_t N, typename T, typename... Ts>
struct take_last<type_list<T, Ts...>, N>
    : std::conditional<!N, type_list<T, Ts...>, typename take_last<type_list<Ts...>, N-1>::type>::type
{ };

template<std::size_t N>
struct take_last<type_list<>, N>
    : type_list<>
{ };

template<typename, typename>
struct is_same_call_stack;

template<typename... Ts, typename T>
struct is_same_call_stack<type_list<Ts...>, T>
    : std::conditional<std::is_same<type_list<Ts...>, T>::value, int_<sizeof...(Ts) + 1>, int_<0>>
{ };

template<typename... Ts>
struct call_stack {
    template<typename T>
    using apply = is_same_call_stack<
        typename make_plain<
            typename take_last<
                typename T::call_stack
              , size<typename T::call_stack>::value - sizeof...(Ts)
            >::type
        >::type
      , type_list<Ts...>
    >;

    template<typename T>
    using eval = int_<
        match<typename make_plain<typename T::call_stack>::type, type_list<Ts...>>::value
    >;
};

} // namespace bindings
} // namespace di
} // namespace boost

#endif

