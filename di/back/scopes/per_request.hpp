//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef DI_BACK_SCOPE_PERREQUEST_HPP
    #define DI_BACK_SCOPE_PERREQUEST_HPP

    #include <boost/shared_ptr.hpp>
    #include <boost/make_shared.hpp>
    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/enum_binary_params.hpp>
    #include "di/Config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, DI_FUNCTION_ARITY_LIMIT_SIZE, "di/Back/Scopes/PerRequest.hpp"))

    namespace di
    {
    namespace Back
    {
    namespace Scopes
    {

    class PerRequest
    {
    public:
        template<typename T> class Scope
        {
        public:
            typedef boost::shared_ptr<T> ResultType;

            ResultType create()
            {
                return boost::make_shared<T>();
            }

            #include BOOST_PP_ITERATE()
        };
    };

    } // namespace Scope
    } // namespace Back
    } // namespace di

    #endif

#else

    template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    ResultType create(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &p_arg))
    {
        return ResultType(new T(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), p_arg)));
    }

#endif

