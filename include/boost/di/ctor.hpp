//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CTOR_HPP
#define BOOST_DI_CTOR_HPP

#include "boost/di/config.hpp"
#include "boost/di/type_traits/ctor_traits.hpp" // di::ctor_traits<...>

#ifdef BOOST_MSVC
    #pragma warning(disable:4822) // local class member function does not have a body
#endif

#if defined(__INTEL_COMPILER)
    #pragma warning(disable:177) //function "..." was declared but never referenced
#endif

#if defined(BOOST_DI_CTOR_CFG_VA_ARGS)

    #if defined(__GNUC__) && (__GNUC__ >= 4)
        #pragma GCC system_header
    #endif

    #define BOOST_DI_CTOR_TRAITS(...)                   \
        static void BOOST_DI_CONSTRUCTOR(__VA_ARGS__)   \

    #define BOOST_DI_CTOR(type, ...)                    \
        BOOST_DI_CTOR_TRAITS(__VA_ARGS__);              \
        type(__VA_ARGS__)

#elif defined(BOOST_DI_CTOR_CFG_BRACKET)

    #define BOOST_DI_CTOR_TRAITS(Params)                \
        static void BOOST_DI_CONSTRUCTOR Params         \

    #define BOOST_DI_CTOR(type, params)                 \
        BOOST_DI_CTOR_TRAITS(params);                   \
        type params

#else
    #error BOOST_DI_CTOR not defined
#endif

#endif

