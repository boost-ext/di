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

#if 0
#if defined(BOOST_HAS_RVALUE_REFS) && defined(BOOST_HAS_VARIADIC_TMPL)
#endif
#endif

namespace di
{

/**
 * Example of changing default behavior using specialized class
 *
 * @code
 * #include <QDesp/Config.hpp>
 *
 * namespace di
 * {
 * template<>
 * struct Defaults<Back::Detail::Policy, di::Specialized>
 * {
 *     typedef Back::Policy<...> type;
 * };
 * } // namespace di
 * @endcode
 */
class Specialized { };
template<typename, typename = Specialized> struct Defaults;

} // namespace di

#endif

