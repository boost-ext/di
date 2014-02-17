//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_EXTERNAL_HPP
#define BOOST_DI_SCOPES_EXTERNAL_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/preprocessed/convertibles/value.hpp"
#include <boost/function.hpp>
#include <boost/non_type.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/aux_/yes_no.hpp>

    namespace boost {
    namespace di {

    template<typename T>
    class has_call_operator
    {
        struct base_impl { void operator()(...) { } };
        struct base
            : base_impl
            , mpl::if_<is_class<T>, T, mpl::void_>::type
        { base() { } };

        template<typename U>
        static mpl::aux::no_tag test(
            U*
          , non_type<void (base_impl::*)(...), &U::operator()>* = 0
        );

        static mpl::aux::yes_tag test(...);

    public:
        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test((base*)(0))) == sizeof(mpl::aux::yes_tag)
        );
    };

    namespace scopes {

    template<template<typename> class TConvertible = convertibles::value>
    class external
    {
    public:
        typedef mpl::int_<1> priority;

        template<typename TExpected, typename = TExpected>
        class scope
        {
        public:
            typedef TConvertible<TExpected> result_type;

        private:
            class callback
            {
            public:
                template<typename T>
                explicit callback(const T& object)
                    : object_(object)
                { }

                result_type operator()() const {
                    return object_;
                }

            private:
                result_type object_;
            };

        public:
            template<typename T>
            explicit scope(const T& object, typename enable_if_c<has_call_operator<T>::value>::type* = 0)
                : object_(object)
            { }

            template<typename T>
            explicit scope(const T& object, typename disable_if_c<has_call_operator<T>::value>::type* = 0)
                : object_(callback(object))
            { }

            result_type create() {
                return object_();
            }

    template< typename Args0>
    result_type create( Args0 ) {
        return object_();
    }

    template< typename Args0 , typename Args1>
    result_type create( Args0 , Args1 ) {
        return object_();
    }

    template< typename Args0 , typename Args1 , typename Args2>
    result_type create( Args0 , Args1 , Args2 ) {
        return object_();
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3>
    result_type create( Args0 , Args1 , Args2 , Args3 ) {
        return object_();
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4>
    result_type create( Args0 , Args1 , Args2 , Args3 , Args4 ) {
        return object_();
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5>
    result_type create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 ) {
        return object_();
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6>
    result_type create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 ) {
        return object_();
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7>
    result_type create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 ) {
        return object_();
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8>
    result_type create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 ) {
        return object_();
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9>
    result_type create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 ) {
        return object_();
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10>
    result_type create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 ) {
        return object_();
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11>
    result_type create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 ) {
        return object_();
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12>
    result_type create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 ) {
        return object_();
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13>
    result_type create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 ) {
        return object_();
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14>
    result_type create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 ) {
        return object_();
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15>
    result_type create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 ) {
        return object_();
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16>
    result_type create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 ) {
        return object_();
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17>
    result_type create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 ) {
        return object_();
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18>
    result_type create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18 ) {
        return object_();
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18 , typename Args19>
    result_type create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18 , Args19 ) {
        return object_();
    }

        private:
            function<result_type()> object_;
        };
    };

    }
    }
    }

#endif

