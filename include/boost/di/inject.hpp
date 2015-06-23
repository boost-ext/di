//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_INJECT_HPP
#define BOOST_DI_INJECT_HPP

#include "boost/di/aux_/compiler_specific.hpp"
#include "boost/di/aux_/preprocessor.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"

#if !defined(BOOST_DI_INJECTOR)
    #define BOOST_DI_INJECTOR boost_di_injector__
#endif

#if !defined(BOOST_DI_CFG_CTOR_LIMIT_SIZE)
    #define BOOST_DI_CFG_CTOR_LIMIT_SIZE 10
#endif

namespace boost { namespace di {
template<class, class>
struct named_type { };

struct named_impl { template<class T> T operator=(const T&) const; };
static constexpr BOOST_DI_UNUSED named_impl named{};

template<class T, class TName>
struct combine_impl {
	using type = named_type<TName, T>;
};

template<class T>
struct combine_impl<T, aux::none_type> {
	using type = T;
};

template<class, class>
struct combine;

template<class... T1, class... T2>
struct combine<aux::type_list<T1...>, aux::type_list<T2...>> {
	using type = aux::type_list<typename combine_impl<T1, T2>::type...>;
};
}} // boost::di

#define BOOST_DI_GEN_CTOR_IMPL(p, i) \
    BOOST_DI_IF(i, BOOST_DI_COMMA, BOOST_DI_EAT)() \
    BOOST_DI_IF(BOOST_DI_IBP(p), BOOST_DI_EAT p, p)
#define BOOST_DI_GEN_CTOR(i, ...) BOOST_DI_GEN_CTOR_IMPL(BOOST_DI_ELEM(i, __VA_ARGS__,), i)
#define BOOST_DI_GEN_ARG_NAME(p) BOOST_DI_GEN_ARG_NAME_IMPL p )
#define BOOST_DI_GEN_NONE_TYPE(p) ::boost::di::aux::none_type
#define BOOST_DI_GEN_ARG_NAME_IMPL(p) decltype(::boost::di::p) BOOST_DI_EAT(
#define BOOST_DI_GEN_NAME_IMPL(p, i) \
    BOOST_DI_IF(i, BOOST_DI_COMMA, BOOST_DI_EAT)() \
    BOOST_DI_IF(BOOST_DI_IBP(p), BOOST_DI_GEN_ARG_NAME, BOOST_DI_GEN_NONE_TYPE)(p)
#define BOOST_DI_GEN_NAME(i, ...) BOOST_DI_GEN_NAME_IMPL(BOOST_DI_ELEM(i, __VA_ARGS__,), i)

#define BOOST_DI_INJECT_TRAITS_EMPTY_IMPL(...) \
    using BOOST_DI_INJECTOR BOOST_DI_UNUSED = ::boost::di::aux::type_list<>

#define BOOST_DI_INJECT_TRAITS_IMPL(...) \
    static void BOOST_DI_CAT(BOOST_DI_INJECTOR, ctor)( \
        BOOST_DI_REPEAT(BOOST_DI_SIZE(__VA_ARGS__), BOOST_DI_GEN_CTOR, __VA_ARGS__) \
    ); \
    static void BOOST_DI_CAT(BOOST_DI_INJECTOR, names)( \
        BOOST_DI_REPEAT(BOOST_DI_SIZE(__VA_ARGS__), BOOST_DI_GEN_NAME, __VA_ARGS__) \
    ); \
    using BOOST_DI_INJECTOR BOOST_DI_UNUSED = ::boost::di::combine< \
        typename ::boost::di::aux::function_traits<decltype(BOOST_DI_CAT(BOOST_DI_INJECTOR, ctor))>::args \
      , typename ::boost::di::aux::function_traits<decltype(BOOST_DI_CAT(BOOST_DI_INJECTOR, names))>::args \
    >::type; \
    static_assert( \
        BOOST_DI_SIZE(__VA_ARGS__) <= BOOST_DI_CFG_CTOR_LIMIT_SIZE \
      , "Number of constructor arguments is out of range - see BOOST_DI_CFG_CTOR_LIMIT_SIZE" \
    )

#if !defined(BOOST_DI_INJECT_TRAITS)
    #define BOOST_DI_INJECT_TRAITS(...) \
        BOOST_DI_IF( \
            BOOST_DI_IS_EMPTY(__VA_ARGS__) \
          , BOOST_DI_INJECT_TRAITS_EMPTY_IMPL \
          , BOOST_DI_INJECT_TRAITS_IMPL \
        )(__VA_ARGS__)
#endif

#if !defined(BOOST_DI_INJECT_TRAITS_NO_LIMITS)
    #define BOOST_DI_INJECT_TRAITS_NO_LIMITS(...) \
        static void BOOST_DI_CAT(BOOST_DI_INJECTOR, ctor)(__VA_ARGS__); \
        using BOOST_DI_INJECTOR BOOST_DI_UNUSED = \
            ::boost::di::aux::function_traits<decltype(BOOST_DI_CAT(BOOST_DI_INJECTOR, ctor))>::args
#endif

#if !defined(BOOST_DI_INJECT)
    #define BOOST_DI_INJECT(type, ...) \
        BOOST_DI_INJECT_TRAITS(__VA_ARGS__); \
        type(BOOST_DI_REPEAT( \
            BOOST_DI_SIZE(__VA_ARGS__) \
          , BOOST_DI_GEN_CTOR \
          , __VA_ARGS__) \
        )
#endif

#endif

