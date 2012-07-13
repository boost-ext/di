//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_DETAIL_DEPENDENCY_HPP
    #define BOOST_DI_DETAIL_DEPENDENCY_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/type_traits/is_same.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/and.hpp>
    #include <boost/mpl/not.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/contains.hpp>
    #include <boost/mpl/placeholders.hpp>
    #include <boost/mpl/assert.hpp>

    #include "boost/di/type_traits/has_traits.hpp"
    #include "boost/di/scopes/external.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (       \
        BOOST_DI_ITERATION_PARAMS(              \
            1                                   \
          , BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE  \
          , "boost/di/detail/dependency.hpp"    \
        )                                       \
    )

    namespace boost {
    namespace di {
    namespace detail {

    template<
        typename TScope
      , typename TExpected
      , typename TGiven = TExpected
      , typename TContext = mpl::vector0<>
      , typename TBind = is_same<mpl::_1, TExpected>
      , template<
            typename
          , typename
          , typename = void
        > class TExternal = scopes::convertible_any
    >
    class dependency
    {
        BOOST_MPL_ASSERT_MSG(
            !type_traits::has_element_type<TGiven>::value
          , GIVEN_TYPE_WITH_ELEMENT_TYPE
          , (TGiven)
        );

        typedef typename TScope::template scope<TExpected, TGiven> scope_type;
        typedef TExternal<TExpected, TContext> external_type;

    public:
        typedef dependency type;
        typedef TScope scope;
        typedef TExpected expected;
        typedef TGiven given;
        typedef TContext context;
        typedef TBind bind;

    private:
        template<typename TExternals>
        struct is_scope_type
            : mpl::not_<
                  mpl::contains<
                      typename TExternals::types
                    , external_type
                  >
              >
        { };

        template<typename TExternals>
        struct is_external_type
            : mpl::contains<
                  typename TExternals::types
                , external_type
              >
        { };

    public:
        template<typename TExternals, typename = void>
        struct result_type
        {
            typedef external_type type;
        };

        template<typename TExternals>
        struct result_type<
            TExternals
          , typename enable_if<is_scope_type<TExternals> >::type
        >
        {
            typedef typename scope_type::result_type type;
        };

        template<typename TExternals>
        typename enable_if<
            is_scope_type<TExternals>
          , typename result_type<TExternals>::type
        >::type
        create(const TExternals&) {
            return scope_.create();
        }

        template<typename TExternals>
        typename enable_if<
            is_external_type<TExternals>
          , typename result_type<TExternals>::type
        >::type
        create(const TExternals& externals) {
            return externals.template get<typename result_type<TExternals>::type>();
        }

        template<typename TAction>
        void call(const TAction& action) {
            scope_.call(action);
        }

        //workaround
        template<typename>
        struct rebind
        {
            typedef dependency<
                TScope
              , TExpected
              , TGiven
              , TContext
              , TBind
              , TExternal
            > type;
        };

        #include BOOST_PP_ITERATE()

    private:
        scope_type scope_;
    };

    template<
        typename TExpected
      , typename TGiven
      , typename TContext
      , typename TBind
      , template<
            typename
          , typename
          , typename
        > class TExternal
    >
    class dependency<
        mpl::_1
      , TExpected
      , TGiven
      , TContext
      , TBind
      , TExternal
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
              , TExternal
            > type;
        };
    };

    template<
        typename TScope
      , typename TBind
      , template<
            typename
          , typename
          , typename
        > class TExternal
    >
    class dependency<
        TScope
      , mpl::_1
      , mpl::_2
      , mpl::_3
      , TBind
      , TExternal
    >
    {
    public:
        template<
            typename TExpected
          , typename TGiven
          , typename TContext
        >
        struct rebind
        {
            typedef dependency<
                TScope
              , TExpected
              , TGiven
              , TContext
              , TBind
              , TExternal
            > type;
        };
    };

    } // namespace detail
    } // namespace di
    } // namespace boost

    #endif

#else

    template<typename TExternals, BOOST_DI_TYPES(Args)>
    typename enable_if<
        is_scope_type<TExternals>
      , typename result_type<TExternals>::type
    >::type
    create(const TExternals&, BOOST_DI_ARGS(Args, args)) {
        return scope_.create(BOOST_DI_ARGS_FORWARD(args));
    }

    template<typename TExternals, BOOST_DI_TYPES(Args)>
    typename enable_if<
        is_external_type<TExternals>
      , typename result_type<TExternals>::type
    >::type
    create(const TExternals& externals, BOOST_DI_ARGS_NOT_USED(Args)) {
        return externals.template get<typename result_type<TExternals>::type>();
    }

#endif

