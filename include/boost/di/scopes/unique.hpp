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
    #include "boost/di/aux_/function.hpp"
    #include "boost/di/wrappers/copy.hpp"
    #include "boost/di/type_traits/create_traits.hpp"
    #include "boost/di/named.hpp"

    #include <boost/preprocessor/cat.hpp>
    #include <boost/mpl/int.hpp>

    namespace boost {
    namespace di {
    namespace scopes {

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

            template<typename TCreatePolicy>
            result_type create() {
                return aux::bind<TExpected*>(
                    &type_traits::create_traits<TCreatePolicy, TExpected, TGiven>
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

    template<typename TCreatePolicy, BOOST_DI_TYPES(Args)>
    result_type create(BOOST_DI_ARGS(Args, args)) {
        return aux::bind<TExpected*>(
            &type_traits::create_traits<TCreatePolicy, TExpected, TGiven, BOOST_DI_TYPES_PASS(Args)>
          , BOOST_DI_ARGS_PASS(args)
        );
    }

#endif

