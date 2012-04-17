//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef DI_CONFIG_HPP
#define DI_CONFIG_HPP

#include <boost/mpl/assert.hpp>
#include <boost/config.hpp>

#if !defined(DI_CTOR_CFG_VA_ARGS) && !defined(DI_CTOR_CFG_BRACKET)
# define DI_CTOR_CFG_VA_ARGS
#endif

#if !defined(DI_FUNCTION_ARITY_LIMIT_SIZE)
# define DI_FUNCTION_ARITY_LIMIT_SIZE 10
#endif

#if !defined(DI_CTOR_UNIQUE_NAME)
# define DI_CTOR_UNIQUE_NAME ctor__
#endif

#ifndef DI_STATIC_ASSERT
# define DI_STATIC_ASSERT(cond, expr, types) BOOST_MPL_ASSERT_MSG(cond, expr, types)
#endif

namespace di
{

/**
 * Example of changing default behavior using specialized class
 *
 * @code
 * #include <di/config.hpp>
 *
 * namespace di
 * {
 * template<>
 * struct defaults<back::detail::policy, di::specialized>
 * {
 *     typedef back::policy<...> type;
 * };
 * } // namespace di
 * @endcode
 */
class specialized { };
template<typename, typename = specialized> struct defaults;

} // namespace di

#endif

