//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONFIG_HPP
#define BOOST_DI_CONFIG_HPP

#include <boost/mpl/assert.hpp>
#include <boost/config.hpp>

#if !defined(BOOST_DI_CTOR_CFG_VA_ARGS) && !defined(BOOST_DI_CTOR_CFG_BRACKET)
# define BOOST_DI_CTOR_CFG_VA_ARGS
#endif

#if !defined(BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE)
# define BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE 10
#endif

#if !defined(BOOST_DI_CTOR_UNIQUE_NAME)
# define BOOST_DI_CTOR_UNIQUE_NAME ctor__
#endif

#ifndef BOOST_DI_STATIC_ASSERT
# define BOOST_DI_STATIC_ASSERT(cond, expr, types) BOOST_MPL_ASSERT_MSG(cond, expr, types)
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

