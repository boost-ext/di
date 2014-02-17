//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_UNIQUE_HPP
#define BOOST_DI_SCOPES_UNIQUE_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/preprocessed/convertibles/copy.hpp"
#include "boost/di/aux_/preprocessed/type_traits/create_traits.hpp"
#include "boost/di/aux_/preprocessed/named.hpp"
#include <boost/mpl/int.hpp>

    namespace boost {
    namespace di {
    namespace scopes {

    template<typename>
    class callback;

        template<
            typename R
           
           
        >
        class callback0
        {
            typedef R(*f_t)();

        public:

            callback0(
                const f_t& f
               
               
            ) : f(f)
               
               
            { }

            R operator()() const { return f(); }

        private:
            f_t f;

           

        };

        template<
            typename R
            ,
            typename Args0
        >
        class callback1
        {
            typedef R(*f_t)( Args0 );

        public:

            callback1(
                const f_t& f
                ,
                Args0 args0
            ) : f(f)
                ,
                args0(args0)
            { }

            R operator()() const { return f( args0); }

        private:
            f_t f;

            Args0 args0;

        };

        template<
            typename R
            ,
            typename Args0 , typename Args1
        >
        class callback2
        {
            typedef R(*f_t)( Args0 , Args1 );

        public:

            callback2(
                const f_t& f
                ,
                Args0 args0 , Args1 args1
            ) : f(f)
                ,
                args0(args0) , args1(args1)
            { }

            R operator()() const { return f( args0 , args1); }

        private:
            f_t f;

            Args0 args0; Args1 args1;

        };

        template<
            typename R
            ,
            typename Args0 , typename Args1 , typename Args2
        >
        class callback3
        {
            typedef R(*f_t)( Args0 , Args1 , Args2 );

        public:

            callback3(
                const f_t& f
                ,
                Args0 args0 , Args1 args1 , Args2 args2
            ) : f(f)
                ,
                args0(args0) , args1(args1) , args2(args2)
            { }

            R operator()() const { return f( args0 , args1 , args2); }

        private:
            f_t f;

            Args0 args0; Args1 args1; Args2 args2;

        };

        template<
            typename R
            ,
            typename Args0 , typename Args1 , typename Args2 , typename Args3
        >
        class callback4
        {
            typedef R(*f_t)( Args0 , Args1 , Args2 , Args3 );

        public:

            callback4(
                const f_t& f
                ,
                Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3
            ) : f(f)
                ,
                args0(args0) , args1(args1) , args2(args2) , args3(args3)
            { }

            R operator()() const { return f( args0 , args1 , args2 , args3); }

        private:
            f_t f;

            Args0 args0; Args1 args1; Args2 args2; Args3 args3;

        };

        template<
            typename R
            ,
            typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4
        >
        class callback5
        {
            typedef R(*f_t)( Args0 , Args1 , Args2 , Args3 , Args4 );

        public:

            callback5(
                const f_t& f
                ,
                Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4
            ) : f(f)
                ,
                args0(args0) , args1(args1) , args2(args2) , args3(args3) , args4(args4)
            { }

            R operator()() const { return f( args0 , args1 , args2 , args3 , args4); }

        private:
            f_t f;

            Args0 args0; Args1 args1; Args2 args2; Args3 args3; Args4 args4;

        };

        template<
            typename R
            ,
            typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5
        >
        class callback6
        {
            typedef R(*f_t)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 );

        public:

            callback6(
                const f_t& f
                ,
                Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5
            ) : f(f)
                ,
                args0(args0) , args1(args1) , args2(args2) , args3(args3) , args4(args4) , args5(args5)
            { }

            R operator()() const { return f( args0 , args1 , args2 , args3 , args4 , args5); }

        private:
            f_t f;

            Args0 args0; Args1 args1; Args2 args2; Args3 args3; Args4 args4; Args5 args5;

        };

        template<
            typename R
            ,
            typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6
        >
        class callback7
        {
            typedef R(*f_t)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 );

        public:

            callback7(
                const f_t& f
                ,
                Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6
            ) : f(f)
                ,
                args0(args0) , args1(args1) , args2(args2) , args3(args3) , args4(args4) , args5(args5) , args6(args6)
            { }

            R operator()() const { return f( args0 , args1 , args2 , args3 , args4 , args5 , args6); }

        private:
            f_t f;

            Args0 args0; Args1 args1; Args2 args2; Args3 args3; Args4 args4; Args5 args5; Args6 args6;

        };

        template<
            typename R
            ,
            typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7
        >
        class callback8
        {
            typedef R(*f_t)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 );

        public:

            callback8(
                const f_t& f
                ,
                Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7
            ) : f(f)
                ,
                args0(args0) , args1(args1) , args2(args2) , args3(args3) , args4(args4) , args5(args5) , args6(args6) , args7(args7)
            { }

            R operator()() const { return f( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7); }

        private:
            f_t f;

            Args0 args0; Args1 args1; Args2 args2; Args3 args3; Args4 args4; Args5 args5; Args6 args6; Args7 args7;

        };

        template<
            typename R
            ,
            typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8
        >
        class callback9
        {
            typedef R(*f_t)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 );

        public:

            callback9(
                const f_t& f
                ,
                Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8
            ) : f(f)
                ,
                args0(args0) , args1(args1) , args2(args2) , args3(args3) , args4(args4) , args5(args5) , args6(args6) , args7(args7) , args8(args8)
            { }

            R operator()() const { return f( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8); }

        private:
            f_t f;

            Args0 args0; Args1 args1; Args2 args2; Args3 args3; Args4 args4; Args5 args5; Args6 args6; Args7 args7; Args8 args8;

        };

        template<
            typename R
            ,
            typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9
        >
        class callback10
        {
            typedef R(*f_t)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 );

        public:

            callback10(
                const f_t& f
                ,
                Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9
            ) : f(f)
                ,
                args0(args0) , args1(args1) , args2(args2) , args3(args3) , args4(args4) , args5(args5) , args6(args6) , args7(args7) , args8(args8) , args9(args9)
            { }

            R operator()() const { return f( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9); }

        private:
            f_t f;

            Args0 args0; Args1 args1; Args2 args2; Args3 args3; Args4 args4; Args5 args5; Args6 args6; Args7 args7; Args8 args8; Args9 args9;

        };

        template<
            typename R
            ,
            typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10
        >
        class callback11
        {
            typedef R(*f_t)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 );

        public:

            callback11(
                const f_t& f
                ,
                Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10
            ) : f(f)
                ,
                args0(args0) , args1(args1) , args2(args2) , args3(args3) , args4(args4) , args5(args5) , args6(args6) , args7(args7) , args8(args8) , args9(args9) , args10(args10)
            { }

            R operator()() const { return f( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10); }

        private:
            f_t f;

            Args0 args0; Args1 args1; Args2 args2; Args3 args3; Args4 args4; Args5 args5; Args6 args6; Args7 args7; Args8 args8; Args9 args9; Args10 args10;

        };

        template<
            typename R
            ,
            typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11
        >
        class callback12
        {
            typedef R(*f_t)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 );

        public:

            callback12(
                const f_t& f
                ,
                Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11
            ) : f(f)
                ,
                args0(args0) , args1(args1) , args2(args2) , args3(args3) , args4(args4) , args5(args5) , args6(args6) , args7(args7) , args8(args8) , args9(args9) , args10(args10) , args11(args11)
            { }

            R operator()() const { return f( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11); }

        private:
            f_t f;

            Args0 args0; Args1 args1; Args2 args2; Args3 args3; Args4 args4; Args5 args5; Args6 args6; Args7 args7; Args8 args8; Args9 args9; Args10 args10; Args11 args11;

        };

        template<
            typename R
            ,
            typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12
        >
        class callback13
        {
            typedef R(*f_t)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 );

        public:

            callback13(
                const f_t& f
                ,
                Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12
            ) : f(f)
                ,
                args0(args0) , args1(args1) , args2(args2) , args3(args3) , args4(args4) , args5(args5) , args6(args6) , args7(args7) , args8(args8) , args9(args9) , args10(args10) , args11(args11) , args12(args12)
            { }

            R operator()() const { return f( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12); }

        private:
            f_t f;

            Args0 args0; Args1 args1; Args2 args2; Args3 args3; Args4 args4; Args5 args5; Args6 args6; Args7 args7; Args8 args8; Args9 args9; Args10 args10; Args11 args11; Args12 args12;

        };

        template<
            typename R
            ,
            typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13
        >
        class callback14
        {
            typedef R(*f_t)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 );

        public:

            callback14(
                const f_t& f
                ,
                Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13
            ) : f(f)
                ,
                args0(args0) , args1(args1) , args2(args2) , args3(args3) , args4(args4) , args5(args5) , args6(args6) , args7(args7) , args8(args8) , args9(args9) , args10(args10) , args11(args11) , args12(args12) , args13(args13)
            { }

            R operator()() const { return f( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13); }

        private:
            f_t f;

            Args0 args0; Args1 args1; Args2 args2; Args3 args3; Args4 args4; Args5 args5; Args6 args6; Args7 args7; Args8 args8; Args9 args9; Args10 args10; Args11 args11; Args12 args12; Args13 args13;

        };

        template<
            typename R
            ,
            typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14
        >
        class callback15
        {
            typedef R(*f_t)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 );

        public:

            callback15(
                const f_t& f
                ,
                Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14
            ) : f(f)
                ,
                args0(args0) , args1(args1) , args2(args2) , args3(args3) , args4(args4) , args5(args5) , args6(args6) , args7(args7) , args8(args8) , args9(args9) , args10(args10) , args11(args11) , args12(args12) , args13(args13) , args14(args14)
            { }

            R operator()() const { return f( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14); }

        private:
            f_t f;

            Args0 args0; Args1 args1; Args2 args2; Args3 args3; Args4 args4; Args5 args5; Args6 args6; Args7 args7; Args8 args8; Args9 args9; Args10 args10; Args11 args11; Args12 args12; Args13 args13; Args14 args14;

        };

        template<
            typename R
            ,
            typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15
        >
        class callback16
        {
            typedef R(*f_t)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 );

        public:

            callback16(
                const f_t& f
                ,
                Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15
            ) : f(f)
                ,
                args0(args0) , args1(args1) , args2(args2) , args3(args3) , args4(args4) , args5(args5) , args6(args6) , args7(args7) , args8(args8) , args9(args9) , args10(args10) , args11(args11) , args12(args12) , args13(args13) , args14(args14) , args15(args15)
            { }

            R operator()() const { return f( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15); }

        private:
            f_t f;

            Args0 args0; Args1 args1; Args2 args2; Args3 args3; Args4 args4; Args5 args5; Args6 args6; Args7 args7; Args8 args8; Args9 args9; Args10 args10; Args11 args11; Args12 args12; Args13 args13; Args14 args14; Args15 args15;

        };

        template<
            typename R
            ,
            typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16
        >
        class callback17
        {
            typedef R(*f_t)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 );

        public:

            callback17(
                const f_t& f
                ,
                Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16
            ) : f(f)
                ,
                args0(args0) , args1(args1) , args2(args2) , args3(args3) , args4(args4) , args5(args5) , args6(args6) , args7(args7) , args8(args8) , args9(args9) , args10(args10) , args11(args11) , args12(args12) , args13(args13) , args14(args14) , args15(args15) , args16(args16)
            { }

            R operator()() const { return f( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16); }

        private:
            f_t f;

            Args0 args0; Args1 args1; Args2 args2; Args3 args3; Args4 args4; Args5 args5; Args6 args6; Args7 args7; Args8 args8; Args9 args9; Args10 args10; Args11 args11; Args12 args12; Args13 args13; Args14 args14; Args15 args15; Args16 args16;

        };

        template<
            typename R
            ,
            typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17
        >
        class callback18
        {
            typedef R(*f_t)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 );

        public:

            callback18(
                const f_t& f
                ,
                Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17
            ) : f(f)
                ,
                args0(args0) , args1(args1) , args2(args2) , args3(args3) , args4(args4) , args5(args5) , args6(args6) , args7(args7) , args8(args8) , args9(args9) , args10(args10) , args11(args11) , args12(args12) , args13(args13) , args14(args14) , args15(args15) , args16(args16) , args17(args17)
            { }

            R operator()() const { return f( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17); }

        private:
            f_t f;

            Args0 args0; Args1 args1; Args2 args2; Args3 args3; Args4 args4; Args5 args5; Args6 args6; Args7 args7; Args8 args8; Args9 args9; Args10 args10; Args11 args11; Args12 args12; Args13 args13; Args14 args14; Args15 args15; Args16 args16; Args17 args17;

        };

        template<
            typename R
            ,
            typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18
        >
        class callback19
        {
            typedef R(*f_t)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18 );

        public:

            callback19(
                const f_t& f
                ,
                Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17 , Args18 args18
            ) : f(f)
                ,
                args0(args0) , args1(args1) , args2(args2) , args3(args3) , args4(args4) , args5(args5) , args6(args6) , args7(args7) , args8(args8) , args9(args9) , args10(args10) , args11(args11) , args12(args12) , args13(args13) , args14(args14) , args15(args15) , args16(args16) , args17(args17) , args18(args18)
            { }

            R operator()() const { return f( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17 , args18); }

        private:
            f_t f;

            Args0 args0; Args1 args1; Args2 args2; Args3 args3; Args4 args4; Args5 args5; Args6 args6; Args7 args7; Args8 args8; Args9 args9; Args10 args10; Args11 args11; Args12 args12; Args13 args13; Args14 args14; Args15 args15; Args16 args16; Args17 args17; Args18 args18;

        };

        template<
            typename R
            ,
            typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18 , typename Args19
        >
        class callback20
        {
            typedef R(*f_t)( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18 , Args19 );

        public:

            callback20(
                const f_t& f
                ,
                Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17 , Args18 args18 , Args19 args19
            ) : f(f)
                ,
                args0(args0) , args1(args1) , args2(args2) , args3(args3) , args4(args4) , args5(args5) , args6(args6) , args7(args7) , args8(args8) , args9(args9) , args10(args10) , args11(args11) , args12(args12) , args13(args13) , args14(args14) , args15(args15) , args16(args16) , args17(args17) , args18(args18) , args19(args19)
            { }

            R operator()() const { return f( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17 , args18 , args19); }

        private:
            f_t f;

            Args0 args0; Args1 args1; Args2 args2; Args3 args3; Args4 args4; Args5 args5; Args6 args6; Args7 args7; Args8 args8; Args9 args9; Args10 args10; Args11 args11; Args12 args12; Args13 args13; Args14 args14; Args15 args15; Args16 args16; Args17 args17; Args18 args18; Args19 args19;

        };

    template<template<typename> class TConvertible = convertibles::copy>
    class unique
    {
    public:
        typedef mpl::int_<0> priority;

        template<typename TExpected, typename TGiven = TExpected>
        class scope
        {
        public:
            typedef TConvertible<TExpected> result_type;

            result_type create() {
                return callback0<TExpected*>(
                    &type_traits::create_traits<TExpected, TGiven>
                );
            }

        template< typename Args0>
        result_type create( Args0 args0) {
            return callback1<TExpected*, Args0>(
                &type_traits::create_traits<TExpected, TGiven, Args0>
              , args0
            );
        }

        template< typename Args0 , typename Args1>
        result_type create( Args0 args0 , Args1 args1) {
            return callback2<TExpected*, Args0 , Args1>(
                &type_traits::create_traits<TExpected, TGiven, Args0 , Args1>
              , args0 , args1
            );
        }

        template< typename Args0 , typename Args1 , typename Args2>
        result_type create( Args0 args0 , Args1 args1 , Args2 args2) {
            return callback3<TExpected*, Args0 , Args1 , Args2>(
                &type_traits::create_traits<TExpected, TGiven, Args0 , Args1 , Args2>
              , args0 , args1 , args2
            );
        }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3>
        result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3) {
            return callback4<TExpected*, Args0 , Args1 , Args2 , Args3>(
                &type_traits::create_traits<TExpected, TGiven, Args0 , Args1 , Args2 , Args3>
              , args0 , args1 , args2 , args3
            );
        }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4>
        result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4) {
            return callback5<TExpected*, Args0 , Args1 , Args2 , Args3 , Args4>(
                &type_traits::create_traits<TExpected, TGiven, Args0 , Args1 , Args2 , Args3 , Args4>
              , args0 , args1 , args2 , args3 , args4
            );
        }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5>
        result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5) {
            return callback6<TExpected*, Args0 , Args1 , Args2 , Args3 , Args4 , Args5>(
                &type_traits::create_traits<TExpected, TGiven, Args0 , Args1 , Args2 , Args3 , Args4 , Args5>
              , args0 , args1 , args2 , args3 , args4 , args5
            );
        }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6>
        result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6) {
            return callback7<TExpected*, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6>(
                &type_traits::create_traits<TExpected, TGiven, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6>
              , args0 , args1 , args2 , args3 , args4 , args5 , args6
            );
        }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7>
        result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7) {
            return callback8<TExpected*, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7>(
                &type_traits::create_traits<TExpected, TGiven, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7>
              , args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7
            );
        }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8>
        result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8) {
            return callback9<TExpected*, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8>(
                &type_traits::create_traits<TExpected, TGiven, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8>
              , args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8
            );
        }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9>
        result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9) {
            return callback10<TExpected*, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9>(
                &type_traits::create_traits<TExpected, TGiven, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9>
              , args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9
            );
        }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10>
        result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10) {
            return callback11<TExpected*, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10>(
                &type_traits::create_traits<TExpected, TGiven, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10>
              , args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10
            );
        }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11>
        result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11) {
            return callback12<TExpected*, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11>(
                &type_traits::create_traits<TExpected, TGiven, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11>
              , args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11
            );
        }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12>
        result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12) {
            return callback13<TExpected*, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12>(
                &type_traits::create_traits<TExpected, TGiven, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12>
              , args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12
            );
        }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13>
        result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13) {
            return callback14<TExpected*, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13>(
                &type_traits::create_traits<TExpected, TGiven, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13>
              , args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13
            );
        }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14>
        result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14) {
            return callback15<TExpected*, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14>(
                &type_traits::create_traits<TExpected, TGiven, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14>
              , args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14
            );
        }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15>
        result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15) {
            return callback16<TExpected*, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15>(
                &type_traits::create_traits<TExpected, TGiven, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15>
              , args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15
            );
        }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16>
        result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16) {
            return callback17<TExpected*, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16>(
                &type_traits::create_traits<TExpected, TGiven, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16>
              , args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16
            );
        }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17>
        result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17) {
            return callback18<TExpected*, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17>(
                &type_traits::create_traits<TExpected, TGiven, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17>
              , args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17
            );
        }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18>
        result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17 , Args18 args18) {
            return callback19<TExpected*, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18>(
                &type_traits::create_traits<TExpected, TGiven, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18>
              , args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17 , args18
            );
        }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18 , typename Args19>
        result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17 , Args18 args18 , Args19 args19) {
            return callback20<TExpected*, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18 , Args19>(
                &type_traits::create_traits<TExpected, TGiven, Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18 , Args19>
              , args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17 , args18 , args19
            );
        }
        };
    };

    }
    }
    }

#endif

