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
    #include "boost/di/aux_/memory.hpp"

    #include <map>
    #include <boost/config.hpp>
    #include <boost/function.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/int.hpp>
    #include <boost/non_type.hpp>
    #include <boost/mpl/aux_/yes_no.hpp>

    namespace boost {
    namespace di {
    namespace scopes {

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

            template<typename T, typename TAction>
            class has_call
            {
                template<typename U>
                static mpl::aux::yes_tag test(non_type<void(T::*)(const TAction&), &U::call>*);

                template<typename>
                static mpl::aux::no_tag test(...);

            public:
                BOOST_STATIC_CONSTANT(
                    bool
                  , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
                );
            };

        public:
            typedef typename scope_type::result_type result_type;

            scope()
                : object_(new aux::thread_specific_ptr<scope_type>())
            { }

            template<typename TAction>
            typename enable_if<has_call<scope_type, TAction> >::type
            call(const TAction& action) {
                (*object_)->call(action);
            }

            template<typename TAction>
            typename disable_if<has_call<scope_type, TAction> >::type
            call(const TAction&) { }

            result_type create() {
                return object().create();
            }

            #define BOOST_PP_FILENAME_1 "boost/di/scopes/thread.hpp"
            #define BOOST_PP_ITERATION_LIMITS BOOST_DI_LIMITS_BEGIN(1)
            #include BOOST_PP_ITERATE()

        private:
            scope_type& object() {
                if (!object_->get()) {
                    object_->reset(new scope_type());
                }
                return **object_;
            }

            aux::shared_ptr<aux::thread_specific_ptr<scope_type> > object_;
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
        return object().create(BOOST_DI_ARGS_PASS(args));
    }

#endif


