//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_CONFIG_HPP
#define BOOST_DI_AUX_CONFIG_HPP

#include <boost/config.hpp>

#if !defined(BOOST_DI_INJECTOR)
    #define BOOST_DI_INJECTOR boost_di_injector__
#endif

#if !defined(BOOST_DI_CFG_CTOR_LIMIT_SIZE)
    #define BOOST_DI_CFG_CTOR_LIMIT_SIZE 10
#endif

#define BOOST_DI_FEATURE(feature) \
    BOOST_DI_FEATURE_##feature

#if defined(BOOST_DI_CFG_EXAMINE_CALL_STACK)
    #define BOOST_DI_FEATURE_EXAMINE_CALL_STACK(...) __VA_ARGS__
    #define BOOST_DI_FEATURE_NO_EXAMINE_CALL_STACK(...)
#else
    #define BOOST_DI_FEATURE_EXAMINE_CALL_STACK(...)
    #define BOOST_DI_FEATURE_NO_EXAMINE_CALL_STACK(...) __VA_ARGS__
#endif

#define BOOST_DI_WKND(wknd) \
    BOOST_DI_WKND_##wknd

#if defined(BOOST_MSVC)
    #define BOOST_DI_WKND_MSVC(...) __VA_ARGS__
    #define BOOST_DI_WKND_NO_MSVC(...)
#else
    #define BOOST_DI_WKND_MSVC(...)
    #define BOOST_DI_WKND_NO_MSVC(...) __VA_ARGS__
#endif

#if !defined(BOOST_DI_ASSERT_MSG)
    #define BOOST_DI_ASSERT_MSG(condition, msg) \
        static_assert(condition, msg)
#endif

#endif

