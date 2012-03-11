//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_FRONT_INJECT_HPP
#define QDEPS_FRONT_INJECT_HPP

#include "QDeps/Config.hpp"

#if defined(QDEPS_INJECT_CFG_VA_ARGS)

#   ifdef __GNUC__
#       pragma GCC system_header
#   endif

/**
 * class SimpleClass
 * {
 * public:
 *      QDEPS_INJECT_TRAITS(int);
 *      SimpleClass(int) { }
 * };
 */
#define QDEPS_INJECT_TRAITS(...)                                               \
    struct inject__ { static void inject(__VA_ARGS__); }

/**
 * class SimpleClass
 * {
 * public:
 *      QDEPS_INJECT(SimpleClass, int, double)
 *      { }
 * };
 */
#   define QDEPS_INJECT(Type, ...)                                        \
        QDEPS_INJECT_TRAITS(__VA_ARGS__);                                      \
        Type(__VA_ARGS__)

#elif defined(QDEPS_INJECT_CFG_BRACKET)

/**
 * class SimpleClass
 * {
 * public:
 *      QDEPS_INJECT_TRAITS((int));
 *      SimpleClass(int) { }
 * };
 */
#define QDEPS_INJECT_TRAITS(Params)                                            \
    struct inject__ { static void inject Params; }

/**
 * class SimpleClass
 * {
 * public:
 *      QDEPS_INJECT(SimpleClass, (int, double))
 *      { }
 * };
 */
#   define QDEPS_INJECT(Type, Params)                                     \
        QDEPS_INJECT_TRAITS(Params);                                           \
        Type Params

#else
#   error QDEPS_INJECT not defined
#endif

#endif

