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

#define BOOST_DI_HAS_MEMBER_IMPL(name, member, declaration, signature, Tag, Type)       \
    template<typename T, Type>                                                          \
    class has_##name {                                                                  \
        struct base_impl { declaration; };                                              \
        struct base                                                                     \
            : base_impl                                                                 \
            , std::conditional<std::is_class<T>::value, T, none_t>::type                \
        { base() { } };                                                                 \
                                                                                        \
        template<typename U>                                                            \
        static no_tag test(U*, non_type<signature, &U::member>* = 0);                   \
        static yes_tag test(...);                                                       \
                                                                                        \
    public:                                                                             \
        static const bool value = sizeof(test((base*)0)) == sizeof(Tag);                \
    }

#define BOOST_DI_HAS_MEMBER_TYPE_IMPL(name)                                             \
    template<typename T>                                                                \
    class has_##name {                                                                  \
        template<typename U> static yes_tag test(typename U::name*);                    \
        template<typename>   static no_tag test(...);                                   \
                                                                                        \
    public:                                                                             \
        static const bool value = sizeof(test<T>(0)) == sizeof(Tag);                    \
    }

#define BOOST_DI_CAT_IMPL(A, B) A ## B
#define BOOST_DI_CAT(A, B) BOOST_DI_CAT_IMPL(A, B)

#define BOOST_DI_HAS_MEMBER_FUNCTION(name, func)                                        \
    BOOST_DI_HAS_MEMBER_IMPL(                                                           \
        name                                                                            \
      , func                                                                            \
      , void func(...) { }                                                              \
      , void (base_impl::*)(...)                                                        \
      , yes_tag                                                                         \
      , typename = void                                                                 \
    )

#define BOOST_DI_HAS_MEMBER(name)                                                       \
    BOOST_DI_HAS_MEMBER_IMPL(                                                           \
        name                                                                            \
      , name                                                                            \
      , int name                                                                        \
      , int base_impl::*                                                                \
      , yes_tag                                                                         \
      , typename = void                                                                 \
    )

//#define BOOST_DI_HAS_MEMBER_TYPE(name) BOOST_DI_HAS_MEMBER_TYPE_IMPL(name)
#define BOOST_DI_HAS_MEMBER_TYPE(name) BOOST_DI_HAS_MEMBER(name)

namespace boost {
namespace di {

typedef char (&no_tag)[1];
typedef char (&yes_tag)[2];

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

template<std::size_t...>
struct index_sequence {
    using type = index_sequence;
};

template<bool...>
struct bool_seq {
    using type = bool_seq;
};

template<typename, typename>
struct concat;

template<std::size_t... I1, std::size_t... I2>
struct concat<index_sequence<I1...>, index_sequence<I2...>>
    : index_sequence<I1..., (sizeof...(I1) + I2)...>
{ };

template<std::size_t N>
struct make_index_sequence
    : concat<
          typename make_index_sequence<N/2>::type
        , typename make_index_sequence<N - N/2>::type
      >::type
{ };

template<>
struct make_index_sequence<0>
    : index_sequence<>
{ };

template<>
struct make_index_sequence<1>
    : index_sequence<1>
{ };

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

template<typename, typename>
struct add;

template<typename... Ts, typename T>
struct add<type_list<Ts...>, T> {
    using type = type_list<Ts..., T>;
};

template<typename, typename>
struct prepend;

template<typename T, typename... Ts>
struct prepend<T, type_list<Ts...>>
	: type_list<T, Ts...>
{ };

template<template<typename> class f, typename>
struct filter;

template<template<typename> class f, typename T, typename... Ts>
struct filter<f, type_list<T, Ts...>>
	: std::conditional<
          f<T>::value
        , typename prepend<
              T
            , typename filter<f, type_list<Ts...>>::type
          >::type
        , typename filter<f, type_list<Ts...>>::type
      >
{ };

template<template<typename> class f>
struct filter<f, type_list<>> {
    using type = type_list<>;
};

template<typename, typename>
struct concat;

template<typename T, typename... Xs, typename... Ys>
struct concat<type_list<T, Xs...>, type_list<Ys...>>
	: prepend<T, typename concat<type_list<Xs...>, type_list<Ys...>>::type>
{ };

template<typename... Ts>
struct concat<type_list<>, type_list<Ts...>>
	: type_list<Ts...>
{ };

template<typename>
struct sort;

template<typename T, typename... Ts>
struct sort<type_list<T, Ts...>> {
    template<typename U>
    using less_than = bool_<(U::scope::priority < T::scope::priority)>;

    template<typename U>
    using more_than = bool_<(U::scope::priority >= T::scope::priority)>;

    using less_stuff = typename filter<less_than, type_list<Ts...>>::type;
    using more_stuff = typename filter<more_than, type_list<Ts...>>::type;

	using type = typename concat<
        typename sort<less_stuff>::type
      , typename prepend<T, typename sort<more_stuff>::type>::type
    >::type;
};

template<typename T>
struct sort<type_list<T>>
	: type_list<T>
{ };

template<>
struct sort<type_list<>>
	: type_list<>
{ };

} // namespace di
} // namespace boost

#endif

