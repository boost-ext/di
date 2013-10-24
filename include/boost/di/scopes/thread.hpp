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
    #include <boost/mpl/int.hpp>

    namespace boost {
    namespace di {
    namespace scopes {

    template<typename T>
    struct thread_entry
    {
        thread_entry(aux::thread::id id) // non explicit
            : id(id)
        { }

        aux::thread::id id;
    };

    template<typename T>
    struct thread_exit
    {
        thread_exit(aux::thread::id id) // non explicit
            : id(id)
        { }

        aux::thread::id id;
    };

    template<typename TScope>
    class thread
    {
    public:
        typedef thread type;
        typedef mpl::int_<1> priority;

        template<typename TExpected, typename TGiven = TExpected>
        class scope
        {
            typedef typename TScope::template scope<TExpected, TGiven> scope_type;

        public:
            typedef typename scope_type::result_type result_type;

            result_type create() {
                return object_.create();
            }

            #define BOOST_PP_FILENAME_1 "boost/di/scopes/thread.hpp"
            #define BOOST_PP_ITERATION_LIMITS BOOST_DI_LIMITS_BEGIN(1)
            #include BOOST_PP_ITERATE()

        private:
            aux::thread_specific_ptr<scope_type> object_;
        };

        template<typename T>
        struct rebind
        {
            typedef thread<T> other;
        };
    };

    } // namespace scopes
    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    result_type create(BOOST_DI_ARGS(Args, args)) {
        return object_.create(BOOST_DI_ARGS_PASS(args));
    }

#endif


