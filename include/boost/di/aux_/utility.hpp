//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_UTILITY_HPP
#define BOOST_DI_AUX_UTILITY_HPP

#include "boost/di/aux_/type_traits.hpp"

namespace boost { namespace di { inline namespace v1 {

struct _ { _(...) { } };

namespace aux {

template<class...>
struct type { };

struct none_type { };

template<class T, T>
struct non_type { };

template<class T>
using owner = T;

template<class...>
struct valid_t { using type = int; };

template<class...>
struct always : true_type { };

template<class...>
struct never : false_type { };

template<class, class>
struct pair { using type = pair; };

template<bool...>
struct bool_list { using type = bool_list; };

template<class... Ts>
struct inherit : Ts... { using type = inherit; };

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
struct not_unique : false_type {
    using type = not_unique;
};

template<>
struct not_unique<> : true_type {
    using type = not_unique;
};

template<class T>
struct is_unique_impl<T> : not_unique<> { };

template<class T1, class T2, class... Ts>
struct is_unique_impl<T1, T2, Ts...>
    : conditional_t<
          is_base_of<type<T2>, T1>::value
        , not_unique<T2>
        , is_unique_impl<inherit<T1, type<T2>>, Ts...>
      >
{ };

template<class... Ts>
using is_unique = is_unique_impl<none_type, Ts...>;

template<int...>
struct index_sequence {
    using type = index_sequence;
};

template<int>
struct make_index_sequence_impl;

template<>
struct make_index_sequence_impl<0> : index_sequence<> { };

template<>
struct make_index_sequence_impl<1> : index_sequence<1> { };

template<>
struct make_index_sequence_impl<2> : index_sequence<1, 2> { };

template<>
struct make_index_sequence_impl<3> : index_sequence<1, 2, 3> { };

template<>
struct make_index_sequence_impl<4> : index_sequence<1, 2, 3, 4> { };

template<>
struct make_index_sequence_impl<5> : index_sequence<1, 2, 3, 4, 5> { };

template<>
struct make_index_sequence_impl<6> : index_sequence<1, 2, 3, 4, 5, 6> { };

template<>
struct make_index_sequence_impl<7> : index_sequence<1, 2, 3, 4, 5, 6, 7> { };

template<>
struct make_index_sequence_impl<8> : index_sequence<1, 2, 3, 4, 5, 6, 7, 8> { };

template<>
struct make_index_sequence_impl<9> : index_sequence<1, 2, 3, 4, 5, 6, 7, 8, 9> { };

template<>
struct make_index_sequence_impl<10> : index_sequence<1, 2, 3, 4, 5, 6, 7, 8, 9, 10> { };

template<int N>
using make_index_sequence = typename make_index_sequence_impl<N>::type;

}}}} // boost::di::v1::aux

#endif

