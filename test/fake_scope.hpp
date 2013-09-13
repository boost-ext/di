//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_FAKE_SCOPE_HPP
    #define BOOST_DI_FAKE_SCOPE_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/shared_ptr.hpp>
    #include <boost/make_shared.hpp>

    #include "boost/di/scopes/external.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (   \
        BOOST_DI_ITERATION_PARAMS(          \
            1                               \
          , BOOST_DI_LIMIT_SIZE             \
          , "fake_scope.hpp"                \
        )                                   \
    )

    namespace boost {
    namespace di {

    template<bool = false>
    struct priority_impl
    { };

    template<>
    struct priority_impl<true>
    {
        typedef void is_priority;
    };

    template<bool Priority = false>
    struct fake_scope : priority_impl<Priority>
    {
        struct entry { };
        struct exit { };

        template<typename T>
        class convertible
        {
        public:
            explicit convertible(shared_ptr<T> obj)
                : obj_(obj)
            { }

            operator shared_ptr<T>() const {
                return obj_;
            }

            operator T() const {
                return *obj_;
            }

        private:
            shared_ptr<T> obj_;
        };

        template<typename T, typename>
        struct scope
        {
            typedef convertible<T> result_type;

            void call(const entry&) {
                entry_calls()++;
            }

            void call(const exit&) {
                exit_calls()++;
            }

            result_type create() {
                if (entry_calls() > exit_calls()) {
                    return result_type(make_shared<T>());
                }

                return result_type(shared_ptr<T>());
            }

            #include BOOST_PP_ITERATE()
        };

        static int& entry_calls() {
            static int calls;
            return calls;
        }

        static int& exit_calls() {
            static int calls;
            return calls;
        }
    };

    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    result_type create(BOOST_DI_ARGS(Args, args)) {
        if (entry_calls() > exit_calls()) {
            return result_type(shared_ptr<T>(new T(BOOST_DI_ARGS_FORWARD(args))));
        }

        return result_type(shared_ptr<T>());
    }

#endif

