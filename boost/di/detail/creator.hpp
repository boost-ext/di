//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_DETAIL_CREATOR_HPP
    #define BOOST_DI_DETAIL_CREATOR_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/push_back.hpp>
    #include "boost/di/aux/dependency.hpp"
    #include "boost/di/aux/ctor.hpp"
    #include "boost/di/detail/binder.hpp"
    #include "boost/di/detail/converter.hpp"
    #include "boost/di/scopes/per_request.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (           \
        BOOST_DI_PARAMS(                            \
            0                                       \
          , BOOST_DI_FUNCTION_ARITY_LIMIT_SIZE      \
          , "boost/di/detail/creator.hpp"           \
        )                                           \
    )

    namespace boost {
    namespace di {
    namespace detail {

    template<
        typename TDeps
      , typename TPool
      , template<
            typename
          , typename
          , typename = TDeps
          , typename = aux::dependency<scopes::per_request, mpl::_1, mpl::_2>
        > class TBinder = binder
      , template<
            typename
          , typename = void
        > class TCtor = aux::ctor
    >
    class creator_impl
    {
        template<typename TDependency>
        struct ctor
            : TCtor<typename TDependency::given>::type
        { };

        template<typename TDependency, int N>
        struct enable_if_ctor_size
            : enable_if_c<
                  mpl::size<typename ctor<TDependency>::type>::value == N
                , typename mpl::at_c<typename ctor<TDependenc>::type, N>::type
              >
        { };

    public:
        template<
            typename T
          , typename TCallStack
          , typename TEntries
        >
        static T execute(TEntries& entries, const TPool& pool) {
            typedef typename TBinder<T, TCallStack>::type to_bo_created_t;
            return execute_impl<
                to_bo_created_t
              , typename mpl::push_back<
                    TCallStack
                  , typename to_bo_created_t::given
                >::type
              , TEntries
            >(entries, pool);
        }

    private:
        #include BOOST_PP_ITERATE()

        template<
          , typename TDependency
          , typename TCallStack
          , int N
          , typename TEntries
        >
        static mpl::at_c<typename ctor<TDependency>::type, N>::type
        execute_impl(TEntries& entries, const TPool& pool) {
            return execute<
                mpl::at_c<typename ctor<TDependenc>::type, N>::type
              , TCallStack
            >(entries, pool)
        }

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

    template<
        typename TDeps
      , typename TPool
    >
    struct creator
        : creator_impl<TDeps, TPool>
    { };

    } // namespace detail
    } // namespace di
    } // namespace boost

    #endif

#else

    template<
        typename TDependency
      , typename TCallStack
      , typename TEntries
    >
    static typename enable_if_ctor_size<TDependency, BOOST_PP_ITERATION()>::type
    execute_impl(TEntries& entries, const TPool& pool) {
        return acquire<TDependency>(entries).create<typename ctor<TDependency>::type>(
            pool
            BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
            BOOST_PP_ENUM_BINARY_PARAMS(
                BOOST_PP_ITERATION()
              , execute_impl<TDependency, TCallStack, TEntries>(entries, pool) BOOST_PP_INTERCEPT
            )
        );
    }

#endif

