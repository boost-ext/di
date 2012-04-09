//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_FRONT_CTOR_HPP
#define QDEPS_FRONT_CTOR_HPP

#include "QDeps/Config.hpp"

#if defined(QDEPS_CTOR_CFG_VA_ARGS)

/**
 * class SimpleClass
 * {
 * public:
 *      QDEPS_CTOR_TRAITS(int);
 *      SimpleClass(int) { }
 * };
 */
#define QDEPS_CTOR_TRAITS(...)                                              \
    struct QDEPS_CTOR_UNIQUE_NAME { static void ctor(__VA_ARGS__); }

/**
 * class SimpleClass
 * {
 * public:
 *      QDEPS_CTOR(SimpleClass, int, double)
 *      { }
 * };
 */
#   define QDEPS_CTOR(Type, ...)                                            \
        QDEPS_CTOR_TRAITS(__VA_ARGS__);                                     \
        Type(__VA_ARGS__)

#elif defined(QDEPS_CTOR_CFG_BRACKET)

/**
 * class SimpleClass
 * {
 * public:
 *      QDEPS_CTOR_TRAITS((int));
 *      SimpleClass(int) { }
 * };
 */
#define QDEPS_CTOR_TRAITS(Params)                                           \
    struct QDEPS_CTOR_UNIQUE_NAME { static void ctor Params; }

/**
 * class SimpleClass
 * {
 * public:
 *      QDEPS_CTOR(SimpleClass, (int, double))
 *      { }
 * };
 */
#   define QDEPS_CTOR(Type, Params)                                         \
        QDEPS_CTOR_TRAITS(Params);                                          \
        Type Params

#else
#   error QDEPS_CTOR not defined
#endif

#endif

