//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if defined(BOOST_HAS_RVALUE_REFS) && defined(BOOST_HAS_VARIADIC_TMPL)

    #ifndef QDEPS_BACK_SCOPE_SINGLETON_HPP
    #define QDEPS_BACK_SCOPE_SINGLETON_HPP

    #include <memory>
    #include <boost/mpl/placeholders.hpp>
    #include "QDeps/Back/Scope.hpp"

    namespace QDeps
    {
    namespace Back
    {
    namespace Scopes
    {

    class Singleton : public Scope< std::shared_ptr<boost::mpl::_1> >
    {
    public:
        template<typename T, typename... Args>
        std::shared_ptr<T> create(Args&&... p_args)
        {
            if (!m_instance)
            {
                m_instance = std::make_shared(std::forward<Args>(p_args)...);
            }

            return m_instance;
        }

    private:
        std::shared_ptr<void> m_instance;
    };

    } // namespace Scope
    } // namespace Back
    } // namespace QDeps

    #endif

#elif !BOOST_PP_IS_ITERATING

    #ifndef QDEPS_BACK_SCOPE_SINGLETON_HPP
    #define QDEPS_BACK_SCOPE_SINGLETON_HPP

    #include <boost/shared_ptr.hpp>
    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/enum_binary_params.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/mpl/placeholders.hpp>
    #include "QDeps/Back/Scope.hpp"
    #include "QDeps/Config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (0, QDEPS_FUNCTION_ARITY_LIMIT_SIZE, "QDeps/Back/Scopes/Singleton.hpp"))

    namespace QDeps
    {
    namespace Back
    {
    namespace Scopes
    {

    class Singleton : public Scope< boost::shared_ptr<boost::mpl::_1> >
    {
    public:
        #include BOOST_PP_ITERATE()

    private:
        boost::shared_ptr<void> m_instance;
    };

    } // namespace Scope
    } // namespace Back
    } // namespace QDeps

    #endif

#else

    template<typename T BOOST_PP_COMMA_IF(BOOST_PP_ITERATION()) BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    boost::shared_ptr<T> create(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &p_arg))
    {
        if (!m_instance)
        {
            m_instance.reset(new T(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), p_arg)));
        }

        return boost::shared_static_cast<T>(m_instance);
    }

#endif

