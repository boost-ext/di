//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_CPP_14_UTILITY_HPP
#define BOOST_DI_AUX_CPP_14_UTILITY_HPP

#include <type_traits>

#define BOOST_DI_CAT_IMPL(a, b) a ## b
#define BOOST_DI_CAT(a, b) BOOST_DI_CAT_IMPL(a, b)
#define BOOST_DI_CALL(m, ...) m(__VA_ARGS__)

namespace boost {
namespace di {
namespace aux {

template<std::size_t...>
struct index_sequence {
    using type = index_sequence;
};

template<class, class>
struct concat;

template<std::size_t... I1, std::size_t... I2>
struct concat<index_sequence<I1...>, index_sequence<I2...>>
    : index_sequence<I1..., (sizeof...(I1) + I2)...>
{ };

template<std::size_t N>
struct make_index_sequence_impl
    : concat<
          typename make_index_sequence_impl<N/2>::type
        , typename make_index_sequence_impl<N - N/2>::type
      >::type
{ };

template<>
struct make_index_sequence_impl<0>
    : index_sequence<>
{ };

template<>
struct make_index_sequence_impl<1>
    : index_sequence<1>
{ };

template<std::size_t N>
using make_index_sequence = typename make_index_sequence_impl<N>::type;

} // namespace aux

struct none_t {};

template<class T, T>
struct non_type { };

template<class>
struct type { };

template<int N>
using int_ = std::integral_constant<int, N>;

template<bool N>
using bool_ = std::integral_constant<bool, N>;

template<class, class>
struct pair { };

template <class x>
struct no_decay { using type = x; };

template <class ...xs>
struct inherit : xs... { };

template<class...>
struct type_list {
    using type = type_list;
};

template<class>
struct size;

template<class... Ts>
struct size<type_list<Ts...>> {
    static constexpr auto value = sizeof...(Ts);
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

template<class... Args1, class... Args2>
struct join<type_list<Args1...>, type_list<Args2...>> {
    using type = type_list<Args1..., Args2...>;
};

template<class... Args1, class... Args2, class... Tail>
struct join<type_list<Args1...>, type_list<Args2...>, Tail...> {
    using type = typename join<type_list<Args1..., Args2...>, Tail...>::type;
};

template<class TDefault, class>
static no_decay<TDefault> lookup(...);

template<class, class TKey, class TValue>
static no_decay<TValue> lookup(pair<TKey, TValue>*);

template<class TDefault, class TKey, class T>
using at_key = decltype(lookup<TDefault, TKey>((T*)nullptr));

template<class TDefault, class TKey, class T>
using at_key_t = typename at_key<TDefault, TKey, T>::type;

} // namespace di
} // namespace boost

#endif

