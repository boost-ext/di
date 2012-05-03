//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_SCOPES_PER_REQUEST_HPP
    #define BOOST_DI_SCOPES_PER_REQUEST_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/shared_ptr.hpp>
    #include <boost/make_shared.hpp>
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (       \
        BOOST_DI_ITERATION_PARAMS(              \
            1                                   \
          , BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE  \
          , "boost/di/scopes/per_request.hpp"   \
        )                                       \
    )

    namespace boost {
    namespace di {
    namespace scopes {

    class per_request
    {
    public:
        template<typename T>
        class scope
        {
        public:
            shared_ptr<T> create() {
                return make_shared<T>();
            }

            #include BOOST_PP_ITERATE()
        };
    };

    } // namespace scopes
    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_ARGS_TYPES(Args)>
    shared_ptr<T> create(BOOST_DI_ARGS(Args, args)) {
        return shared_ptr<T>(new T(BOOST_DI_ARGS_FORWARD(args)));
    }

#endif

