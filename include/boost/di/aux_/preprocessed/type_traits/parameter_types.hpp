//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_PARAMETER_TYPES_HPP
#define BOOST_DI_TYPE_TRAITS_PARAMETER_TYPES_HPP

#include "boost/di/aux_/preprocessed/type_traits/parameter_types.hpp"
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

    template<typename R, typename Args0>
    struct parameter_types<R(*)( Args0)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0> type;
    };

    template<typename R, typename T, typename Args0>
    struct parameter_types<R(T::*)( Args0)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0> type;
    };

    template<typename R, typename T, typename Args0>
    struct parameter_types<R(T::*)( Args0) const>
    {
        typedef R result_type;
        typedef mpl::vector< Args0> type;
    };

    template<typename R, typename Args0 , typename Args1>
    struct parameter_types<R(*)( Args0 , Args1)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1>
    struct parameter_types<R(T::*)( Args0 , Args1)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1>
    struct parameter_types<R(T::*)( Args0 , Args1) const>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1> type;
    };

    template<typename R, typename Args0 , typename Args1 , typename Args2>
    struct parameter_types<R(*)( Args0 , Args1 , Args2)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2) const>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2> type;
    };

    template<typename R, typename Args0 , typename Args1 , typename Args2 , typename Args3>
    struct parameter_types<R(*)( Args0 , Args1 , Args2 , Args3)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3) const>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3> type;
    };

    template<typename R, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4>
    struct parameter_types<R(*)( Args0 , Args1 , Args2 , Args3 , Args4)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4) const>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4> type;
    };

    template<typename R, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5>
    struct parameter_types<R(*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5) const>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5> type;
    };

    template<typename R, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6>
    struct parameter_types<R(*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6) const>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6> type;
    };

    template<typename R, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7>
    struct parameter_types<R(*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7) const>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7> type;
    };

    template<typename R, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8>
    struct parameter_types<R(*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8) const>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8> type;
    };

    template<typename R, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9>
    struct parameter_types<R(*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9) const>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9> type;
    };

    template<typename R, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10>
    struct parameter_types<R(*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10) const>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10> type;
    };

    template<typename R, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11>
    struct parameter_types<R(*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11) const>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11> type;
    };

    template<typename R, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12>
    struct parameter_types<R(*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12) const>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12> type;
    };

    template<typename R, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13>
    struct parameter_types<R(*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13) const>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13> type;
    };

    template<typename R, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14>
    struct parameter_types<R(*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14) const>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14> type;
    };

    template<typename R, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15>
    struct parameter_types<R(*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15) const>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15> type;
    };

    template<typename R, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16>
    struct parameter_types<R(*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16) const>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16> type;
    };

    template<typename R, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17>
    struct parameter_types<R(*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17) const>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17> type;
    };

    template<typename R, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18>
    struct parameter_types<R(*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18) const>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18> type;
    };

    template<typename R, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18 , typename Args19>
    struct parameter_types<R(*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18 , Args19)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18 , Args19> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18 , typename Args19>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18 , Args19)>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18 , Args19> type;
    };

    template<typename R, typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18 , typename Args19>
    struct parameter_types<R(T::*)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18 , Args19) const>
    {
        typedef R result_type;
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18 , Args19> type;
    };

    }
    }
    }

#endif

