//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef QDEPS_BACK_AUX_DEPENDENCY_HPP
    #define QDEPS_BACK_AUX_DEPENDENCY_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/enum_binary_params.hpp>
    #include <boost/preprocessor/facilities/intercept.hpp>
    #include <boost/preprocessor/cat.hpp>
    #include <boost/typeof/typeof.hpp>
    #include <boost/function_types/parameter_types.hpp>
    #include <boost/type_traits/is_same.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/and.hpp>
    #include <boost/mpl/not.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/contains.hpp>
    #include <boost/mpl/placeholders.hpp>
    #include "QDeps/Back/Aux/Utility.hpp"
    #include "QDeps/Back/Aux/Value.hpp"
    #include "QDeps/Config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, QDEPS_FUNCTION_ARITY_LIMIT_SIZE, "QDeps/Back/Aux/Dependency.hpp"))

    namespace QDeps
    {
    namespace Back
    {
    namespace Aux
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
        template<typename TPool> struct IsPoolType
            : boost::mpl::contains<typename TPool::Seq, TExpected>
        { };

        template<typename TPool> struct IsValueType
            : boost::mpl::and_< TValue<TGiven>, boost::mpl::not_<boost::mpl::contains<typename TPool::Seq, TExpected> > >
        { };

        template<typename TPool> struct IsScopeType
            : boost::mpl::and_< boost::mpl::not_<TValue<TGiven> >, boost::mpl::not_<boost::mpl::contains<typename TPool::Seq, TExpected> > >
        { };

        template<bool, typename = void> struct CtorImpl
            : boost::mpl::vector0<>
        { };

        template<typename Dummy>
        struct CtorImpl<true, Dummy>
            : boost::function_types::parameter_types<BOOST_TYPEOF_TPL(TGiven::QDEPS_CTOR_UNIQUE_NAME::ctor)>::type
        { };

    public:
        typedef TScope Scope;
        typedef TExpected Expected;
        typedef TGiven Given;
        typedef TContext Context;
        typedef TBind Bind;

        struct Ctor
            : CtorImpl<BOOST_PP_CAT(Detail::has_, QDEPS_CTOR_UNIQUE_NAME)<Given>::value>::type
        { };

        template<typename, typename = void> struct ResultType;

        template<typename TPool>
        struct ResultType<TPool, typename boost::enable_if< IsPoolType<TPool> >::type>
            : TPool::template ResultType<TExpected>
        { };

        template<typename TPool>
        struct ResultType<TPool, typename boost::enable_if< IsValueType<TPool> >::type>
            : TValue<TGiven>::template ResultType<TExpected>
        { };

        template<typename TPool>
        struct ResultType<TPool, typename boost::enable_if< IsScopeType<TPool> >::type>
            : TScope::template ResultType<TExpected>
        { };

        template<typename TPool>
        typename boost::enable_if<IsPoolType<TPool>, typename ResultType<TPool>::type>::type create(TPool& p_pool)
        {
            return p_pool.template get<TExpected>();
        }

        template<typename TPool>
        typename boost::enable_if<IsValueType<TPool>, typename ResultType<TPool>::type>::type create(TPool&)
        {
            return TValue<TGiven>::template create<TExpected>();
        }

        template<typename TPool>
        typename boost::enable_if<IsScopeType<TPool>, typename ResultType<TPool>::type>::type create(TPool&)
        {
            return m_scope.template create<TGiven>();
        }

        #include BOOST_PP_ITERATE()

    private:
        TScope m_scope;
    };

    template
    <
        typename TExpected,
        typename TGiven,
        typename TContext,
        typename TBind,
        template<typename> class TValue
    >
    class Dependency<boost::mpl::_1, TExpected, TGiven, TContext, TBind, TValue>
    {
    public:
        template<typename Scope> struct Rebind
        {
            typedef Dependency<Scope, TExpected, TGiven, TContext, TBind, TValue> type;
        };
    };

    template
    <
        typename TScope,
        typename TContext,
        typename TBind,
        template<typename> class TValue
    >
    class Dependency<TScope, boost::mpl::_1, boost::mpl::_1, TContext, TBind, TValue>
    {
    public:
        template<typename Expected> struct Rebind
        {
            typedef Dependency<TScope, Expected, Expected, TContext, TBind, TValue> type;
        };
    };

    } // namespace Aux
    } // namespace Back
    } // namespace QDeps

    #endif

#else
    template<typename TPool, BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    typename boost::enable_if<IsScopeType<TPool>, typename ResultType<TPool>::type>::type create(TPool&, BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &p_arg))
    {
        return m_scope.template create<TGiven>(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), p_arg));
    }

    template<typename TPool, BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    typename boost::enable_if<IsPoolType<TPool>, typename ResultType<TPool>::type>::type create(TPool& p_pool, BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, & BOOST_PP_INTERCEPT))
    {
        return p_pool.template get<TExpected>();
    }

#endif

