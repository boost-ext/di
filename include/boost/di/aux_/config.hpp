//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_CONFIG_HPP
#define BOOST_DI_AUX_CONFIG_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/mpl/limits/vector.hpp>

#if !defined(BOOST_DI_INJECTOR)
    #define BOOST_DI_INJECTOR boost_di_injector__
#endif

#if !defined(BOOST_DI_CFG_CTOR_LIMIT_SIZE)
    #define BOOST_DI_CFG_CTOR_LIMIT_SIZE 10
#endif

#if (__cplusplus >= 201100L) &&                                 \
    !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES) &&              \
    !defined(BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS) &&  \
    !defined(BOOST_NO_CXX11_RVALUE_REFERENCES) &&               \
    !defined(BOOST_NO_CXX11_DECLTYPE)
    #define BOOST_DI_CFG_STD cpp_11
#else
    #define BOOST_DI_CFG_STD cpp_03
#endif

#if defined(BOOST_DI_CFG_NO_PREPROCESSED_HEADERS) || \
    (BOOST_DI_CFG_CTOR_LIMIT_SIZE % 10) ||           \
    (BOOST_MPL_LIMIT_VECTOR_SIZE % 10)

    #define BOOST_DI_CFG_FILE boost/di/aux_/config/BOOST_DI_CFG_STD.hpp
    #include BOOST_PP_STRINGIZE(BOOST_DI_CFG_FILE)
    #undef BOOST_DI_CFG_FILE

#endif

#if defined(BOOST_GCC) || defined(BOOST_CLANG)
    #pragma GCC system_header
    #pragma GCC diagnostic ignored "-Wvariadic-macros"
#endif

#define BOOST_DI_FEATURE(feature) \
    BOOST_DI_FEATURE_##feature

#if defined(BOOST_NO_RVALUE_REFERENCES)
    #define BOOST_DI_FEATURE_RVALUE_REFERENCES(...)
    #define BOOST_DI_FEATURE_NO_RVALUE_REFERENCES(...) __VA_ARGS__
#else
    #define BOOST_DI_FEATURE_RVALUE_REFERENCES(...) __VA_ARGS__
    #define BOOST_DI_FEATURE_NO_RVALUE_REFERENCES(...)
#endif

#if defined(BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS)
    #define BOOST_DI_FEATURE_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS(...) __VA_ARGS__
    #define BOOST_DI_FEATURE_FUNCTION_TEMPLATE_DEFAULT_ARGS(...)
#else
    #define BOOST_DI_FEATURE_FUNCTION_TEMPLATE_DEFAULT_ARGS(...) __VA_ARGS__
    #define BOOST_DI_FEATURE_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS(...)
#endif

#if defined(BOOST_NO_CXX11_DECLTYPE)
    #include <boost/typeof/typeof.hpp>
    #define BOOST_DI_FEATURE_DECLTYPE(...) BOOST_TYPEOF_TPL(__VA_ARGS__)
#else
    #define BOOST_DI_FEATURE_DECLTYPE(...) decltype(__VA_ARGS__)
#endif

#if (defined(BOOST_NO_CXX11_TYPE_TRAITS) || (__cplusplus < 201100L)) && !defined(BOOST_MSVC) 
    #define BOOST_DI_FEATURE_CPP_11_TYPE_TRAITS(...)
#else
    #include <type_traits>
    #define BOOST_DI_FEATURE_CPP_11_TYPE_TRAITS(...) __VA_ARGS__
#endif

#if (defined(BOOST_NO_CXX11_FUNCTIONAL) || (__cplusplus < 201100L)) && !defined(BOOST_MSVC) 
    #define BOOST_DI_FEATURE_CPP_11_FUNCTIONAL(...)
#else
    #include <functional>
    #define BOOST_DI_FEATURE_CPP_11_FUNCTIONAL(...) __VA_ARGS__
#endif

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
    #include <boost/mpl/assert.hpp>
    #define BOOST_DI_ASSERT_MSG(c, msg, type)   \
        BOOST_MPL_ASSERT_MSG(c, msg, (type))
#endif

#endif

