//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_SCOPES_SESSION_HPP
    #define BOOST_DI_SCOPES_SESSION_HPP

    #include "boost/di/aux_/config.hpp"
    #include "boost/di/aux_/memory.hpp"
    #include "boost/di/type_traits/create_traits.hpp"
    #include "boost/di/convertibles/shared.hpp"

    #include <cassert>
    #include <boost/mpl/int.hpp>

    namespace boost {
    namespace di {
    namespace scopes {

    class session_entry { };
    class session_exit { };

    template<template<typename> class TConvertible = convertibles::shared>
    class session
    {
    public:
        typedef mpl::int_<0> priority;

        template<typename TExpected, typename TGiven = TExpected>
        class scope
        {
        public:
            typedef TConvertible<TExpected> result_type;

            scope()
                : in_scope_(false)
            { }

            void call(const session_entry&) {
                in_scope_ = true;
            }

            void call(const session_exit&) {
                in_scope_ = false;
                object_.reset();
            }

            template<typename TPolicy>
            result_type create(const TPolicy&) {
                if (in_scope_ && !object_) {
                    object_.reset(type_traits::create_traits<TPolicy, TExpected, TGiven>());
                }
                return object_;
            }

            #define BOOST_PP_FILENAME_1 "boost/di/scopes/session.hpp"
            #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM(1)
            #include BOOST_PP_ITERATE()

        private:
            result_type object_;
            bool in_scope_;
        };
    };

    } // namespace scopes
    } // namespace di
    } // namespace boost

    #endif

#else

    template<typename TPolicy, BOOST_DI_TYPES(Args)>
    result_type create(const TPolicy&, BOOST_DI_ARGS(Args, args)) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TPolicy, TExpected, TGiven>(BOOST_DI_ARGS_PASS(args))
            );
        }
        return object_;
    }

#endif

