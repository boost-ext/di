//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef DI_BACK_SCOPE_SINGLETON_HPP
    #define DI_BACK_SCOPE_SINGLETON_HPP

    #include <boost/shared_ptr.hpp>
    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/enum_binary_params.hpp>
    #include "di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, DI_FUNCTION_ARITY_LIMIT_SIZE, "di/back/scopes/singleton.hpp"))

    namespace di
    {
    namespace back
    {
    namespace scopes
    {

    class singleton
    {
    public:
        template<typename T> class scope
        {
        public:
            typedef boost::shared_ptr<T> result_type;

            result_type create()
            {
                if (!m_instance)
                {
                    m_instance.reset(new T);
                }

                return m_instance;
            }

            #include BOOST_PP_ITERATE()

        private:
            result_type m_instance;
        };
    };

    } // namespace scope
    } // namespace back
    } // namespace di

    #endif

#else

    template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    result_type create(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &arg))
    {
        if (!m_instance)
        {
            m_instance.reset(new T(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), arg)));
        }

        return m_instance;
    }

#endif

