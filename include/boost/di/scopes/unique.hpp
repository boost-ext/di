//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_SCOPES_UNIQUE_HPP
    #define BOOST_DI_SCOPES_UNIQUE_HPP

    #include "boost/di/aux_/config.hpp"
    #include "boost/di/wrappers/copy.hpp"
    #include "boost/di/type_traits/create_traits.hpp"

    #include <boost/preprocessor/cat.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/mpl/int.hpp>

    namespace boost {
    namespace di {
    namespace scopes {

    #define BOOST_DI_CALLBACK
    #define BOOST_PP_FILENAME_1 "boost/di/scopes/unique.hpp"
    #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM(0)
    #include BOOST_PP_ITERATE()
    #undef BOOST_DI_CALLBACK

    template<template<typename> class TWrapper = wrappers::copy>
    class unique
    {
    public:
        typedef mpl::int_<0> priority;

        template<typename TExpected, typename TGiven = TExpected>
        class scope
        {
        public:
            typedef scope type;
            typedef TWrapper<TExpected> result_type;

            result_type create() {
                return callback0<TExpected*>(
                    &type_traits::create_traits<TExpected, TGiven>
                );
            }

            #define BOOST_PP_FILENAME_1 "boost/di/scopes/unique.hpp"
            #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM(1)
            #include BOOST_PP_ITERATE()
        };
    };

    } // namespace scopes
    } // namespace di
    } // namespace boost

    #endif

#else

    #define n BOOST_PP_ITERATION()

    #if !defined(BOOST_DI_CALLBACK)

        template<BOOST_DI_TYPES(Args)>
        result_type create(BOOST_DI_ARGS(Args, args)) {
            return BOOST_PP_CAT(callback, n)<TExpected*, BOOST_DI_TYPES_PASS(Args)>(
                &type_traits::create_traits<TExpected, TGiven, BOOST_DI_TYPES_PASS(Args)>
              , BOOST_DI_ARGS_PASS(args)
            );
        }

    #else

        template<
            typename R
            BOOST_PP_COMMA_IF(n)
            BOOST_DI_TYPES(Args)
        >
        class BOOST_PP_CAT(callback, n)
        {
            typedef R(*f_t)(BOOST_DI_ARGS_NOT_USED(Args));

        public:
            #define BOOST_DI_CALLBACK_ARGS_PASS(z, n, na) \
                BOOST_PP_COMMA_IF(n) args##n(args##n)

            BOOST_PP_CAT(callback, n)(
                const f_t& f
                BOOST_PP_COMMA_IF(n)
                BOOST_DI_ARGS(Args, args)
            ) : f(f)
                BOOST_PP_COMMA_IF(n)
                BOOST_PP_REPEAT(n, BOOST_DI_CALLBACK_ARGS_PASS, ~)
            { }

            #undef BOOST_DI_CALLBACK_ARGS_PASS

            R operator()() const { return f(BOOST_DI_ARGS_PASS(args)); }

        private:
            f_t f;

            #define BOOST_DI_CALLBACK_MEMBER_DECL(z, n, na) Args##n args##n;
            BOOST_PP_REPEAT(n, BOOST_DI_CALLBACK_MEMBER_DECL, ~)
            #undef BOOST_DI_CALLBACK_MEMBER_DECL
        };

    #endif

    #undef n

#endif

