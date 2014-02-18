//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_TYPE_TRAITS_PARAMETER_TYPES_HPP
    #define BOOST_DI_TYPE_TRAITS_PARAMETER_TYPES_HPP

    #include "boost/di/aux_/config.hpp"

    #include <boost/mpl/vector.hpp>

    namespace boost {
    namespace di {
    namespace type_traits {

    template<typename>
    struct parameter_types;

    template<typename R>
    struct parameter_types<R(*)()>
    {
        typedef R result_type;
        typedef mpl::vector0<> type;
    };

    template<typename R, typename T>
    struct parameter_types<R(T::*)()>
    {
        typedef R result_type;
        typedef mpl::vector0<> type;
    };

    template<typename R, typename T>
    struct parameter_types<R(T::*)() const>
    {
        typedef R result_type;
        typedef mpl::vector0<> type;
    };

    #define BOOST_PP_FILENAME_1 "boost/di/type_traits/parameter_types.hpp"
    #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM(1)
    #include BOOST_PP_ITERATE()

    } // namespace type_traits
    } // namespace di
    } // namespace boost

    #endif

#else

    template<typename R, BOOST_DI_TYPES(Args)>                          \
    struct parameter_types<R(*)(BOOST_DI_TYPES_PASS(Args))>             \
    {                                                                   \
        typedef R result_type;                                          \
        typedef mpl::vector<BOOST_DI_TYPES_PASS(Args)> type;            \
    };

    template<typename R, typename T, BOOST_DI_TYPES(Args)>              \
    struct parameter_types<R(T::*)(BOOST_DI_TYPES_PASS(Args))>          \
    {                                                                   \
        typedef R result_type;                                          \
        typedef mpl::vector<BOOST_DI_TYPES_PASS(Args)> type;            \
    };

    template<typename R, typename T, BOOST_DI_TYPES(Args)>              \
    struct parameter_types<R(T::*)(BOOST_DI_TYPES_PASS(Args)) const>    \
    {                                                                   \
        typedef R result_type;                                          \
        typedef mpl::vector<BOOST_DI_TYPES_PASS(Args)> type;            \
    };

#endif

