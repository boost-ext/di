//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_SCOPES_THREAD_HPP
    #define BOOST_DI_SCOPES_THREAD_HPP

    #include "boost/di/aux_/meta.hpp"
    #include "boost/di/aux_/memory.hpp"
    #include "boost/di/aux_/thread.hpp"
    #include "boost/di/convertibles/shared.hpp"
    #include "boost/di/type_traits/create_traits.hpp"

    #include <map>
    #include <algorithm>

    namespace boost {
    namespace di {
    namespace scopes {

    template<template<typename> class TConvertible = convertibles::shared>
    class per_thread
    {
    public:
        template<typename TExpected, typename TGiven = TExpected>
        class scope
        {
        public:
            typedef TConvertible<TExpected> result_type;

            result_type create() {
                thread::id id_ = this_thread::get_id();

                if (std::find(objects_.begin(), objects_.end(), id_) != objects_.end()) {
                    objects_[id_].reset(type_traits::create_traits<TExpected, TGiven>());
                }
                return objects_[id_];
            }

            #define BOOST_PP_FILENAME_1 "boost/di/scopes/per_thread.hpp"
            #define BOOST_PP_ITERATION_LIMITS BOOST_DI_LIMITS_BEGIN(1)
            #include BOOST_PP_ITERATE()

        private:
            std::map<thread::id, result_type> objects_;
        };
    };

    } // namespace scopes
    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    result_type create(BOOST_DI_ARGS(Args, args)) {
        thread::id id_ = this_thread::get_id();

        if (std::find(objects_.begin(), objects_.end(), id_) != objects_.end()) {
            objects_[id_].reset(
                type_traits::create_traits<TExpected, TGiven>(BOOST_DI_ARGS_PASS(args))
            );
        }
        return objects_[id_];
    }

#endif


