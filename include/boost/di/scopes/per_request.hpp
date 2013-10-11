//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_SCOPES_PER_REQUEST_HPP
    #define BOOST_DI_SCOPES_PER_REQUEST_HPP

    #include "boost/di/detail/meta.hpp"
    #include "boost/di/convertibles/copy.hpp"
    #include "boost/di/type_traits/create_traits.hpp"
    #include "boost/di/named.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 ( \
        BOOST_DI_ITERATION_PARAMS(1, "boost/di/scopes/per_request.hpp"))

    namespace boost {
    namespace di {
    namespace scopes {

    template<template<typename> class TConvertible = convertibles::copy>
    class per_request
    {
    public:
        template<typename TExpected, typename TGiven = TExpected>
        class scope
        {
            template<typename>
            struct callback;

            template<typename R>
            struct callback<R(*)()>
            {
                typedef R(*f_t)();

                callback(const f_t& f)
                    : f(f)
                { }

                R operator()() const {
                    return f();
                }

                f_t f;
            };

        template<typename F>
        callback<F> make_callback(const F& f) {
            return callback<F>(f);
        }

        public:
            typedef TConvertible<TExpected> result_type;

            result_type create() {
                return result_type(
                    make_callback(&type_traits::create_traits<TExpected, TGiven>)
                );
            }

            #include BOOST_PP_ITERATE()
        };
    };

    } // namespace scopes
    } // namespace di
    } // namespace boost

    #endif

#else

    template<typename R, BOOST_DI_TYPES(Args)>
    struct callback<R(*)(BOOST_DI_TYPES_PASS(Args))>
    {
        typedef R(*f_t)(BOOST_DI_TYPES_PASS(Args));

#define BLA(z, n, text) \
    BOOST_PP_COMMA_IF(n) args##n(args##n)

        callback(const f_t& f, BOOST_DI_ARGS(Args, args))
            : f(f), BOOST_PP_REPEAT(BOOST_PP_ITERATION(), BLA, ~)
        { }

#undef BLA

        R operator()() const {
            return f(BOOST_DI_ARGS_PASS(args));
        }

        f_t f;

#define D(z, n, txt) typename boost::remove_reference<Args##n>::type args##n;
BOOST_PP_REPEAT(BOOST_PP_ITERATION(), D, ~)
#undef D
    };

    template<typename F, BOOST_DI_TYPES(Args)>
    callback<F> make_callback(const F& f, BOOST_DI_ARGS(Args, args)) {
        return callback<F>(f, BOOST_DI_ARGS_PASS(args));
    }

    template<BOOST_DI_TYPES(Args)>
    result_type create(BOOST_DI_ARGS(Args, args)) {
        return result_type(make_callback(
            &type_traits::create_traits<TExpected, TGiven, BOOST_DI_TYPES_PASS(Args)>
          , BOOST_DI_ARGS_PASS(args))
        );
    }

#endif

