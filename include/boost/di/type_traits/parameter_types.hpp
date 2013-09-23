//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_TYPE_TRAITS_PARAMETER_TYPES_HPP
    #define BOOST_DI_TYPE_TRAITS_PARAMETER_TYPES_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/mpl/vector.hpp>

    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 ( \
        BOOST_DI_ITERATION_PARAMS(1, "boost/di/type_traits/parameter_types.hpp"))

    namespace boost {
    namespace di {
    namespace type_traits {

    template<typename>
    struct parameter_types
    {
        typedef mpl::vector0<> type;
    };

    #include BOOST_PP_ITERATE()

    } // namespace type_traits
    } // namespace di
    } // namespace boost

    #endif

#else

    template<typename R, BOOST_DI_TYPES(Args)>                  \
    struct parameter_types<R(*)(BOOST_DI_TYPES_PASS(Args))>     \
    {                                                           \
        typedef mpl::vector<BOOST_DI_TYPES_PASS(Args)> type;    \
    };

    template<typename R, typename T, BOOST_DI_TYPES(Args)>      \
    struct parameter_types<R(T::*)(BOOST_DI_TYPES_PASS(Args))>  \
    {                                                           \
        typedef mpl::vector<BOOST_DI_TYPES_PASS(Args)> type;    \
    };

#endif

