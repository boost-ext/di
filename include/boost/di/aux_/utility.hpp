//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_CPP_14_UTILITY_HPP
#define BOOST_DI_AUX_CPP_14_UTILITY_HPP

#include <utility>
#include <type_traits>

#define BOOST_DI_CAT_IMPL(a, b) a ## b
#define BOOST_DI_CAT(a, b) BOOST_DI_CAT_IMPL(a, b)
#define BOOST_DI_CALL(m, ...) m(__VA_ARGS__)

namespace boost {
namespace di {
namespace aux {

template<class>
struct type { };

struct none_t {};

template<class T, T>
struct non_type { };

template<class, class>
struct pair { };

template<class T>
struct no_decay { using type = T; };

template<class... TArgs>
struct inherit : TArgs... { };

template<class...>
struct type_list {
    using type = type_list;
};

template<class...>
struct join;

template<>
struct join<> {
    using type = type_list<>;
};

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

template<class TDefault, class>
static no_decay<TDefault> lookup(...);

template<class, class TKey, class TValue>
static no_decay<TValue> lookup(pair<TKey, TValue>*);

template<class TDefault, class TKey, class... Ts>
using at_key = decltype(lookup<TDefault, TKey>((inherit<Ts...>*)0));

template<class TDefault, class TKey, class T>
using at_key_t = typename at_key<TDefault, TKey, T>::type;

} // namespace aux
} // namespace di
} // namespace boost

#endif

