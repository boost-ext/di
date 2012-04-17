//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef DI_BACK_AUX_DEPENDENCY_HPP
    #define DI_BACK_AUX_DEPENDENCY_HPP

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
    #include <boost/mpl/identity.hpp>
    #include <boost/mpl/has_xxx.hpp>
    #include "di/back/aux/instance.hpp"
    #include "di/back/aux/result.hpp"
    #include "di/front/ctor.hpp"
    #include "di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, DI_FUNCTION_ARITY_LIMIT_SIZE, "di/back/aux/dependency.hpp"))

    namespace di
    {
    namespace back
    {
    namespace aux
    {

    template
    <
        typename TScope,
        typename TExpected,
        typename TGiven = TExpected,
        typename TContext = boost::mpl::vector0<>,
        typename TBind = boost::is_same<boost::mpl::_1, TExpected>,
        template<typename, typename = void> class TResult = result,
        template<typename = TExpected, typename = TContext, typename = void> class TInstance = Instance
    >
    class Dependency
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(DI_CTOR_UNIQUE_NAME)
        BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)

        DI_STATIC_ASSERT(
            !has_element_type<TGiven>::value,
            GIVEN_TYPE_WITH_ELEMENT_TYPE,
            (TGiven)
        );

        template<typename TPool> struct IsPoolType
            : boost::mpl::contains<typename TPool::Seq, TInstance<> >
        { };

        template<typename TPool> struct IsvalueType
            : boost::mpl::and_< TResult<TGiven>, boost::mpl::not_<boost::mpl::contains<typename TPool::Seq, TInstance<> > > >
        { };

        template<typename TPool> struct IsScopeType
            : boost::mpl::and_< boost::mpl::not_<TResult<TGiven> >, boost::mpl::not_<boost::mpl::contains<typename TPool::Seq, TInstance<> > > >
        { };

        template<bool, typename = void> struct ctor_impl
            : boost::function_types::parameter_types<BOOST_TYPEOF_TPL(ctorTraits<TGiven>::ctor)>::type
        { };

        template<typename Dummy>
        struct ctor_impl<true, Dummy>
            : boost::function_types::parameter_types<BOOST_TYPEOF_TPL(TGiven::DI_CTOR_UNIQUE_NAME::ctor)>::type
        { };

    public:
        typedef TScope Scope;
        typedef TExpected Expected;
        typedef TGiven Given;
        typedef TContext Context;
        typedef TBind Bind;

        struct ctor
            : ctor_impl<BOOST_PP_CAT(has_, DI_CTOR_UNIQUE_NAME)<Given>::value>::type
        { };

        template<typename, typename = void> struct result_type;

        template<typename TPool>
        struct result_type<TPool, typename boost::enable_if< IsPoolType<TPool> >::type>
            : TPool::template result_type<TInstance<> >
        { };

        template<typename TPool>
        struct result_type<TPool, typename boost::enable_if< IsvalueType<TPool> >::type>
            : boost::mpl::identity<typename TResult<TGiven>::result_type>
        { };

        template<typename TPool>
        struct result_type<TPool, typename boost::enable_if< IsScopeType<TPool> >::type>
            : boost::mpl::identity<typename TScope::template Scope<TGiven>::result_type>
        { };

        template<typename TPool>
        typename boost::enable_if<IsPoolType<TPool>, typename result_type<TPool>::type>::type create(const TPool& p_pool)
        {
            return p_pool.template get<TInstance<> >();
        }

        template<typename TPool>
        typename boost::enable_if<IsvalueType<TPool>, typename result_type<TPool>::type>::type create(const TPool&)
        {
            return TResult<TGiven>::create();
        }

        template<typename TPool>
        typename boost::enable_if<IsScopeType<TPool>, typename result_type<TPool>::type>::type create(const TPool&)
        {
            return m_scope.create();
        }

        #include BOOST_PP_ITERATE()

    private:
        typename TScope::template Scope<TGiven> m_scope;
    };

    template
    <
        typename TExpected,
        typename TGiven,
        typename TContext,
        typename TBind,
        template<typename, typename> class TResult,
        template<typename, typename, typename> class TInstance
    >
    class Dependency<boost::mpl::_1, TExpected, TGiven, TContext, TBind, TResult, TInstance>
    {
    public:
        template<typename Scope> struct Rebind
        {
            typedef Dependency<Scope, TExpected, TGiven, TContext, TBind, TResult, TInstance> type;
        };
    };

    template
    <
        typename TScope,
        typename TContext,
        typename TBind,
        template<typename, typename> class TResult,
        template<typename, typename, typename> class TInstance
    >
    class Dependency<TScope, boost::mpl::_1, boost::mpl::_2, TContext, TBind, TResult, TInstance>
    {
    public:
        template<typename Expected, typename Given> struct Rebind
        {
            typedef Dependency<TScope, Expected, Given, TContext, TBind, TResult, TInstance> type;
        };
    };

    } // namespace aux
    } // namespace back
    } // namespace di

    #endif

#else

    template<typename TPool, BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    typename boost::enable_if<IsScopeType<TPool>, typename result_type<TPool>::type>::type create(const TPool&, BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &p_arg))
    {
        return m_scope.create(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), p_arg));
    }

    template<typename TPool, BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    typename boost::enable_if<IsPoolType<TPool>, typename result_type<TPool>::type>::type create(const TPool& p_pool, BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, & BOOST_PP_INTERCEPT))
    {
        return p_pool.template get<TInstance<> >();
    }

#endif

