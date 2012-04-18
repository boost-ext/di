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
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/repetition/enum_binary_params.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/limits/vector.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/back_inserter.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/has_xxx.hpp>
    #include "boost/di/aux/utility.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, BOOST_MPL_LIMIT_VECTOR_SIZE, "boost/di/aux/pool.hpp"))

    namespace boost {
    namespace di {
    namespace aux {

    template
    <
        typename TSeq = boost::mpl::vector0<>,
        typename Enable = void
    >
    class pool
    {
    public:
        typedef TSeq seq;

        pool() { }

        template<typename T> struct result_type
        {
            typedef typename T::result_type type;
        };

        template<typename T> typename result_type<T>::type get() const
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
    #define BOOST_DI_DERIVES_IMPL(_, n, seq) BOOST_PP_COMMA_IF(n) public boost::mpl::at_c<seq, n>::type
    #define BOOST_DI_CTOR_INITLIST_IMPL(_, n, na) BOOST_PP_COMMA_IF(n) T##n(arg##n)

    template<typename TSeq>
    class pool<TSeq, typename boost::enable_if_c< boost::mpl::size<TSeq>::value == BOOST_PP_ITERATION()>::type>
        : BOOST_PP_REPEAT(BOOST_PP_ITERATION(), BOOST_DI_DERIVES_IMPL, TSeq)
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(seq)

    public:
        struct seq : boost::mpl::fold
            <
                TSeq,
                boost::mpl::vector0<>,
                boost::mpl::copy
                <
                    boost::mpl::if_
                    <
                        has_seq<boost::mpl::_2>,
                        get_seq<boost::mpl::_2>,
                        typename boost::mpl::vector<boost::mpl::_2>::type
                    >,
                    boost::mpl::back_inserter<boost::mpl::_1>
                >
            >::type
        { };

        #if __GNUC__ >= 4
        # pragma GCC diagnostic ignored "-Wreorder"
        #endif

        pool() { }

        template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename T)>
        pool(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const T, &arg))
            : BOOST_PP_REPEAT(BOOST_PP_ITERATION(), BOOST_DI_CTOR_INITLIST_IMPL, ~)
        { }

        template<typename T> struct result_type
        {
            typedef typename T::result_type type;
        };

        template<typename T>
        typename result_type<T>::type get() const
        {
            return T::get();
        }
    };

    #undef BOOST_DI_DERIVES_IMPL
    #undef BOOST_DI_CTOR_INITLIST_IMPL

#endif

