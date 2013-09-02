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
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/back_inserter.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/size.hpp>

    #include "boost/di/type_traits/has_traits.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (   \
        BOOST_DI_ITERATION_PARAMS(          \
            1                               \
          , BOOST_MPL_LIMIT_VECTOR_SIZE     \
          , "boost/di/detail/pool.hpp"      \
        )                                   \
    )

    #if defined(__GNUC__) && (__GNUC__ >= 4)
    #    pragma GCC diagnostic ignored "-Wreorder"
    #endif

    namespace boost {
    namespace di {
    namespace detail {

    template<
        typename TTypes = mpl::vector0<>
      , typename = void
    >
    class pool
    {
    public:
        typedef pool type;
        typedef TTypes types;

        pool() { }

        template<typename T>
        explicit pool(const pool<T>&)
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

    #define BOOST_DI_DERIVES_IMPL(_, n, types)                              \
        BOOST_PP_COMMA_IF(n) public mpl::at_c<types, n>::type

    template<typename TTypes>
    class pool<
        TTypes
      , typename enable_if_c<mpl::size<TTypes>::value == n>::type
    >
        : BOOST_PP_REPEAT(
              n
            , BOOST_DI_DERIVES_IMPL
            , TTypes
          )
    {
        template<typename T, typename = void>
        struct types_impl
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct types_impl<
            T
          , typename enable_if<
                type_traits::has_types<T>
            >::type
        >
        {
            typedef typename T::types type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TTypes
                , mpl::vector0<>
                , mpl::copy<
                      types_impl<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }

        #define BOOST_DI_CTOR_INITLIST_IMPL(_, n, na)                       \
            BOOST_PP_COMMA_IF(n) Args##n(args##n)

        #define BOOST_PP_LOCAL_MACRO(n)                                     \
            template<BOOST_DI_TYPES_IMPL(n, Args)>                          \
            explicit pool(BOOST_DI_ARGS_IMPL(n, Args, args))                \
                : BOOST_PP_REPEAT(n, BOOST_DI_CTOR_INITLIST_IMPL, ~)        \
            { }

        #define BOOST_PP_LOCAL_LIMITS (1, n)
        #include BOOST_PP_LOCAL_ITERATE()
        #undef BOOST_DI_CTOR_INITLIST_IMPL

        #define BOOST_DI_CTOR_INITLIST_IMPL(_, n, types)                    \
            BOOST_PP_COMMA_IF(n) mpl::at_c<types, n>::type(                 \
                p.get<typename mpl::at_c<types, n>::type>())

        template<typename T>
        explicit pool(
            const pool<T>&
          , typename enable_if_c<
                mpl::size<typename pool<T>::types>::value == 0
            >::type* = 0)
        { }

        #define BOOST_PP_LOCAL_MACRO(n)                                     \
            template<typename T>                                            \
            explicit pool(                                                  \
                const pool<T>& p                                            \
              , typename enable_if_c<                                       \
                    mpl::size<typename pool<T>::types>::value == n          \
                >::type* = 0)                                               \
                : BOOST_PP_REPEAT(                                          \
                      n                                                     \
                    , BOOST_DI_CTOR_INITLIST_IMPL                           \
                    , typename pool<T>::types                               \
                  )                                                         \
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

