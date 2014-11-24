//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_CPP_14_UTILITY_HPP
#define BOOST_DI_AUX_CPP_14_UTILITY_HPP

#include <type_traits>

#define BOOST_DI_CAT_IMPL(A, B) A ## B
#define BOOST_DI_CAT(A, B) BOOST_DI_CAT_IMPL(A, B)

namespace boost {
namespace di {
namespace aux {

template<std::size_t...>
struct index_sequence {
    using type = index_sequence;
};

template<typename, typename>
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

template<typename T, T>
struct non_type { };

template<typename>
struct type { };

template<int N>
using int_ = std::integral_constant<int, N>;

template<bool N>
using bool_ = std::integral_constant<bool, N>;

template<typename, typename>
struct pair { };

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
    static constexpr auto value = sizeof...(Ts);
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

template<typename TDefault, typename>
static no_decay<TDefault> lookup(...);

template<typename, typename TKey, typename TValue>
static no_decay<TValue> lookup(pair<TKey, TValue>*);

template<typename TDefault, typename TKey, typename T>
using at_key = decltype(lookup<TDefault, TKey>((T*)nullptr));

template<typename TDefault, typename TKey, typename T>
using at_key_t = typename at_key<TDefault, TKey, T>::type;

} // namespace di
} // namespace boost

#endif

