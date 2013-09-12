//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_DETAIL_POOL_HPP
    #define BOOST_DI_DETAIL_POOL_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/iteration/local.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/apply.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/back_inserter.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/has_xxx.hpp>

    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (   \
        BOOST_DI_ITERATION_PARAMS(          \
            1                               \
          , BOOST_DI_LIMIT_SIZE             \
          , "boost/di/detail/pool.hpp"      \
        )                                   \
    )

    #if defined(__GNUC__) && (__GNUC__ >= 4)
    #    pragma GCC diagnostic ignored "-Wreorder"
    #endif

    namespace boost {
    namespace di {
    namespace detail {

    class init { };

    template<typename T>
    struct never
        : mpl::false_
    { };

    template<
        typename TSeq = mpl::vector0<>
      , typename TIgnore = never<mpl::_1>
      , typename = void
    >
    class pool
    {
    public:
        typedef pool type;
        typedef TSeq types;

        pool() { }

        template<typename T>
        explicit pool(const pool<T>&, const init&)
        { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    #include BOOST_PP_ITERATE()

    } // namespace detail
    } // namespace di
    } // namespace boost

    #endif

#else
    #define n BOOST_PP_ITERATION()

    #define BOOST_DI_DERIVES_IMPL(_, n, types)                          \
        BOOST_PP_COMMA_IF(n) public mpl::at_c<types, n>::type

    template<
        typename TSeq
      , typename TIgnore
    >
    class pool<
        TSeq
      , TIgnore
      , typename enable_if_c<mpl::size<TSeq>::value == n>::type
    >
        : BOOST_PP_REPEAT(
              n
            , BOOST_DI_DERIVES_IMPL
            , TSeq
          )
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(types)

        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type; // ignore type
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }

        #define BOOST_DI_CTOR_INITLIST_IMPL(_, n, na)                   \
            BOOST_PP_COMMA_IF(n) Args##n(args##n)

        #define BOOST_PP_LOCAL_MACRO(n)                                 \
            template<BOOST_DI_TYPES_IMPL(n, Args)>                      \
            explicit pool(BOOST_DI_ARGS_IMPL(n, Args, args))            \
                : BOOST_PP_REPEAT(n, BOOST_DI_CTOR_INITLIST_IMPL, ~)    \
            { }

        #define BOOST_PP_LOCAL_LIMITS (1, n)
        #include BOOST_PP_LOCAL_ITERATE()
        #undef BOOST_DI_CTOR_INITLIST_IMPL

        #define BOOST_DI_CTOR_INITLIST_IMPL(_, n, na)                   \
            BOOST_PP_COMMA_IF(n)                                        \
            mpl::at_c<typename pool<T, I>::types, n>::type(             \
                p.get<typename mpl::at_c<                               \
                    typename pool<T, I>::types                          \
                  , n                                                   \
                >::type>()                                              \
            )

        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }

        #define BOOST_PP_LOCAL_MACRO(n)                                 \
            template<typename T, typename I>                            \
            explicit pool(                                              \
                const pool<T, I>& p                                     \
              , const init&                                             \
              , typename enable_if_c<                                   \
                    mpl::size<typename pool<T, I>::types>::value == n   \
                >::type* = 0)                                           \
                : BOOST_PP_REPEAT(                                      \
                      n                                                 \
                    , BOOST_DI_CTOR_INITLIST_IMPL                       \
                    , ~                                                 \
                  )                                                     \
            { }

        #define BOOST_PP_LOCAL_LIMITS (1, n)
        #include BOOST_PP_LOCAL_ITERATE()
        #undef BOOST_DI_CTOR_INITLIST_IMPL

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    #undef BOOST_DI_DERIVES_IMPL
    #undef n

#endif

