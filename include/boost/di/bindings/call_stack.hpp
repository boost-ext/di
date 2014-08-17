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

template<typename... Ts>
struct call_stack {
    template<typename T>
    using apply = std::is_same<typename make_plain<typename T::call_stack>::type, type_list<Ts...>>;

    template<typename T>
    using eval = typename match<typename make_plain<typename T::call_stack>::type, type_list<Ts...>>::type;
};

} // namespace bindings
} // namespace di
} // namespace boost

#endif

