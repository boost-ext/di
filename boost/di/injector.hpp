//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_INJECTOR_HPP
    #define BOOST_DI_INJECTOR_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/type_traits/is_same.hpp>
    #include <boost/mpl/limits/vector.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/push_back.hpp>
    #include <boost/mpl/set.hpp>
    #include <boost/mpl/insert.hpp>
    #include "boost/di/aux/pool.hpp"
    #include "boost/di/detail/module.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (   \
        BOOST_DI_ITERATION_PARAMS(          \
            1                               \
          , BOOST_MPL_LIMIT_VECTOR_SIZE     \
          , "boost/di/injector.hpp"         \
        )                                   \
    )

    namespace boost {
    namespace di {

    namespace detail {

    BOOST_MPL_HAS_XXX_TRAIT_DEF(dependencies)

    template<typename T>
    struct get_dependencies
    {
        typedef typename T::dependencies type;
    };

    template<typename T>
    struct get_pool
    {
        typedef typename T::pool type;
    };

    template<typename TSequence, typename TResult = mpl::set0<> >
    struct dependencies_impl
        : mpl::fold<
            TSequence,
            TResult,
            mpl::if_<
                has_dependencies<mpl::_2>,
                dependencies_impl<get_dependencies<mpl::_2>, mpl::_1>,
                mpl::insert<mpl::_1, mpl::_2>
            >
          >
    { };

    template<typename TSequence>
    struct pools
        : mpl::fold<
            typename mpl::fold<
                TSequence,
                mpl::set<>,
                mpl::insert< mpl::_1, get_pool<mpl::_2> >
            >::type,
            mpl::vector0<>,
            mpl::push_back<mpl::_1, mpl::_2>
          >::type
    { };

    template<typename TSequence>
    struct dependencies
        : mpl::fold<
            typename dependencies_impl<TSequence>::type,
            mpl::vector0<>,
            mpl::push_back<mpl::_1, mpl::_2>
          >::type
    { };

    } // namespace detail

    template<BOOST_DI_ARGS_TYPES_MPL(T)>
    class injector
        : public detail::module<
              typename detail::dependencies<mpl::vector<BOOST_DI_ARGS_MPL(T)> >::type
            , typename detail::pools<mpl::vector<BOOST_DI_ARGS_MPL(T)> >::type
          >
    {
    public:
        injector() { }

        #include BOOST_PP_ITERATE()
    };

    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_ARGS_TYPES(M)>
    injector(BOOST_DI_ARGS(M, module))
        : detail::module<
              typename detail::dependencies<mpl::vector<BOOST_DI_ARGS_MPL(T)> >::type
            , typename detail::pools<mpl::vector<BOOST_DI_ARGS_MPL(T)> >::type
          >(
              BOOST_PP_ENUM_BINARY_PARAMS(
                  BOOST_PP_ITERATION()
                , module
                , .get_pool() BOOST_PP_INTERCEPT
              )
          )
    { }

    //#define BOOST_DI_MODULE_ARG(_, n, M) BOOST_PP_COMMA_IF(n) const M##n& module##n = M##n()

    //template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename M)>
    //injector<
        //typename mpl::joint_view<
            //modules,
            //mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), M)>
        //>::type
    //>
    //install(BOOST_PP_REPEAT(BOOST_PP_ITERATION(), BOOST_DI_MODULE_ARG, M)) {
        //typedef injector<
            //typename mpl::joint_view<
                //modules,
                //mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), M)>
            //>::type
        //> injector_t;

        //return injector_t(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), module));
    //}

    #undef BOOST_DI_MODULE_ARG

#endif

