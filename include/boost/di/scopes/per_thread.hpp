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
    #include "boost/di/aux_/thread.hpp"

    #include <map>
    #include <boost/function.hpp>

    namespace boost {
    namespace di {
    namespace scopes {

    //struct session_entry
    //{
        //session_entry(thread::id id) // non explicit
            //: id(id)
        //{ }

         //thread::id id;
    //};

    template<typename TScope>
    class per_thread
    {
    public:
        template<typename TExpected, typename TGiven = TExpected>
        class scope
        {
            typedef typename TScope::template scope<TExpected, TGiven> scope_type;
            typedef function<thread::id()> get_id_t;

        public:
            typedef typename scope_type::result_type result_type;

            explicit scope(const get_id_t& get_id = &this_thread::get_id)
                : get_id_(get_id)
            { }

            result_type create() {
                return objects_[get_id_()].create();
            }

            #define BOOST_PP_FILENAME_1 "boost/di/scopes/per_thread.hpp"
            #define BOOST_PP_ITERATION_LIMITS BOOST_DI_LIMITS_BEGIN(1)
            #include BOOST_PP_ITERATE()

        private:
            get_id_t get_id_;
            std::map<thread::id, scope_type> objects_;
        };
    };

    } // namespace scopes
    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    result_type create(BOOST_DI_ARGS(Args, args)) {
        return objects_[get_id_()].create(BOOST_DI_ARGS_PASS(args));
    }

#endif


