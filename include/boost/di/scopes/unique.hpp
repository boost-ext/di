//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_SCOPES_UNIQUE_HPP
    #define BOOST_DI_SCOPES_UNIQUE_HPP

    #include "boost/di/aux_/meta.hpp"
    #include "boost/di/convertibles/copy.hpp"
    #include "boost/di/type_traits/create_traits.hpp"
    #include "boost/di/type_traits/remove_accessors.hpp"
    #include "boost/di/named.hpp"

    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>

    namespace boost {
    namespace di {
    namespace scopes {

    template<typename>
    class callback;

    #define BOOST_DI_CALLBACK
    #define BOOST_PP_FILENAME_1 "boost/di/scopes/unique.hpp"
    #define BOOST_PP_ITERATION_LIMITS BOOST_DI_LIMITS_BEGIN(0)
    #include BOOST_PP_ITERATE()
    #undef BOOST_DI_CALLBACK

    template<template<typename> class TConvertible = convertibles::copy>
    class unique
    {
    public:
        template<typename TExpected, typename TGiven = TExpected>
        class scope
        {
        public:
            typedef TConvertible<TExpected> result_type;

            result_type create() {
                return result_type(make_callback(
                    &type_traits::create_traits<TExpected, TGiven>)
                );
            }

            #define BOOST_PP_FILENAME_1 "boost/di/scopes/unique.hpp"
            #define BOOST_PP_ITERATION_LIMITS BOOST_DI_LIMITS_BEGIN(1)
            #include BOOST_PP_ITERATE()
        };
    };

    } // namespace scopes
    } // namespace di
    } // namespace boost

    #endif

#else

    #if !defined(BOOST_DI_CALLBACK)

        template<BOOST_DI_TYPES(Args)>
        result_type create(BOOST_DI_ARGS(Args, args)) {
            return result_type(make_callback(
                &type_traits::create_traits<TExpected, TGiven, BOOST_DI_TYPES_PASS(Args)>
              , BOOST_DI_ARGS_PASS(args))
            );
        }

    #else

        template<
            typename R
            BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
            BOOST_DI_TYPES(Args)
        >

    #if defined(_MSC_VER)
        class callback<R(BOOST_DI_TYPES_PASS(Args))>
    #else
        class callback<R(*)(BOOST_DI_TYPES_PASS(Args))>
    #endif
        {
            typedef R(*f_t)(BOOST_DI_TYPES_PASS(Args));

            #define BOOST_DI_CALLBACK_TYPE_DECL(z, n, na) \
                typedef typename type_traits::remove_accessors<Args##n>::type Args_t##n;
            BOOST_PP_REPEAT(BOOST_PP_ITERATION(), BOOST_DI_CALLBACK_TYPE_DECL, ~)
            #undef BOOST_DI_CALLBACK_TYPE_DECL

        public:
            #define BOOST_DI_CALLBACK_ARGS_PASS(z, n, na) \
                BOOST_PP_COMMA_IF(n) args##n(args##n)

            callback(const f_t& f
                     BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                     BOOST_DI_ARGS(Args_t, args))
                : f(f)
                  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                  BOOST_PP_REPEAT(BOOST_PP_ITERATION(), BOOST_DI_CALLBACK_ARGS_PASS, ~)
            { }

            #undef BOOST_DI_CALLBACK_ARGS_PASS

            R operator()() const { return f(BOOST_DI_ARGS_PASS(args)); }

        private:
            f_t f;

            #define BOOST_DI_CALLBACK_MEMBER_DECL(z, n, na) Args_t##n args##n;
            BOOST_PP_REPEAT(BOOST_PP_ITERATION(), BOOST_DI_CALLBACK_MEMBER_DECL, ~)
            #undef BOOST_DI_CALLBACK_MEMBER_DECL
        };

        template<
            typename F
            BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
            BOOST_DI_TYPES(Args)
        >
        callback<F> make_callback(const F& f
                                  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                                  BOOST_DI_ARGS(Args, args)) {
            return callback<F>(
                f
                BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                BOOST_DI_ARGS_PASS(args)
            );
        }

    #endif

#endif

