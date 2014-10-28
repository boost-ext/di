//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_INJECT_HPP
#define BOOST_DI_INJECT_HPP

#include "boost/di/aux_/config.hpp"

#if !defined(BOOST_DI_INJECTOR)
    #define BOOST_DI_INJECTOR boost_di_injector__
#endif

#if !defined(BOOST_DI_CFG_CTOR_LIMIT_SIZE)
    #define BOOST_DI_CFG_CTOR_LIMIT_SIZE 10
#endif

#if !defined(BOOST_DI_CFG_INJECT_VA_ARGS) && !defined(BOOST_DI_CFG_INJECT_BRACKET)
    #define BOOST_DI_CFG_INJECT_VA_ARGS
#endif

#if defined(_MSC_VER)
    #pragma warning(disable:4822) // local class member function does not have a body
#endif

#if defined(__INTEL_COMPILER)
    #pragma warning(disable:177) //function "..." was declared but never referenced
#endif

#define BOOST_DI_INJECT_TRAITS(...)             \
    static void BOOST_DI_INJECTOR(__VA_ARGS__)

#if !defined(BOOST_DI_INJECT)
    #define BOOST_DI_INJECT(type, ...)          \
        BOOST_DI_INJECT_TRAITS(__VA_ARGS__);    \
        type(__VA_ARGS__)
#endif

#if defined(BOOST_DI_CFG_NO_PREPROCESSED_HEADERS)
    #include "boost/di/type_traits/ctor_traits.hpp" // di::ctor_traits<...>
#endif

#endif

