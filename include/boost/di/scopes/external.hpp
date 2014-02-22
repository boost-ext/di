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
    #include "boost/di/convertibles/value.hpp"
    #include "boost/di/type_traits/create_traits.hpp"

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

            template<typename TPolicy>
            result_type create(const TPolicy&) {
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

    template<typename TPolicy, BOOST_DI_TYPES(Args)>
    result_type create(const TPolicy&, BOOST_DI_ARGS_NOT_USED(Args)) {
        return object_();
    }

#endif

