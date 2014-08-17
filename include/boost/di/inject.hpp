//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_INJECT_HPP
#define BOOST_DI_INJECT_HPP

#include "boost/di/aux_/config.hpp"

#if defined(BOOST_MSVC)
    #pragma warning(disable:4822) // local class member function does not have a body
#endif

#if defined(BOOST_INTEL)
    #pragma warning(disable:177) //function "..." was declared but never referenced
#endif

#if defined(BOOST_DI_CFG_INJECT_VA_ARGS)
    #define BOOST_DI_INJECT_TRAITS(...)                 \
        static void BOOST_DI_INJECTOR(__VA_ARGS__)

    #if !defined(BOOST_DI_INJECT)
        #define BOOST_DI_INJECT(type, ...)              \
            BOOST_DI_INJECT_TRAITS(__VA_ARGS__);        \
            type(__VA_ARGS__)
    #endif

#else
    #define BOOST_DI_INJECT_TRAITS(Args)                \
        static void BOOST_DI_INJECTOR Args

    #if !defined(BOOST_DI_INJECT)
        #define BOOST_DI_INJECT(type, args)             \
            BOOST_DI_INJECT_TRAITS(args);               \
            type args
    #endif

#endif

#if defined(BOOST_DI_CFG_NO_PREPROCESSED_HEADERS)
    #include "boost/di/type_traits/ctor_traits.hpp" // di::ctor_traits<...>
#endif

#endif

