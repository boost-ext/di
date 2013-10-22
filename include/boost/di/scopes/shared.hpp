//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_SCOPES_SHARED_HPP
    #define BOOST_DI_SCOPES_SHARED_HPP

    #include "boost/di/aux_/meta.hpp"
    #include "boost/di/convertibles/shared.hpp"
    #include "boost/di/type_traits/create_traits.hpp"

    #include <boost/mpl/int.hpp>

    namespace boost {
    namespace di {
    namespace scopes {

    template<template<typename> class TConvertible = convertibles::shared>
    class shared
    {
    public:
        typedef mpl::int_<0> priority;

        template<typename TExpected, typename TGiven = TExpected>
        class scope
        {
        public:
            typedef TConvertible<TExpected> result_type;

            result_type create() {
                if (!object_) {
                    object_.reset(type_traits::create_traits<TExpected, TGiven>());
                }
                return object_;
            }

            #define BOOST_PP_FILENAME_1 "boost/di/scopes/shared.hpp"
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
    result_type create(BOOST_DI_ARGS(Args, args)) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>(BOOST_DI_ARGS_PASS(args))
            );
        }
        return object_;
    }

#endif

