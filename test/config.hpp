//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TEST_CONFIG_HPP
#define BOOST_DI_TEST_CONFIG_HPP

#include <boost/config.hpp>

#if defined(BOOST_NO_CXX11_SMART_PTR) &&    \
    __clang_major__ >= 3 &&                 \
    __clang_minor__ >= 2 &&                 \
    __cplusplus >= 201100L
    #undef BOOST_NO_CXX11_SMART_PTR
#endif

#if defined(BOOST_GCC) || defined(BOOST_CLANG)
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations" // std::aux::auto_ptr
#endif

#if defined(BOOST_INTEL)
    #pragma warning(disable:1478) //class "std::aux::auto_ptr<...>" was declared deprecated
#endif

#endif

