//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_SCOPES_SESSION_HPP
    #define BOOST_DI_SCOPES_SESSION_HPP

    #include "boost/di/aux_/meta.hpp"
    #include "boost/di/type_traits/create_traits.hpp"
    #include "boost/di/convertibles/shared.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 ( \
        BOOST_DI_ITERATION_PARAMS(1, "boost/di/scopes/session.hpp"))

    namespace boost {
    namespace di {
    namespace scopes {

    class session_entry { };
    class session_exit { };

    template<template<typename> class TConvertible = convertibles::shared>
    class session
    {
    public:
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
                object().reset();
            }

            result_type create() {
                if (in_scope_ && !object()) {
                    object().reset(type_traits::create_traits<TExpected, TGiven>());
                }
                return object();
            }

            #include BOOST_PP_ITERATE()

        private:
            static result_type& object() {
                static result_type object;
                return object;
            }

            bool in_scope_;
        };
    };

    } // namespace scopes
    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    result_type create(BOOST_DI_ARGS(Args, args)) {
        if (in_scope_ && !object()) {
            object().reset(
                type_traits::create_traits<TExpected, TGiven>(BOOST_DI_ARGS_PASS(args))
            );
        }
        return object();
    }

#endif

