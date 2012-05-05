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
    #include <boost/type_traits/is_same.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/and.hpp>
    #include <boost/mpl/not.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/contains.hpp>
    #include <boost/mpl/placeholders.hpp>
    #include <boost/mpl/assert.hpp>
    #include <boost/mpl/has_xxx.hpp>
    #include "boost/di/aux/instance.hpp"
    #include "boost/di/aux/explicit_value.hpp"
    #include "boost/di/aux/converter.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (       \
        BOOST_DI_ITERATION_PARAMS(              \
            1                                   \
          , BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE  \
          , "boost/di/aux/dependency.hpp"       \
        )                                       \
    )

    namespace boost {
    namespace di {
    namespace aux {

    template<
        typename TScope
      , typename TExpected
      , typename TGiven = TExpected
      , typename TContext = mpl::vector0<>
      , typename TBind = is_same<mpl::_1, TExpected>
      , template<
            typename
          , typename = void
        > class TExplicitValue = explicit_value
      , template<
            typename = TExpected
          , typename = TContext
          , typename = void
        > class TInstance = instance
      , template<
            typename
          , typename
        > class TConverter = converter
    >
    class dependency
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)

        BOOST_MPL_ASSERT_MSG(
            !has_element_type<TGiven>::value
          , GIVEN_TYPE_WITH_ELEMENT_TYPE
          , (TGiven)
        );

        template<typename TPool>
        struct is_value_type
            : mpl::and_<
                  TExplicitValue<TGiven>
                , mpl::not_<
                      mpl::contains<typename TPool::sequence, TInstance<> >
                  >
              >
        { };

        template<typename TPool>
        struct is_scope_type
            : mpl::and_<
                  mpl::not_<TExplicitValue<TGiven> >
                , mpl::not_<mpl::contains<typename TPool::sequence, TInstance<> > >
              >
        { };

        template<typename TPool>
        struct is_pool_type
            : mpl::contains<typename TPool::sequence, TInstance<> >
        { };

    public:
        typedef dependency type;
        typedef TScope scope;
        typedef TExpected expected;
        typedef TGiven given;
        typedef TContext context;
        typedef TBind bind;

        template<typename T, typename TPool>
        typename enable_if<is_value_type<TPool>, T>::type
        create(const TPool&) {
            return TConverter<scope, T>::execute(TExplicitValue<TGiven>::create());
        }

        template<typename T, typename TPool>
        typename enable_if<is_scope_type<TPool>, T>::type
        create(const TPool&) {
            return TConverter<scope, T>::execute(scope_.create());
        }

        template<typename T, typename TPool>
        typename enable_if<is_pool_type<TPool>, T>::type
        create(const TPool& pool) {
            return TConverter<scope, T>::execute(pool.template get<TInstance<> >());
        }

        template<typename TAction>
        void call(const TAction& action) {
            scope_.call(action);
        }

        #include BOOST_PP_ITERATE()

    private:
        typename TScope::template scope<TGiven> scope_;
    };

    template<
        typename TExpected
      , typename TGiven
      , typename TContext
      , typename TBind
      , template<
            typename
          , typename
        > class TExplicitValue
      , template<
            typename
          , typename
          , typename
        > class TInstance
      , template<
            typename
          , typename
        > class TConverter
    >
    class dependency<
        mpl::_1
      , TExpected
      , TGiven
      , TContext
      , TBind
      , TExplicitValue
      , TInstance
      , TConverter
    >
    {
    public:
        template<typename TScope>
        struct rebind
        {
            typedef dependency<
                TScope
              , TExpected
              , TGiven
              , TContext
              , TBind
              , TExplicitValue
              , TInstance
              , TConverter
            > type;
        };
    };

    template<
        typename TScope
      , typename TContext
      , typename TBind
      , template<
            typename
          , typename
        > class TExplicitValue
      , template<
            typename
          , typename
          , typename
        > class TInstance
      , template<
            typename
          , typename
        > class TConverter
    >
    class dependency<
        TScope
      , mpl::_1
      , mpl::_2
      , TContext
      , TBind
      , TExplicitValue
      , TInstance
      , TConverter
    >
    {
    public:
        template<typename TExpected, typename TGiven>
        struct rebind
        {
            typedef dependency<
                TScope
              , TExpected
              , TGiven
              , TContext
              , TBind
              , TExplicitValue
              , TInstance
              , TConverter
            > type;
        };
    };

    } // namespace aux
    } // namespace di
    } // namespace boost

    #endif

#else

    template<typename T, typename TPool, BOOST_DI_TYPES(Args)>
    typename enable_if<is_scope_type<TPool>, T>::type
    create(const TPool&, BOOST_DI_ARGS(Args, args)) {
        return TConverter<scope, T>::execute(scope_.create(BOOST_DI_ARGS_PASS(args)));
    }

    template<typename T, typename TPool, BOOST_DI_TYPES(Args)>
    typename enable_if<is_pool_type<TPool> >::type
    create(const TPool& pool, BOOST_DI_ARGS_NOT_USED(Args)) {
        return TConverter<scope, T>::execute(pool.template get<TInstance<> >());
    }

#endif

