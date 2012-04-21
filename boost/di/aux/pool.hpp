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
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/back_inserter.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/has_xxx.hpp>
    #include "boost/di/aux/utility.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (   \
        BOOST_DI_PARAMS(                    \
            1                               \
          , BOOST_MPL_LIMIT_VECTOR_SIZE     \
          , "boost/di/aux/pool.hpp"         \
        )                                   \
    )

    namespace boost {
    namespace di {
    namespace aux {

    template<
        typename TSequence = mpl::vector0<>
      , typename Enable = void
    >
    class pool
    {
    public:
        typedef TSequence sequence;

        pool() { }

        template<typename T>
        struct result_type
        {
            typedef typename T::result_type type;
        };

        template<typename T>
        typename result_type<T>::type get() const
        {
            return T::get();
        }
    };

    #include BOOST_PP_ITERATE()

    } // namespace aux
    } // namespace di
    } // namespace boost

    #endif

#else
    #define BOOST_DI_DERIVES_IMPL(_, n, sequence)                   \
        BOOST_PP_COMMA_IF(n) public mpl::at_c<sequence, n>::type

    #define BOOST_DI_CTOR_INITLIST_IMPL(_, n, na)                   \
        BOOST_PP_COMMA_IF(n) Args##n(args##n)

    template<typename TSequence>
    class pool<TSequence, typename enable_if_c<mpl::size<TSequence>::value == BOOST_PP_ITERATION()>::type>
        : BOOST_PP_REPEAT(BOOST_PP_ITERATION(), BOOST_DI_DERIVES_IMPL, TSequence)
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(sequence)

    public:
        struct sequence
            : mpl::fold<
                TSequence,
                mpl::vector0<>,
                mpl::copy<
                    mpl::if_<
                        has_sequence<mpl::_2>,
                        get_sequence<mpl::_2>,
                        typename mpl::vector<mpl::_2>::type
                    >,
                    mpl::back_inserter<mpl::_1>
                >
            >::type
        { };

        #if __GNUC__ >= 4
        # pragma GCC diagnostic ignored "-Wreorder"
        #endif

        pool() { }

        template<BOOST_DI_ARGS_TYPES(Args)>
        pool(BOOST_DI_ARGS(Args, args))
            : BOOST_PP_REPEAT(BOOST_PP_ITERATION(), BOOST_DI_CTOR_INITLIST_IMPL, ~)
        { }

        template<typename T>
        struct result_type
        {
            typedef typename T::result_type type;
        };

        template<typename T>
        typename result_type<T>::type get() const {
            return T::get();
        }
    };

    #undef BOOST_DI_DERIVES_IMPL
    #undef BOOST_DI_CTOR_INITLIST_IMPL

#endif

