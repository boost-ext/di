//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef DI_FRONT_CTOR_HPP
#define DI_FRONT_CTOR_HPP

#include "di/config.hpp"

namespace di
{

/**
 * @code
 * struct C { C(int, double); };
 *
 * template<>
 * struct ctorTraits<C>
 * {
 *     static void ctor(int, double);
 * };
 * @endcode
 */
template<typename T> struct ctorTraits
{
    static void ctor(); //trivial ctor
};

} // namespace di

#if defined(DI_CTOR_CFG_VA_ARGS)

/**
 * class SimpleClass
 * {
 * public:
 *      DI_CTOR_TRAITS(int);
 *      SimpleClass(int) { }
 * };
 */
#define DI_CTOR_TRAITS(...)                                              \
    struct DI_CTOR_UNIQUE_NAME { static void ctor(__VA_ARGS__); }

/**
 * class SimpleClass
 * {
 * public:
 *      DI_CTOR(SimpleClass, int, double)
 *      { }
 * };
 */
#   define DI_CTOR(Type, ...)                                            \
        DI_CTOR_TRAITS(__VA_ARGS__);                                     \
        Type(__VA_ARGS__)

#elif defined(DI_CTOR_CFG_BRACKET)

/**
 * class SimpleClass
 * {
 * public:
 *      DI_CTOR_TRAITS((int));
 *      SimpleClass(int) { }
 * };
 */
#define DI_CTOR_TRAITS(Params)                                           \
    struct DI_CTOR_UNIQUE_NAME { static void ctor Params; }

/**
 * class SimpleClass
 * {
 * public:
 *      DI_CTOR(SimpleClass, (int, double))
 *      { }
 * };
 */
#   define DI_CTOR(Type, Params)                                         \
        DI_CTOR_TRAITS(Params);                                          \
        Type Params

#else
#   error DI_CTOR not defined
#endif

#endif

