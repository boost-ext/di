//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_SCOPES_SINGLETON_HPP
    #define BOOST_DI_SCOPES_SINGLETON_HPP

    #include <boost/shared_ptr.hpp>
    #include <boost/preprocessor/iteration/iterate.hpp>
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (\
        BOOST_DI_PARAMS(1, BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE, "boost/di/scopes/singleton.hpp"))

    namespace boost {
    namespace di {
    namespace scopes {

    class singleton
    {
    public:
        template<typename T>
        class scope
        {
        public:
            typedef shared_ptr<T> result_type;

            result_type create() {
                if (!instance_) {
                    instance_.reset(new T);
                }

                return instance_;
            }

            #include BOOST_PP_ITERATE()

        private:
            result_type instance_;
        };
    };

    } // namespace scopes
    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    result_type create(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &arg)) {
        if (!instance_) {
            instance_.reset(new T(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), arg)));
        }

        return instance_;
    }

#endif

