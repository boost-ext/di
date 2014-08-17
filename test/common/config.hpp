//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TEST_CONFIG_HPP
#define BOOST_DI_TEST_CONFIG_HPP

#if defined(__GNUC__)
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations" // std::aux::auto_ptr
#endif

#if defined(__INTEL_COMPILER)
    #pragma warning(disable:1478) // class "std::aux::auto_ptr<...>" was declared deprecated
#endif

#if defined(_MSC_VER)
    #pragma warning(disable:4127) // conditional expression is constant
#endif

#if defined(BOOST_DI_CFG_TEST_CTOR)
    #if defined(BOOST_DI_CFG_INJECT_BRACKET)
        #define BOOST_DI_INJECT(type, args) type args
    #else
        #define BOOST_DI_INJECT(type, ...) type(__VA_ARGS__)
    #endif
#endif

#endif

