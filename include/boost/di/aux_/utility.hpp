//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_UTILITY_HPP
#define BOOST_DI_AUX_UTILITY_HPP

#define BOOST_DI_COMMA() ,
#define BOOST_DI_NONE()
#define BOOST_DI_PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__
#define BOOST_DI_CAT(a, ...) BOOST_DI_PRIMITIVE_CAT(a, __VA_ARGS__)
#define BOOST_DI_CALL(m, ...) m(__VA_ARGS__)
#define BOOST_DI_IBP_SPLIT(i, ...) BOOST_DI_PRIMITIVE_CAT(BOOST_DI_IBP_SPLIT_, i)(__VA_ARGS__)
#define BOOST_DI_IBP_SPLIT_0(a, ...) a
#define BOOST_DI_IBP_SPLIT_1(a, ...) __VA_ARGS__
#define BOOST_DI_IBP_IS_VARIADIC_C(...) 1
#define BOOST_DI_IBP_IS_VARIADIC_R_1 1,
#define BOOST_DI_IBP_IS_VARIADIC_R_BOOST_DI_IBP_IS_VARIADIC_C 0,
#define BOOST_DI_IBP(...) BOOST_DI_IBP_SPLIT(0, BOOST_DI_CAT(BOOST_DI_IBP_IS_VARIADIC_R_, BOOST_DI_IBP_IS_VARIADIC_C __VA_ARGS__))
#define BOOST_DI_IF(c) BOOST_DI_PRIMITIVE_CAT(BOOST_DI_IF_, c)
#define BOOST_DI_IF_0(t, ...) __VA_ARGS__
#define BOOST_DI_IF_1(t, ...) t
#define BOOST_DI_VA_NARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...) N
#define BOOST_DI_VA_NARGS(...) BOOST_DI_VA_NARGS_IMPL(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define BOOST_DI_VARARG_IMPL2(m, count, ...) m##count(__VA_ARGS__)
#define BOOST_DI_VARARG_IMPL(m, count, ...) BOOST_DI_VARARG_IMPL2(m, count, __VA_ARGS__)
#define BOOST_DI_VAR_ARG(m, ...) BOOST_DI_VARARG_IMPL(m, BOOST_DI_VA_NARGS(__VA_ARGS__), __VA_ARGS__)

namespace boost { namespace di { namespace aux {

struct none_t { };

template<class>
struct type { };

template<class T, T>
struct non_type { };

template<class, class>
struct pair { using type = pair; };

template<class T>
struct no_decay { using type = T; };

template<class... TArgs>
struct inherit : TArgs... { };

template<class...>
struct type_list { using type = type_list; };

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

template<class TDefault, class>
static no_decay<TDefault> lookup(...);

template<class, class TKey, class TValue>
static no_decay<TValue> lookup(pair<TKey, TValue>*);

template<class TDefault, class TKey, class... Ts>
using at_key = decltype(lookup<TDefault, TKey>((inherit<Ts...>*)0));

template<class TDefault, class TKey, class... Ts>
using at_key_t = typename at_key<TDefault, TKey, Ts...>::type;

}}} // boost::di::aux

#endif

