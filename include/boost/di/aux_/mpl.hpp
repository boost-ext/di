//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_MPL_HPP
#define BOOST_DI_AUX_MPL_HPP

#include <type_traits>

#define BOOST_DI_HAS_MEMBER_IMPL(name, member, declaration, signature)                  \
    template<typename T>                                                                \
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
        static const bool value = sizeof(test((base*)0)) == sizeof(yes_tag);            \
    }

#define BOOST_DI_HAS_MEMBER_TYPE_IMPL(name)                                             \
    template<typename T>                                                                \
    class has_##name {                                                                  \
        template <typename U> static yes_tag test(typename U::name*);                   \
        template <typename> static no_tag test(...);                                    \
                                                                                        \
    public:                                                                             \
        static const bool value = sizeof(test<T>(0)) == sizeof(yes_tag);                \
    }

#define BOOST_DI_CAT_IMPL(A, B) A ## B
#define BOOST_DI_CAT(A, B) BOOST_DI_CAT_IMPL(A, B)

#define BOOST_DI_HAS_MEMBER_FUNCTION(name, func)                                        \
    BOOST_DI_HAS_MEMBER_IMPL(name, func, void func(...) { }, void (base_impl::*)(...))

#define BOOST_DI_HAS_MEMBER(name)                                                       \
    BOOST_DI_HAS_MEMBER_IMPL(name, name, int name, int base_impl::*)

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

template<typename T, typename E>
struct pair {
    using first = T;
    using second = E;
};

template<typename...>
struct type_list {
    using type = type_list;
};

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

template<typename T, typename... TArgs>
struct is_constructible {
    using type = typename std::conditional<
        std::is_constructible<T, TArgs...>::value
      , type_list<TArgs...>
      , type_list<>
    >::type;
};

template<typename T, std::size_t>
struct type_ {
    using type = T;
};

template<typename, typename T, typename>
struct gen_type;

template<typename R, typename T, std::size_t... TArgs>
struct gen_type<R, T, index_sequence<TArgs...>>
    : is_constructible<R, typename type_<T, TArgs>::type...>
{ };

template<typename R, typename T, std::size_t N>
using genn = gen_type<R, T, typename make_index_sequence<N>::type>;

template<typename>
struct size;

template<typename... Ts>
struct size<type_list<Ts...>> {
    static const std::size_t value = sizeof...(Ts);
};

template<typename, std::size_t I, typename... Ts>
struct longest_impl;

template<typename R, typename T, typename... Ts>
struct longest_impl<R, 0, T, Ts...> {
    using type = R;
};

template<typename R, std::size_t I, typename T, typename... Ts>
struct longest_impl<R, I, T, Ts...> {
    using type = typename std::conditional<
        (size<R>::value > size<T>::value)
      , typename longest_impl<R, I - 1, Ts...>::type
      , typename longest_impl<T, I-1, Ts...>::type
    >::type;
};

template<typename... Ts>
using longest = longest_impl<type_list<>, sizeof...(Ts) - 1, Ts...>;

template<typename, std::size_t I, typename... Ts>
struct greatest_impl;

template<typename R, typename T, typename... Ts>
struct greatest_impl<R, 0, T, Ts...> {
    using type = typename std::conditional<
        (T::second::value > R::second::value)
      , typename T::first
      , typename R::first
    >::type;
};

template<typename R, std::size_t I, typename T, typename... Ts>
struct greatest_impl<R, I, T, Ts...> {
    using type = typename std::conditional<
        (T::second::value > R::second::value)
      , typename greatest_impl<T, I - 1, Ts...>::type
      , typename greatest_impl<R, I-1, Ts...>::type
    >::type;
};

template<typename T, typename... Ts>
struct greatest
    : greatest_impl<T, sizeof...(Ts) - 1, Ts...>
{ };

template<typename T>
struct greatest<T>
    : T::first
{ };

template<typename, std::size_t I, typename... Ts>
struct max_impl;

template<typename R, typename T, typename... Ts>
struct max_impl<R, 0, T, Ts...> {
    using type = typename std::conditional<
        (R::value > T::value), R, T
    >::type;
};

template<typename R, std::size_t I, typename T, typename... Ts>
struct max_impl<R, I, T, Ts...> {
    using type = typename std::conditional<
        (R::value > T::value)
      , typename max_impl<R, I - 1, Ts...>::type
      , typename max_impl<T, I-1, Ts...>::type
    >::type;
};

template<typename T, typename... Ts>
using max = max_impl<T, sizeof...(Ts) - 1, Ts...>;

template<typename, typename...>
struct times;

template<typename TMultiplicationFactor>
struct times<TMultiplicationFactor>
    : int_<1>
{ };

template<typename TMultiplicationFactor, typename T, typename... Ts>
struct times<TMultiplicationFactor, T, Ts...>
    : int_<
        TMultiplicationFactor::value *
        T::value *
        times<TMultiplicationFactor, Ts...>::value
      >
{ };

template<std::size_t...>
struct sum;

template<>
struct sum<>
    : int_<0>
{ };

template<std::size_t N, std::size_t... Ts>
struct sum<N, Ts...>
    : int_<N + sum<Ts...>::value>
{ };

template<typename>
struct is_type_list
    : std::false_type
{ };

template<typename... Ts>
struct is_type_list<type_list<Ts...>>
    : std::true_type
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

template<bool...>
struct bool_seq {
    using type = bool_seq;
};

template<bool B, bool... Ts>
struct contains_impl
    : bool_<B || contains_impl<Ts...>::value>
{ };

template<bool B>
struct contains_impl<B>
    : bool_<B>
{ };

template<typename, typename> struct contains;

template<typename T, typename... Ts>
struct contains<type_list<Ts...>, T>
    : contains_impl<std::is_same<T, Ts>::value...>
{ };

template<typename T>
struct contains<type_list<>, T>
    : bool_<false>
{ };

template<typename, typename>
struct add;

template<typename... Ts, typename T>
struct add<type_list<Ts...>, T> {
    using type = type_list<Ts..., T>;
};

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

} // namespace di
} // namespace boost

#endif

