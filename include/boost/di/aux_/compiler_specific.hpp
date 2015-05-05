//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_COMPILER_SPECIFIC_HPP
#define BOOST_DI_AUX_COMPILER_SPECIFIC_HPP

#if defined(__clang__)
    #define BOOST_DI_CLANG
#elif defined(__GNUC__)
    #define BOOST_DI_GCC
#elif defined(_MSC_VER)
    #define BOOST_DI_MSVC
#endif

#define BOOST_DI_WKND(T) BOOST_DI_WKND_IMPL_##T

#define BOOST_DI_WKND(T) BOOST_DI_WKND_IMPL_##T
#define BOOST_DI_WKND_NOT(T) BOOST_DI_WKND_IMPL_NOT_##T

#if defined(BOOST_DI_CLANG)
    #define BOOST_DI_UNUSED __attribute__((unused))
    #define BOOST_DI_ATTR_ERROR(...) [[deprecated(__VA_ARGS__)]]
    #define BOOST_DI_CONSTEXPR constexpr
    #define BOOST_DI_WKND_IMPL_BOOST_DI_CLANG(...) __VA_ARGS__
    #define BOOST_DI_WKND_IMPL_BOOST_DI_GCC(...)
    #define BOOST_DI_WKND_IMPL_BOOST_DI_MSVC(...)
    #define BOOST_DI_WKND_IMPL_NOT_BOOST_DI_CLANG(...)
    #define BOOST_DI_WKND_IMPL_NOT_BOOST_DI_GCC(...) __VA_ARGS__
    #define BOOST_DI_WKND_IMPL_NOT_BOOST_DI_MSVC(...) __VA_ARGS__
#elif defined(BOOST_DI_GCC)
    #define BOOST_DI_UNUSED __attribute__((unused))
    #define BOOST_DI_ATTR_ERROR(...) __attribute__ ((error(__VA_ARGS__)))
    #define BOOST_DI_CONSTEXPR constexpr
    #define BOOST_DI_WKND_IMPL_BOOST_DI_GCC(...) __VA_ARGS__
    #define BOOST_DI_WKND_IMPL_BOOST_DI_CLANG(...)
    #define BOOST_DI_WKND_IMPL_BOOST_DI_MSVC(...)
    #define BOOST_DI_WKND_IMPL_NOT_BOOST_DI_GCC(...)
    #define BOOST_DI_WKND_IMPL_NOT_BOOST_DI_CLANG(...) __VA_ARGS__
    #define BOOST_DI_WKND_IMPL_NOT_BOOST_DI_MSVC(...) __VA_ARGS__
#elif defined(BOOST_DI_MSVC)
    #define BOOST_DI_UNUSED
    #define BOOST_DI_ATTR_ERROR(...) __declspec(deprecated(__VA_ARGS__))
    #define BOOST_DI_CONSTEXPR inline
    #define BOOST_DI_WKND_IMPL_BOOST_DI_MSVC(...) __VA_ARGS__
    #define BOOST_DI_WKND_IMPL_BOOST_DI_CLANG(...)
    #define BOOST_DI_WKND_IMPL_BOOST_DI_GCC(...)
    #define BOOST_DI_WKND_IMPL_NOT_BOOST_DI_MSVC(...)
    #define BOOST_DI_WKND_IMPL_NOT_BOOST_DI_CLANG(...) __VA_ARGS__
    #define BOOST_DI_WKND_IMPL_NOT_BOOST_DI_GCC(...) __VA_ARGS__

    #pragma warning(disable : 4503) // decorated name length exceeded, name was truncated
    #pragma warning(disable : 4822) // local class member function does not have a body
#endif

#endif

