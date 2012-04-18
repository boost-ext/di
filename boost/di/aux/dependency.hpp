//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_AUX_DEPENDENCY_HPP
    #define BOOST_DI_AUX_DEPENDENCY_HPP

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
    #include "boost/di/aux/instance.hpp"
    #include "boost/di/aux/explicit_value.hpp"
    #include "boost/di/ctor.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE, "boost/di/aux/dependency.hpp"))

    namespace boost {
    namespace di {
    namespace aux {

    template
    <
        typename TScope,
        typename TExpected,
        typename TGiven = TExpected,
        typename TContext = boost::mpl::vector0<>,
        typename TBind = boost::is_same<boost::mpl::_1, TExpected>,
        template<typename, typename = void> class TValue = explicit_value,
        template<typename = TExpected, typename = TContext, typename = void> class TInstance = instance
    >
    class dependency
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(BOOST_DI_CTOR_UNIQUE_NAME)
        BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)

        BOOST_DI_STATIC_ASSERT(
            !has_element_type<TGiven>::value,
            GIVEN_TYPE_WITH_ELEMENT_TYPE,
            (TGiven)
        );

        template<typename TPool> struct is_pool_type
            : boost::mpl::contains<typename TPool::seq, TInstance<> >
        { };

        template<typename TPool> struct is_value_type
            : boost::mpl::and_< TValue<TGiven>, boost::mpl::not_<boost::mpl::contains<typename TPool::seq, TInstance<> > > >
        { };

        template<typename TPool> struct is_scope_type
            : boost::mpl::and_< boost::mpl::not_<TValue<TGiven> >, boost::mpl::not_<boost::mpl::contains<typename TPool::seq, TInstance<> > > >
        { };

        template<bool, typename = void> struct ctor_impl
            : boost::function_types::parameter_types<BOOST_TYPEOF_TPL(ctor_traits<TGiven>::ctor)>::type
        { };

        template<typename Dummy>
        struct ctor_impl<true, Dummy>
            : boost::function_types::parameter_types<BOOST_TYPEOF_TPL(TGiven::BOOST_DI_CTOR_UNIQUE_NAME::ctor)>::type
        { };

    public:
        typedef TScope scope;
        typedef TExpected expected;
        typedef TGiven given;
        typedef TContext context;
        typedef TBind bind;

        struct ctor
            : ctor_impl<BOOST_PP_CAT(has_, BOOST_DI_CTOR_UNIQUE_NAME)<given>::value>::type
        { };

        template<typename, typename = void> struct result_type;

        template<typename TPool>
        struct result_type<TPool, typename boost::enable_if< is_pool_type<TPool> >::type>
            : TPool::template result_type<TInstance<> >
        { };

        template<typename TPool>
        struct result_type<TPool, typename boost::enable_if< is_value_type<TPool> >::type>
            : boost::mpl::identity<typename TValue<TGiven>::result_type>
        { };

        template<typename TPool>
        struct result_type<TPool, typename boost::enable_if< is_scope_type<TPool> >::type>
            : boost::mpl::identity<typename TScope::template scope<TGiven>::result_type>
        { };

        template<typename TPool>
        typename boost::enable_if<is_pool_type<TPool>, typename result_type<TPool>::type>::type create(const TPool& pool)
        {
            return pool.template get<TInstance<> >();
        }

        template<typename TPool>
        typename boost::enable_if<is_value_type<TPool>, typename result_type<TPool>::type>::type create(const TPool&)
        {
            return TValue<TGiven>::create();
        }

        template<typename TPool>
        typename boost::enable_if<is_scope_type<TPool>, typename result_type<TPool>::type>::type create(const TPool&)
        {
            return scope_.create();
        }

        #include BOOST_PP_ITERATE()

    private:
        typename TScope::template scope<TGiven> scope_;
    };

    template
    <
        typename TExpected,
        typename TGiven,
        typename TContext,
        typename TBind,
        template<typename, typename> class TValue,
        template<typename, typename, typename> class TInstance
    >
    class dependency<boost::mpl::_1, TExpected, TGiven, TContext, TBind, TValue, TInstance>
    {
    public:
        template<typename scope> struct rebind
        {
            typedef dependency<scope, TExpected, TGiven, TContext, TBind, TValue, TInstance> type;
        };
    };

    template
    <
        typename TScope,
        typename TContext,
        typename TBind,
        template<typename, typename> class TValue,
        template<typename, typename, typename> class TInstance
    >
    class dependency<TScope, boost::mpl::_1, boost::mpl::_2, TContext, TBind, TValue, TInstance>
    {
    public:
        template<typename expected, typename given> struct rebind
        {
            typedef dependency<TScope, expected, given, TContext, TBind, TValue, TInstance> type;
        };
    };

    } // namespace aux
    } // namespace di
    } // namespace boost

    #endif

#else

    template<typename TPool, BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    typename boost::enable_if<is_scope_type<TPool>, typename result_type<TPool>::type>::type
    create(const TPool&, BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &arg))
    {
        return scope_.create(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), arg));
    }

    template<typename TPool, BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    typename boost::enable_if<is_pool_type<TPool>, typename result_type<TPool>::type>::type
    create(const TPool& pool, BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, & BOOST_PP_INTERCEPT))
    {
        return pool.template get<TInstance<> >();
    }

#endif

