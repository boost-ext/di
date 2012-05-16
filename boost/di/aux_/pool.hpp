//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_AUX_POOL_HPP
    #define BOOST_DI_AUX_POOL_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/iteration/local.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/function_types/result_type.hpp>
    #include <boost/typeof/typeof.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/back_inserter.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/size.hpp>
    #include "boost/di/aux_/has_traits.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (   \
        BOOST_DI_ITERATION_PARAMS(          \
            1                               \
          , BOOST_MPL_LIMIT_VECTOR_SIZE     \
          , "boost/di/aux_/pool.hpp"        \
        )                                   \
    )

    #if (__GNUC__ >= 4)
    #    pragma GCC diagnostic ignored "-Wreorder"
    #endif

    namespace boost {
    namespace di {
    namespace aux_ {

    template<
        typename TExternals = mpl::vector0<>
      , typename = void
    >
    class pool
    {
        template<typename T>
        struct result_type
            : function_types::result_type<BOOST_TYPEOF_TPL(&T::get)>
        { };

    public:
        typedef TExternals externals;

        template<typename T>
        typename result_type<T>::type get() const
        {
            return T::get();
        }
    };

    #include BOOST_PP_ITERATE()

    } // namespace aux_
    } // namespace di
    } // namespace boost

    #endif

#else
    #define BOOST_DI_DERIVES_IMPL(_, n, sequence)                   \
        BOOST_PP_COMMA_IF(n) public mpl::at_c<sequence, n>::type

    template<typename TExternals>
    class pool<
        TExternals
      , typename enable_if_c<mpl::size<TExternals>::value == BOOST_PP_ITERATION()>::type
    >
        : BOOST_PP_REPEAT(BOOST_PP_ITERATION(), BOOST_DI_DERIVES_IMPL, TExternals)
    {
        template<typename T>
        struct result_type
            : function_types::result_type<BOOST_TYPEOF_TPL(&T::get)>
        { };

        template<typename T, typename = void>
        struct externals_impl
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct externals_impl<T, typename enable_if<has_externals<T> >::type>
        {
            typedef typename T::externals type;
        };

    public:
        struct externals
            : mpl::fold<
                  TExternals
                , mpl::vector0<>
                , mpl::copy<
                      externals_impl<mpl::_2>
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

        #define BOOST_PP_LOCAL_LIMITS (1, BOOST_PP_ITERATION())
        #include BOOST_PP_LOCAL_ITERATE()
        #undef BOOST_DI_CTOR_INITLIST_IMPL

        template<typename T>
        typename result_type<T>::type get() const {
            return T::get();
        }
    };

    #undef BOOST_DI_DERIVES_IMPL

#endif

