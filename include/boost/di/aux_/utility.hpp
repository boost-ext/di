//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software type_listicense, Version 1.0.
// (See accompanying file type_listICENSE_1_0.txt or copy at http://www.boost.org/type_listICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_UTILITY_HPP
#define BOOST_DI_AUX_UTILITY_HPP

struct _ {
  _(...) {}
};

namespace aux {

using swallow = int[];

template <class T>
using owner = T;

template <class...>
struct valid {
  using type = int;
};

template <class... Ts>
using valid_t = typename valid<Ts...>::type;

template <class...>
struct type {};

struct none_type {};

template <class T, T>
struct non_type {};

template <class...>
struct always {
  static constexpr auto value = true;
};

template <class...>
struct never {
  static constexpr auto value = false;
};

template <class T, class...>
struct identity {
  using type = T;
};

template <class...>
struct type_list {
  using type = type_list;
};

template <bool...>
struct bool_list {
  using type = bool_list;
};

template <class T1, class T2>
struct pair {
  using type = pair;
  using first = T1;
  using second = T2;
};

template <class... Ts>
struct inherit : Ts... {
  using type = inherit;
};

template <class... Ts>
struct join {
  using type = type_list<>;
};

template <class T>
struct join<T> {
  using type = T;
};

template <class... T1s, class... T2s, class... Ts>
struct join<type_list<T1s...>, type_list<T2s...>, Ts...> : join<type_list<T1s..., T2s...>, Ts...> {};

template <class... Ts, class... T1s, class... T2s, class... T3s, class... T4s, class... T5s, class... T6s, class... T7s,
          class... T8s, class... T9s, class... T10s, class... T11s, class... T12s, class... T13s, class... T14s, class... T15s,
          class... T16s, class... Us>
struct join<type_list<Ts...>, type_list<T1s...>, type_list<T2s...>, type_list<T3s...>, type_list<T4s...>, type_list<T5s...>,
            type_list<T6s...>, type_list<T7s...>, type_list<T8s...>, type_list<T9s...>, type_list<T10s...>, type_list<T11s...>,
            type_list<T12s...>, type_list<T13s...>, type_list<T14s...>, type_list<T15s...>, type_list<T16s...>, Us...>
    : join<type_list<Ts..., T1s..., T2s..., T3s..., T4s..., T5s..., T6s..., T7s..., T8s..., T9s..., T10s..., T11s..., T12s...,
                     T13s..., T14s..., T15s..., T16s...>,
           Us...> {};

template <class... TArgs>
using join_t = typename join<TArgs...>::type;

template <int...>
struct index_sequence {
  using type = index_sequence;
};

#if __has_builtin(__make_integer_seq)  // __pph__
template <class T, T... Ns>
struct integer_sequence {
  using type = index_sequence<Ns...>;
};
template <int N>
struct make_index_sequence_impl {
  using type = typename __make_integer_seq<integer_sequence, int, N>::type;
};
#else   // __pph__
template <class...>
struct build_index_sequence;
template <int... Cs1, int... Cs2>
struct build_index_sequence<index_sequence<Cs1...>, index_sequence<Cs2...>> {
  using type = index_sequence<Cs1..., sizeof...(Cs1) + Cs1..., 2 * sizeof...(Cs1) + Cs2...>;
};

template <int N>
struct make_index_sequence_impl {
  using type = typename build_index_sequence<typename make_index_sequence_impl<N / 2>::type,
                                             typename make_index_sequence_impl<N % 2>::type>::type;
};
template <>
struct make_index_sequence_impl<0> : index_sequence<> {};
template <>
struct make_index_sequence_impl<1> : index_sequence<0> {};
template <>
struct make_index_sequence_impl<2> : index_sequence<0, 1> {};
template <>
struct make_index_sequence_impl<3> : index_sequence<0, 1, 2> {};
template <>
struct make_index_sequence_impl<4> : index_sequence<0, 1, 2, 3> {};
template <>
struct make_index_sequence_impl<5> : index_sequence<0, 1, 2, 3, 4> {};
template <>
struct make_index_sequence_impl<6> : index_sequence<0, 1, 2, 3, 4, 5> {};
template <>
struct make_index_sequence_impl<7> : index_sequence<0, 1, 2, 3, 4, 5, 6> {};
template <>
struct make_index_sequence_impl<8> : index_sequence<0, 1, 2, 3, 4, 5, 6, 7> {};
template <>
struct make_index_sequence_impl<9> : index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8> {};
template <>
struct make_index_sequence_impl<10> : index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9> {};
#endif  // __pph__

template <int N>
using make_index_sequence = typename make_index_sequence_impl<N>::type;
}  // namespace aux

#endif
