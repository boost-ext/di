//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_PARAMETER_TYPES_HPP
#define BOOST_DI_TYPE_TRAITS_PARAMETER_TYPES_HPP

#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/mpl/vector.hpp>

#include "boost/di/config.hpp"

namespace boost {
namespace di {
namespace type_traits {

template<typename>
struct parameter_types
{
    typedef mpl::vector0<> type;
};

#define BOOST_PP_LOCAL_MACRO(n)                                             \
    template<typename R, BOOST_PP_ENUM_PARAMS(n, typename Arg)>             \
    struct parameter_types<R(*)(BOOST_PP_ENUM_PARAMS(n, Arg))>              \
    {                                                                       \
        typedef mpl::vector##n<BOOST_PP_ENUM_PARAMS(n, Arg)> type;          \
    };                                                                      \
                                                                            \
    template<typename R, typename C, BOOST_PP_ENUM_PARAMS(n, typename Arg)> \
    struct parameter_types<R(C::*)(BOOST_PP_ENUM_PARAMS(n, Arg))>           \
    {                                                                       \
        typedef mpl::vector##n<BOOST_PP_ENUM_PARAMS(n, Arg)> type;          \
    };

#define BOOST_PP_LOCAL_LIMITS (1, BOOST_DI_LIMIT_SIZE)
#include BOOST_PP_LOCAL_ITERATE()

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

