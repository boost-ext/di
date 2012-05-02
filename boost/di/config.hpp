//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONFIG_HPP
#define BOOST_DI_CONFIG_HPP

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/config.hpp>

#if !defined(BOOST_DI_CTOR_CFG_VA_ARGS)                 \
 && !defined(BOOST_DI_CTOR_CFG_BRACKET)
# define BOOST_DI_CTOR_CFG_VA_ARGS
#endif

#if !defined(BOOST_DI_CTOR_UNIQUE_NAME)
# define BOOST_DI_CTOR_UNIQUE_NAME ctor__
#endif

#if defined(BOOST_HAS_VARIADIC_TMPL)                    \
 && defined(BOOST_HAS_RVALUE_REFERENCES)

# define BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE 0 // infinity

#define BOOST_DI_ITERATION_PARAMS(start, limit, file)   \
    3, (0, 0, file)

# define BOOST_DI_ARGS_TYPES_MPL(Args)                  \
    typename... Args

# define BOOST_DI_ARGS_MPL(Args)                        \
    Args...

# define BOOST_DI_ARGS_TYPES(Args)                      \
    typename... Args

# define BOOST_DI_ARGS(Args, args)                      \
    Args&&... args

# define BOOST_DI_ARGS_NOT_USED(Args)                   \
    Args&&...

# define BOOST_DI_ARGS_PASS(args)                       \
    args

# define BOOST_DI_ARGS_PASS_TYPES(Args)                 \
    Args...

# define BOOST_DI_ARGS_FORWARD(args)                    \
    std::forward(args)...

#else

# if !defined(BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE)
#  define BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE 10
# endif

#define BOOST_DI_ITERATION_PARAMS(start, limit, file)   \
    3, (start, limit, file)

# define BOOST_DI_ARGS_TYPES_MPL(Arg)                   \
    BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(                \
        BOOST_MPL_LIMIT_VECTOR_SIZE                     \
      , typename Arg                                    \
      , mpl_::na                                        \
    )

# define BOOST_DI_ARGS_MPL(Arg)                         \
    BOOST_PP_ENUM_PARAMS(                               \
        BOOST_MPL_LIMIT_VECTOR_SIZE                     \
      , Arg                                             \
    )

# define BOOST_DI_ARGS_TYPES(Arg)                       \
    BOOST_PP_ENUM_PARAMS(                               \
        BOOST_PP_ITERATION()                            \
      , typename Arg                                    \
    )

# define BOOST_DI_ARGS(Arg, arg)                        \
    BOOST_PP_ENUM_BINARY_PARAMS(                        \
        BOOST_PP_ITERATION()                            \
      , const Arg                                       \
      , &arg                                            \
    )

# define BOOST_DI_ARGS_NOT_USED(Arg)                    \
    BOOST_PP_ENUM_BINARY_PARAMS(                        \
        BOOST_PP_ITERATION()                            \
      , const Arg                                       \
      , & BOOST_PP_INTERCEPT                            \
    )

# define BOOST_DI_ARGS_PASS(arg)                        \
    BOOST_PP_ENUM_PARAMS(                               \
        BOOST_PP_ITERATION()                            \
      , arg                                             \
    )

# define BOOST_DI_ARGS_PASS_TYPES(Args)                 \
    BOOST_PP_ENUM_PARAMS(                               \
        BOOST_PP_ITERATION()                            \
      , Args                                            \
    )

# define BOOST_DI_ARGS_FORWARD(arg)                     \
    BOOST_PP_ENUM_PARAMS(                               \
        BOOST_PP_ITERATION()                            \
      , arg                                             \
    )

#endif

namespace boost {
namespace di {

/**
 * example of changing default behavior using specialized class
 *
 * @code
 * #include <boost/di/config.hpp>
 *
 * namespace boost {
 * namespace di {
 *     template<>
 *     struct defaults<detail::policy, specialized>
 *     {
 *         typedef policy<
 *             polices::check_for_circular_dependencies
 *         > type;
 *     };
 * } // namespace di
 * } // namespace boost
 * @endcode
 */
class specialized { };

template<typename, typename = specialized>
struct defaults;

} // namespace di
} // namespace boost

#endif

