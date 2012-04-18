//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CTOR_HPP
#define BOOST_DI_CTOR_HPP

#include "boost/di/config.hpp"

namespace boost {
namespace di {

/**
 * @code
 * struct c { c(int, double); };
 *
 * template<>
 * struct ctor_traits<c>
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
} // namespace boost

#if defined(BOOST_DI_CTOR_CFG_VA_ARGS)

/**
 * class simple_class
 * {
 * public:
 *      BOOST_DI_CTOR_TRAITS(int);
 *      simple_class(int) { }
 * };
 */
#define BOOST_DI_CTOR_TRAITS(...)                                              \
    struct BOOST_DI_CTOR_UNIQUE_NAME { static void ctor(__VA_ARGS__); }

/**
 * class simple_class
 * {
 * public:
 *      BOOST_DI_CTOR(simple_class, int, double)
 *      { }
 * };
 */
#   define BOOST_DI_CTOR(type, ...)                                            \
        BOOST_DI_CTOR_TRAITS(__VA_ARGS__);                                     \
        type(__VA_ARGS__)

#elif defined(BOOST_DI_CTOR_CFG_BRACKET)

/**
 * class simple_class
 * {
 * public:
 *      BOOST_DI_CTOR_TRAITS((int));
 *      simple_class(int) { }
 * };
 */
#define BOOST_DI_CTOR_TRAITS(Params)                                           \
    struct BOOST_DI_CTOR_UNIQUE_NAME { static void ctor Params; }

/**
 * class simple_class
 * {
 * public:
 *      BOOST_DI_CTOR(simple_class, (int, double))
 *      { }
 * };
 */
#   define BOOST_DI_CTOR(type, params)                                         \
        BOOST_DI_CTOR_TRAITS(params);                                          \
        type params

#else
#   error BOOST_DI_CTOR not defined
#endif

#endif

