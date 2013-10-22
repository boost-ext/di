//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_SCOPES_EXTERNAL_HPP
    #define BOOST_DI_SCOPES_EXTERNAL_HPP

    #include "boost/di/aux_/meta.hpp"
    #include "boost/di/convertibles/value.hpp"

    namespace boost {
    namespace di {
    namespace scopes {

    template<template<typename> class TConvertible = convertibles::value>
    class external
    {
    public:
        typedef mpl::int_<2> priority;

        template<typename TExpected, typename = TExpected>
        class scope
        {
        public:
            typedef TConvertible<TExpected> result_type;

            template<typename T>
            explicit scope(const T& object)
                : object_(object)
            { }

            result_type create() {
                return object_;
            }

            #define BOOST_PP_FILENAME_1 "boost/di/scopes/external.hpp"
            #define BOOST_PP_ITERATION_LIMITS BOOST_DI_LIMITS_BEGIN(1)
            #include BOOST_PP_ITERATE()

        private:
            result_type object_;
        };
    };

    } // namespace scopes
    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    result_type create(BOOST_DI_ARGS_NOT_USED(Args)) {
        return object_;
    }

#endif

