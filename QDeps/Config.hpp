//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_CONFIG_HPP
#define QDEPS_CONFIG_HPP

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/config.hpp>

#if !defined(QDEPS_CTOR_CFG_VA_ARGS) && !defined(QDEPS_CTOR_CFG_BRACKET)
# define QDEPS_CTOR_CFG_VA_ARGS
#endif

#if !defined(QDEPS_FUNCTION_ARITY_LIMIT_SIZE)
# define QDEPS_FUNCTION_ARITY_LIMIT_SIZE 10
#endif

#if !defined(QDEPS_CTOR_UNIQUE_NAME)
# define QDEPS_CTOR_UNIQUE_NAME ctor__
#endif

#ifndef QDEPS_STATIC_ASSERT
# define QDEPS_STATIC_ASSERT(cond, expr, types) BOOST_MPL_ASSERT_MSG(cond, expr, types)
#endif

#if defined(BOOST_HAS_RVALUE_REFS) && defined(BOOST_HAS_VARIADIC_TMPL)
# define QDEPS_ITERATION(begin, file)
# define QDEPS_VARIADIC_TYPENAME_ARGS typename... Arg
# define QDEPS_VARIADIC_ARGS Args&&... p_args
# define QDEPS_VARIADIC_FORWARD_ARGS std::forward<Args>(p_args)...
#else
# define QDEPS_ITERATION(begin, file) BOOST_PP_ITERATION_PARAMS_1 (3, (begin, QDEPS_FUNCTION_ARITY_LIMIT_SIZE, file))
# define QDEPS_VARIADIC_TYPENAME_ARGS BOOST_PP_COMMA_IF(BOOST_PP_ITERATION()) BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)
# define QDEPS_VARIADIC_ARGS BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &p_arg)
# define QDEPS_VARIADIC_FORWARD_ARGS BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), p_arg)
#endif

namespace QDeps
{

/**
 * Example of changing default behavior using specialized class
 *
 * @code
 * #include <QDesp/Config.hpp>
 *
 * namespace QDeps
 * {
 * template<>
 * struct Defaults<Back::Detail::Policy, QDeps::Specialized>
 * {
 *     typedef Back::Policy<...> type;
 * };
 * } // namespace QDeps
 * @endcode
 */
class Specialized { };
template<typename, typename = Specialized> struct Defaults;

} // namespace QDeps

#endif

