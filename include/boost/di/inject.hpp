//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_INJECT_HPP
#define BOOST_DI_INJECT_HPP

#include <type_traits>
#include "boost/di/aux_/preprocessor.hpp"
#include "boost/di/aux_/utility.hpp"

#if !defined(BOOST_DI_INJECTOR)
    #define BOOST_DI_INJECTOR boost_di_injector__
#endif

#if !defined(BOOST_DI_CFG_CTOR_LIMIT_SIZE)
    #define BOOST_DI_CFG_CTOR_LIMIT_SIZE 10
#endif

#define BOOST_DI_GEN_ARGS(i, ...) BOOST_DI_GEN_ARGS_IMPL(BOOST_DI_VARARG_IMPL(ARG, i, __VA_ARGS__,), i)
#define BOOST_DI_GEN_ARGS_IMPL(p, i) struct arg##i { BOOST_DI_IF(BOOST_DI_IBP(p))(BOOST_DI_GEN_NAME_ARG p BOOST_DI_GEN_NAME_ARG_END(), BOOST_DI_GEN_ARG(p)) };
#define BOOST_DI_GEN_NAME_ARG(p) static auto BOOST_DI_CAT(BOOST_DI_INJECTOR, name)() { auto p; return named; } static void BOOST_DI_CAT(BOOST_DI_INJECTOR, arg)(
#define BOOST_DI_GEN_NAME_ARG_END() );
#define BOOST_DI_GEN_ARG(p) static void BOOST_DI_CAT(BOOST_DI_INJECTOR, arg)(p);
#define BOOST_DI_GEN_CTOR(i, ...) BOOST_DI_GEN_CTOR_IMPL(BOOST_DI_VARARG_IMPL(ARG, i, __VA_ARGS__,), i)
#define BOOST_DI_GEN_CTOR_IMPL(p, i) BOOST_DI_IF(i)(BOOST_DI_COMMA(),) BOOST_DI_IF(BOOST_DI_IBP(p))(EAT p, p)
#define BOOST_DI_GEN_TYPE_LIST(i, ...) BOOST_DI_GEN_TYPE_LIST_IMPL(BOOST_DI_VARARG_IMPL(ARG, i, __VA_ARGS__,), i)
#define BOOST_DI_GEN_TYPE_LIST_IMPL(p, n)  BOOST_DI_IF(n)(BOOST_DI_COMMA(),) BOOST_DI_IF(BOOST_DI_IBP(p))( \
    const ::boost::di::aux::type<arg##n BOOST_DI_COMMA() ::std::true_type>& \
  , BOOST_DI_IF(ISEMPTY(p))(,const ::boost::di::aux::type<arg##n BOOST_DI_COMMA() ::std::false_type>&))

#if !defined(BOOST_DI_INJECT_TRAITS)
    #define BOOST_DI_INJECT_TRAITS(...) \
        struct BOOST_DI_INJECTOR { \
            EVAL(REPEAT(PP_NARG(__VA_ARGS__), BOOST_DI_GEN_ARGS, __VA_ARGS__)) \
            using type = ::boost::di::aux::type_list<EVAL(REPEAT(PP_NARG(__VA_ARGS__), BOOST_DI_GEN_TYPE_LIST, __VA_ARGS__))>; \
            static_assert(PP_NARG(__VA_ARGS__) <= BOOST_DI_CFG_CTOR_LIMIT_SIZE, "Number of constructor arguments is out of range - see BOOST_DI_CFG_CTOR_LIMIT_SIZE"); \
        }
#endif

#if !defined(BOOST_DI_INJECT)
    #define BOOST_DI_INJECT(type, ...) \
        BOOST_DI_INJECT_TRAITS(__VA_ARGS__); \
        type(EVAL(REPEAT(PP_NARG(__VA_ARGS__), BOOST_DI_GEN_CTOR, __VA_ARGS__)))
#endif

#endif

