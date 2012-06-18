//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONFIG_HPP
#define BOOST_DI_CONFIG_HPP

#include <memory>

#include "boost/di/policy.hpp"

#if !defined(BOOST_DI_CTOR_CFG_VA_ARGS) && !defined(BOOST_DI_CTOR_CFG_BRACKET)
    #define BOOST_DI_CTOR_CFG_VA_ARGS
#endif

#if !defined(BOOST_DI_CTOR_UNIQUE_NAME)
    #define BOOST_DI_CTOR_UNIQUE_NAME boost_di_ctor
#endif

#if !defined(BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE)
    #define BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE 10
#endif

namespace boost {
namespace di {

namespace policies {
class check_for_binding_correctness;
class check_for_circular_dependencies;
class check_for_creation_ownership;
} // namespace policies

class specialized { };

/**
 * @code
 * #include <boost/di/config.hpp>
 *
 * namespace boost {
 * namespace di {
 *
 * template<>
 * struct config<specialized>
 *     : config<>
 * {
 *     typedef policy<
 *         di::policies::check_for_circular_dependencies
 *     > policies;
 * };
 *
 * } // namespace di
 * } // namespace boost
 * @endcode
 */
template<typename TDefault = void>
struct config
{
    typedef policy<
        policies::check_for_binding_correctness
      , policies::check_for_circular_dependencies
      , policies::check_for_creation_ownership
    > policies;

    template<typename T>
    struct allocator
        : std::allocator<T>
    { };
};

} // namespace di
} // namespace boost

#endif

