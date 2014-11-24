//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TEST_CONFIG_HPP
#define BOOST_DI_TEST_CONFIG_HPP

#include <boost/config.hpp>
#include <boost/version.hpp>

#if defined(BOOST_GCC) || defined(BOOST_CLANG)
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations" // std::aux::auto_ptr
#endif

#if defined(BOOST_INTEL)
    #pragma warning(disable:1478) // class "std::aux::auto_ptr<...>" was declared deprecated
#endif

#if defined(BOOST_MSVC)
    #pragma warning(disable:4127) // conditional expression is constant

    #if (BOOST_VERSION < 105500)
        #undef BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
    #endif
#endif

#if defined(BOOST_DI_CFG_TEST_CTOR)
    #if defined(BOOST_DI_CFG_INJECT_BRACKET)
        #define BOOST_DI_INJECT(type, args) type args
    #else
        #define BOOST_DI_INJECT(type, ...) type(__VA_ARGS__)
    #endif
#endif

#endif

