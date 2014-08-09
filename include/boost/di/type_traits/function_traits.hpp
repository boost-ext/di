//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_AUX_DETAIL_FUNCTION_TRAITS_HPP
    #define BOOST_DI_AUX_DETAIL_FUNCTION_TRAITS_HPP

    #include "boost/di/aux_/config.hpp"

    namespace boost {
    namespace di {
    namespace type_traits {

    template<typename>
    struct function_traits;

    template<typename R>
    struct function_traits<R(*)()>
    {
        using result_type = R;
        using type = type_list<>;
    };

    template<typename R, typename T>
    struct function_traits<R(T::*)()>
    {
        using result_type = R;
        using base_type = T;
        using type = type_list<>;
    };

    template<typename R, typename T>
    struct function_traits<R(T::*)() const>
    {
        using result_type = R;
        using base_type = T;
        using type = type_list<>;
    };

    #define BOOST_PP_FILENAME_1 "boost/di/type_traits/function_traits.hpp"
    #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM_FORCE(1)
    #include BOOST_PP_ITERATE()

    } // namespace type_traits
    } // namespace di
    } // namespace boost

    #endif

#else

    template<typename R, BOOST_DI_TYPES(Args)>                              \
    struct function_traits<R(*)(BOOST_DI_TYPES_PASS(Args))>                 \
    {                                                                       \
        using result_type = R;                                              \
        using type = type_list<BOOST_DI_TYPES_PASS(Args)>;                  \
    };

    template<typename R, typename T, BOOST_DI_TYPES(Args)>                  \
    struct function_traits<R(T::*)(BOOST_DI_TYPES_PASS(Args))>              \
    {                                                                       \
        using result_type = R;                                              \
        using base_type = T;                                                \
        using type = type_list<BOOST_DI_TYPES_PASS(Args)>;                  \
    };

    template<typename R, typename T, BOOST_DI_TYPES(Args)>                  \
    struct function_traits<R(T::*)(BOOST_DI_TYPES_PASS(Args)) const>        \
    {                                                                       \
        using result_type = R;                                              \
        using base_type = T;                                                \
        using type = type_list<BOOST_DI_TYPES_PASS(Args)>;                  \
    };

#endif

