//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_MPL_HPP
#define BOOST_DI_AUX_MPL_HPP

#include "boost/di/aux_/config.hpp"
#include <type_traits>

namespace boost {
namespace di {

struct none_t {};

template<typename T, T>
struct non_type { };

template<typename>
struct type { };

template<int N>
using int_ = std::integral_constant<int, N>;

template<bool N>
using bool_ = std::integral_constant<bool, N>;

template<typename, typename>
struct pair {
    using type = pair;
};

template <typename x>
struct no_decay { using type = x; };

template <typename ...xs>
struct inherit : xs... { };

template<typename...>
struct type_list {
    using type = type_list;
};

template<typename>
struct is_type_list
    : std::false_type
{ };

template<typename... Ts>
struct is_type_list<type_list<Ts...>>
    : std::true_type
{ };

template<typename>
struct size;

template<typename... Ts>
struct size<type_list<Ts...>> {
    static const std::size_t value = sizeof...(Ts);
};

template<bool...>
struct bool_seq {
    using type = bool_seq;
};

template<typename...>
struct join;

template<>
struct join<> {
    using type = type_list<>;
};

template<typename... TArgs>
struct join<type_list<TArgs...>> {
    using type = type_list<TArgs...>;
};

template<typename... Args1, typename... Args2>
struct join<type_list<Args1...>, type_list<Args2...>> {
    using type = type_list<Args1..., Args2...>;
};

template<typename... Args1, typename... Args2, typename... Tail>
struct join<type_list<Args1...>, type_list<Args2...>, Tail...> {
     using type = typename join<type_list<Args1..., Args2...>, Tail...>::type;
};

template<typename T>
using not_ = bool_<!T::value>;

template<typename... Ts>
using and_ = std::is_same<
    bool_seq<Ts::value...>,
    bool_seq<(Ts::value, true)...>
>;

template<typename... Ts>
using or_ = bool_<
    !std::is_same<
        bool_seq<Ts::value...>,
        bool_seq<(Ts::value, false)...>
    >::value
>;

template <typename d, typename key>
static no_decay<d> lookup(...);

template <typename, typename key, typename value>
static no_decay<value> lookup(pair<key, value>*);

template <typename d, typename key, typename ...pairs>
using at_key = decltype(lookup<d, key>((inherit<pairs...>*)0));

} // namespace di
} // namespace boost

#endif

