//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef QDEPS_BACK_AUX_DEPENDENCY_HPP
    #define QDEPS_BACK_AUX_DEPENDENCY_HPP

    #include <boost/shared_ptr.hpp>
    #include <boost/type_traits/is_same.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/contains.hpp>
    #include <boost/mpl/placeholders.hpp>
    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/enum_binary_params.hpp>
    #include <boost/preprocessor/facilities/intercept.hpp>
    #include "QDeps/Back/Value.hpp"
    #include "QDeps/Config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, QDEPS_FUNC_ARITY, "QDeps/Back/Dependency.hpp"))

    namespace QDeps
    {
    namespace Back
    {

    template
    <
        typename TScope,
        typename TExpected,
        typename TGiven = TExpected,
        typename TContext = boost::mpl::vector0<>,
        typename TBind = boost::is_same<boost::mpl::_1, TExpected>,
        template<typename> class TValue = Value
    >
    class Dependency
    {
        template<bool> class HasValue { };

    public:
        typedef TContext Context;
        typedef TBind Bind;

        template<typename TPool> boost::shared_ptr<TExpected> create
        (
            TPool& p_pool,
            typename boost::enable_if< boost::mpl::contains<typename TPool::Seq, TExpected> >::type* = 0
        )
        {
            return p_pool.template get<TExpected>();
        }

        template<typename TPool> boost::shared_ptr<TExpected> create
        (
            TPool&,
            typename boost::disable_if< boost::mpl::contains<typename TPool::Seq, TExpected> >::type* = 0
        )
        {
            return createImpl(HasValue<TValue<TGiven>::value>());
        }

        #include BOOST_PP_ITERATE()

    private:
        boost::shared_ptr<TExpected> createImpl(const HasValue<true>&)
        {
            return TValue<TGiven>::template create<TExpected>();
        }

        boost::shared_ptr<TExpected> createImpl(const HasValue<false>&)
        {
            return m_scope.template create<TGiven>();
        }

        TScope m_scope;
    };

    } // namespace Back
    } // namespace QDeps

    #endif

#else
    template<typename TPool, BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    boost::shared_ptr<TExpected> create
    (
        TPool&,
        BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &p_arg),
        typename boost::disable_if< boost::mpl::contains<typename TPool::Seq, TExpected> >::type* = 0
    )
    {
        return m_scope.template create<TGiven>(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), p_arg));
    }

    template<typename TPool, BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    boost::shared_ptr<TExpected> create
    (
        TPool& p_pool,
        BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, & BOOST_PP_INTERCEPT),
        typename boost::enable_if< boost::mpl::contains<typename TPool::Seq, TExpected> >::type* = 0
    )
    {
        return p_pool.template get<TExpected>();
    }

#endif

