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

    #include "boost/di/aux_/meta.hpp"
    #include "boost/di/aux_/ctor_traits.hpp"

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
        typename TBinder
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

        template<
            typename T
          , typename TCallStack
        >
        struct binder
            : TBinder::template impl<T, TCallStack>::type
        { };

    public:
        template<
            typename T
          , typename TCallStack
          , typename TEntries
          , typename TPool
        >
        static typename binder<T, TCallStack>::result_type
        execute(TEntries& entries, const TPool& pool) {
            return execute_impl<
                T
              , typename mpl::push_back<
                    TCallStack
                  , typename binder<T, TCallStack>::given
                >::type
              , binder<T, TCallStack>
            >(entries, pool);
        }

    private:
        #include BOOST_PP_ITERATE()

        template<
            typename TDependency
          , typename TEntries
        >
        static typename enable_if<
            is_base_of<TDependency, TEntries>
          , TDependency&
        >::type
        acquire(TEntries& entries) {
            return static_cast<TDependency&>(entries);
        }

        template<
            typename TDependency
          , typename TEntries
        >
        static typename disable_if<
            is_base_of<TDependency, TEntries>
          , TDependency
        >::type
        acquire(TEntries&) {
            return TDependency();
        }
    };

    template<typename TBinder>
    struct creator
        : creator_impl<TBinder>
    { };

    } // namespace aux_
    } // namespace di
    } // namespace boost

    #endif

#else

    template<
        typename T
      , typename TCallStack
      , typename TDependency
      , typename TEntries
      , typename TPool
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value
        ==
        BOOST_PP_ITERATION()
      , typename TDependency::result_type
    >::type execute_impl(TEntries& entries, const TPool& pool) {

        #define BOOST_DI_CREATOR_EXECUTE(z, n, _)       \
            BOOST_PP_COMMA_IF(n)                        \
            execute<                                    \
                typename mpl::at_c<                     \
                    typename ctor<TDependency>::type    \
                  , n                                   \
                >::type                                 \
              , TCallStack                              \
            >(entries, pool)

        return acquire<typename TDependency::type>(entries).create(
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

