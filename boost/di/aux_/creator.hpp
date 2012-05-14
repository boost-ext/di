//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_AUX_CREATOR_HPP
    #define BOOST_DI_AUX_CREATOR_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/push_back.hpp>
    #include "boost/di/aux_/dependency.hpp"
    #include "boost/di/aux_/ctor_traits.hpp"
    #include "boost/di/aux_/converter.hpp"
    #include "boost/di/aux_/binder.hpp"
    #include "boost/di/scopes/per_request.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (       \
        BOOST_DI_ITERATION_PARAMS(              \
            0                                   \
          , BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE  \
          , "boost/di/aux_/creator.hpp"         \
        )                                       \
    )

    namespace boost {
    namespace di {
    namespace aux_ {

    template<
        typename TDeps
      , template<
            typename
          , typename
          , typename
          , typename = dependency<scopes::per_request, mpl::_1, mpl::_2>
        > class TBinder = binder
      , template<
            typename
          , typename = void
        > class TCtorTraits = ctor_traits
    >
    class creator_impl
    {
        template<typename TDependency>
        struct ctor
            : TCtorTraits<typename TDependency::given>::type
        { };

    public:
        template<
            typename T
          , typename TCallStack
          , typename TEntries
          , typename TPool
        >
        static T execute(TEntries& entries, const TPool& pool) {
            typedef typename TBinder<T, TCallStack, TDeps>::type to_bo_created_t;
            return execute_impl<
                T
              , to_bo_created_t
              , typename mpl::push_back<
                    TCallStack
                  , typename to_bo_created_t::given
                >::type
            >(entries, pool);
        }

    private:
        #include BOOST_PP_ITERATE()

        template<
            typename TDependency
          , typename TEntries
        >
        static typename enable_if<is_base_of<TDependency, TEntries>, TDependency&>::type
        acquire(TEntries& entries) {
            return static_cast<TDependency&>(entries);
        }

        template<
            typename TDependency
          , typename TEntries
        >
        static typename disable_if<is_base_of<TDependency, TEntries>, TDependency>::type
        acquire(TEntries&) {
            return TDependency();
        }
    };

    template<typename TDeps>
    struct creator
        : creator_impl<TDeps>
    { };

    } // namespace aux_
    } // namespace di
    } // namespace boost

    #endif

#else

    template<
        typename T
      , typename TDependency
      , typename TCallStack
      , typename TEntries
      , typename TPool
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value
        ==
        BOOST_PP_ITERATION()
      , T
    >::type execute_impl(TEntries& entries, const TPool& pool) {

        #define BOOST_DI_CREATOR_EXECUTE(z, n, _)                               \
            BOOST_PP_COMMA_IF(n)                                                \
            execute<                                                            \
                typename mpl::at_c<typename ctor<TDependency>::type, n>::type   \
              , TCallStack                                                      \
            >(entries, pool)

        return acquire<TDependency>(entries).template create<T>(
            pool
            BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
            BOOST_PP_REPEAT(
                BOOST_PP_ITERATION()
              , BOOST_DI_CREATOR_EXECUTE
              , ~
            )
        );

        #undef BOOST_DI_CREATOR_EXECUTE
    }

#endif

