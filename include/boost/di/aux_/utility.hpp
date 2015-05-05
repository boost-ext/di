//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_UTILITY_HPP
#define BOOST_DI_AUX_UTILITY_HPP

#include <type_traits>

namespace boost { namespace di { inline namespace v1 {

struct _ { constexpr _(...) { } };

namespace aux {

struct none_t { };

template<class...>
struct type { };

template<class T, T>
struct non_type { };

template<class...>
struct void_t {
    using type = void;
};

template<class...>
struct always : std::true_type { };

template<class...>
struct never : std::false_type { };

template<class, class>
struct pair { using type = pair; };

template<bool...>
struct bool_list { using type = bool_list; };

template<class...>
struct type_list { using type = type_list; };

template<class... Ts>
struct inherit : Ts... { using type = inherit; };

template<class T>
struct no_decay { using type = T; };

template<class TDefault, class>
static no_decay<TDefault> lookup(...);

template<class, class TKey, class TValue>
static no_decay<TValue> lookup(pair<TKey, TValue>*);

template<class TDefault, class TKey, class... Ts>
using at_key = decltype(lookup<TDefault, TKey>((inherit<Ts...>*)0));

template<class TDefault, class TKey, class... Ts>
using at_key_t = typename at_key<TDefault, TKey, Ts...>::type;

template<class...>
struct join;

template<>
struct join<> { using type = type_list<>; };

template<class... TArgs>
struct join<type_list<TArgs...>> {
    using type = type_list<TArgs...>;
};

template<class... TArgs1, class... TArgs2>
struct join<type_list<TArgs1...>, type_list<TArgs2...>> {
    using type = type_list<TArgs1..., TArgs2...>;
};

template<class... TArgs1, class... TArgs2, class... Ts>
struct join<type_list<TArgs1...>, type_list<TArgs2...>, Ts...> {
    using type = typename join<type_list<TArgs1..., TArgs2...>, Ts...>::type;
};

template<class... TArgs>
using join_t = typename join<TArgs...>::type;

template<class, class...>
struct is_unique_impl;

template<class...>
struct not_unique : std::false_type {
    using type = not_unique;
};

template<>
struct not_unique<> : std::true_type {
    using type = not_unique;
};

template<class T>
struct is_unique_impl<T> : not_unique<> { };

template<class T1, class T2, class... Ts>
struct is_unique_impl<T1, T2, Ts...>
    : std::conditional_t<
          std::is_base_of<type<T2>, T1>::value
        , not_unique<T2>
        , is_unique_impl<inherit<T1, type<T2>>, Ts...>
       >
{ };

template<class... Ts>
using is_unique = is_unique_impl<none_t, Ts...>;

}}}} // boost::di::v1::aux

#endif

