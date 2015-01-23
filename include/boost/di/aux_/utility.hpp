//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_UTILITY_HPP
#define BOOST_DI_AUX_UTILITY_HPP

#include <type_traits>

namespace boost { namespace di { namespace aux {

struct none_t { };

template<class...>
struct type { };

template<class T, T>
struct non_type { };

template<class...>
using void_t = void;

template<class...>
using always = std::true_type;

template<class...>
using never = std::false_type;

template<class T>
struct identity {
    using type = T;
};

template<class, class>
struct pair { using type = pair; };

template<bool...>
struct bool_list { using type = bool_list; };

template<class...>
struct type_list { using type = type_list; };

template<class... TArgs>
struct inherit : TArgs... { using type = inherit; };

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

template<class T, class TWrapper>
struct wrapper {
    inline operator T() noexcept {
        return wrapper_;
    }

    TWrapper wrapper_;
};

}}} // boost::di::aux

#endif

