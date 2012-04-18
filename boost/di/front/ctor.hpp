//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FRONT_CTOR_HPP
#define BOOST_DI_FRONT_CTOR_HPP

#include "di/config.hpp"

namespace di
{

/**
 * @code
 * struct C { C(int, double); };
 *
 * template<>
 * struct ctor_traits<C>
 * {
 *     static void ctor(int, double);
 * };
 * @endcode
 */
template<typename T> struct ctor_traits
{
    static void ctor(); //trivial ctor
};

} // namespace di

#if defined(BOOST_DI_CTOR_CFG_VA_ARGS)

/**
 * class SimpleClass
 * {
 * public:
 *      BOOST_DI_CTOR_TRAITS(int);
 *      SimpleClass(int) { }
 * };
 */
#define BOOST_DI_CTOR_TRAITS(...)                                              \
    struct BOOST_DI_CTOR_UNIQUE_NAME { static void ctor(__VA_ARGS__); }

/**
 * class SimpleClass
 * {
 * public:
 *      BOOST_DI_CTOR(SimpleClass, int, double)
 *      { }
 * };
 */
#   define BOOST_DI_CTOR(Type, ...)                                            \
        BOOST_DI_CTOR_TRAITS(__VA_ARGS__);                                     \
        Type(__VA_ARGS__)

#elif defined(BOOST_DI_CTOR_CFG_BRACKET)

/**
 * class SimpleClass
 * {
 * public:
 *      BOOST_DI_CTOR_TRAITS((int));
 *      SimpleClass(int) { }
 * };
 */
#define BOOST_DI_CTOR_TRAITS(Params)                                           \
    struct BOOST_DI_CTOR_UNIQUE_NAME { static void ctor Params; }

/**
 * class SimpleClass
 * {
 * public:
 *      BOOST_DI_CTOR(SimpleClass, (int, double))
 *      { }
 * };
 */
#   define BOOST_DI_CTOR(Type, Params)                                         \
        BOOST_DI_CTOR_TRAITS(Params);                                          \
        Type Params

#else
#   error BOOST_DI_CTOR not defined
#endif

#endif

