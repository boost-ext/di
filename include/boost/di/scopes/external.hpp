//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_SCOPES_EXTERNAL_HPP
    #define BOOST_DI_SCOPES_EXTERNAL_HPP

    #include "boost/di/aux_/config.hpp"
    #include "boost/di/wrappers/value.hpp"
    #include "boost/di/type_traits/create_traits.hpp"
    #include "boost/di/type_traits/has_call_operator.hpp"

    #include <boost/function.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/utility/enable_if.hpp>

    namespace boost {
    namespace di {
    namespace scopes {

    template<template<typename> class TWrapper = wrappers::value>
    class external
    {
    public:
        typedef mpl::int_<1> priority;

        template<typename TExpected, typename = TExpected>
        class scope
        {
        public:
            typedef scope type;
            typedef TWrapper<TExpected> result_type;

        private:
            class result_type_holder
            {
            public:
                template<typename T>
                explicit result_type_holder(const T& object)
                    : object_(object)
                { }

                result_type operator()() const {
                    return object_;
                }

            private:
                result_type object_;
            };

            template<typename TValueType, typename T>
            typename disable_if<type_traits::has_call_operator<TValueType>, function<result_type()> >::type
            convert_when_function(const T& object) {
                return object;
            }

            template<typename TValueType, typename T>
            typename enable_if<type_traits::has_call_operator<TValueType>, result_type_holder>::type
            convert_when_function(const T& object) {
                return result_type_holder(object);
            }

        public:
            template<typename T>
            explicit scope(const T& object
                         , typename enable_if<type_traits::has_call_operator<T> >::type* = 0)
                : object_(convert_when_function<TExpected>(object))
            { }

            template<typename T>
            explicit scope(const T& object
                         , typename disable_if<type_traits::has_call_operator<T> >::type* = 0)
                : object_(result_type_holder(object))
            { }

            result_type create() {
                return object_();
            }

            #define BOOST_PP_FILENAME_1 "boost/di/scopes/external.hpp"
            #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM(1)
            #include BOOST_PP_ITERATE()

        private:
            function<result_type()> object_;
        };
    };

    } // namespace scopes
    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    result_type create(BOOST_DI_ARGS_NOT_USED(Args)) {
        return object_();
    }

#endif

