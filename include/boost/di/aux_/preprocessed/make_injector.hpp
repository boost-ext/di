//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_MAKE_INJECTOR_HPP
#define BOOST_DI_MAKE_INJECTOR_HPP

#include "boost/di/aux_/preprocessed/make_injector.hpp"
#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/preprocessed/injector.hpp"

    namespace boost {
    namespace di {

    inline injector<> make_injector() {
        return injector<>()();
    }

    template< typename Args0>
    injector<typename detail::concepts<mpl::vector< Args0> >::type>
    inline make_injector( Args0 args0) {
        return injector<typename detail::concepts<mpl::vector< Args0> >::type>(
            args0
        );
    }

    template< typename Args0 , typename Args1>
    injector<typename detail::concepts<mpl::vector< Args0 , Args1> >::type>
    inline make_injector( Args0 args0 , Args1 args1) {
        return injector<typename detail::concepts<mpl::vector< Args0 , Args1> >::type>(
            args0 , args1
        );
    }

    template< typename Args0 , typename Args1 , typename Args2>
    injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2> >::type>
    inline make_injector( Args0 args0 , Args1 args1 , Args2 args2) {
        return injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2> >::type>(
            args0 , args1 , args2
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3>
    injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3> >::type>
    inline make_injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3) {
        return injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3> >::type>(
            args0 , args1 , args2 , args3
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4>
    injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4> >::type>
    inline make_injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4) {
        return injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4> >::type>(
            args0 , args1 , args2 , args3 , args4
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5>
    injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5> >::type>
    inline make_injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5) {
        return injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5> >::type>(
            args0 , args1 , args2 , args3 , args4 , args5
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6>
    injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6> >::type>
    inline make_injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6) {
        return injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6> >::type>(
            args0 , args1 , args2 , args3 , args4 , args5 , args6
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7>
    injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7> >::type>
    inline make_injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7) {
        return injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7> >::type>(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8>
    injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8> >::type>
    inline make_injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8) {
        return injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8> >::type>(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9>
    injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9> >::type>
    inline make_injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9) {
        return injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9> >::type>(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10>
    injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10> >::type>
    inline make_injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10) {
        return injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10> >::type>(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11>
    injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11> >::type>
    inline make_injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11) {
        return injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11> >::type>(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12>
    injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12> >::type>
    inline make_injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12) {
        return injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12> >::type>(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13>
    injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13> >::type>
    inline make_injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13) {
        return injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13> >::type>(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14>
    injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14> >::type>
    inline make_injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14) {
        return injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14> >::type>(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15>
    injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15> >::type>
    inline make_injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15) {
        return injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15> >::type>(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16>
    injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16> >::type>
    inline make_injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16) {
        return injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16> >::type>(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17>
    injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17> >::type>
    inline make_injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17) {
        return injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17> >::type>(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18>
    injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18> >::type>
    inline make_injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17 , Args18 args18) {
        return injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18> >::type>(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17 , args18
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18 , typename Args19>
    injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18 , Args19> >::type>
    inline make_injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17 , Args18 args18 , Args19 args19) {
        return injector<typename detail::concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18 , Args19> >::type>(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17 , args18 , args19
        );
    }

    }
    }

#endif

