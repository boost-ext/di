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
          , BOOST_DI_MODULES_LIMIT_SIZE     \
          , "boost/di/injector.hpp"         \
        )                                   \
    )

    namespace boost {
    namespace di {

    namespace detail {

    BOOST_MPL_HAS_XXX_TRAIT_DEF(deps)

    template<typename T>
    struct get_deps
    {
        typedef typename T::deps type;
    };

    template<typename T>
    struct get_pool
    {
        typedef typename T::pool type;
    };

    template<typename TModules, typename TResult = mpl::set0<> >
    struct deps_impl
        : mpl::fold<
              TModules
            , TResult
            , mpl::if_<
                  has_deps<mpl::_2>
                , deps_impl<get_deps<mpl::_2>, mpl::_1>
                , mpl::insert<mpl::_1, mpl::_2>
            >
          >
    { };

    template<typename TModules>
    struct pools
        : mpl::fold<
              typename mpl::fold<
                  TModules
                , mpl::set<>
                , mpl::insert<mpl::_1, get_pool<mpl::_2> >
              >::type
            , mpl::vector0<>
            , mpl::push_back<mpl::_1, mpl::_2>
          >::type
    { };

    template<typename TModules>
    struct deps
        : mpl::fold<
              typename deps_impl<TModules>::type
            , mpl::vector0<>
            , mpl::push_back<mpl::_1, mpl::_2>
          >::type
    { };

    } // namespace detail

    template<BOOST_DI_TYPES_DEFAULT_IMPL(BOOST_DI_MODULES_LIMIT_SIZE, T)>
    class injector
        : public detail::module<
              typename detail::deps<
                  mpl::vector<BOOST_DI_TYPES_PASS_IMPL(BOOST_DI_MODULES_LIMIT_SIZE, T)>
              >::type
            , typename detail::pools<
                  mpl::vector<(BOOST_DI_TYPES_PASS_IMPL(BOOST_DI_MODULES_LIMIT_SIZE, T)>
              >::type
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

    template<BOOST_DI_TYPES(M)>
    injector(BOOST_DI_ARGS(M, module))
        : detail::module<
              typename detail::deps<
                  mpl::vector<BOOST_DI_TYPES_PASS_IMPL(BOOST_DI_MODULES_LIMIT_SIZE, T)>
              >::type
            , typename detail::pools<
                  mpl::vector<(BOOST_DI_TYPES_PASS_IMPL(BOOST_DI_MODULES_LIMIT_SIZE, T)>
              >::type
          >(
              BOOST_PP_ENUM_BINARY_PARAMS(
                  BOOST_PP_ITERATION()
                , module
                , .get_pool() BOOST_PP_INTERCEPT
              )
          )
    { }

    #define BOOST_DI_INJECTOR_INSTALL_ARG(_, n, M)  \
        BOOST_PP_COMMA_IF(n)                        \
        const M##n& module##n = M##n()

    //template<BOOST_DI_ARGS_MODULES(M)>
    //injector<
        //BOOST_DI_ARGS_MODULES(T)
      //, BOOST_DI_ARGS_MODULES(M)
    //>
    //install(BOOST_PP_REPEAT(BOOST_PP_ITERATION(), BOOST_DI_INJECTOR_INSTALL_ARG, M)) {
        //return injector<
            //BOOST_DI_ARGS_MODULES(T)
          //, BOOST_DI_ARGS_PASS_TYPES(M)
        //>(BOOST_DI_ARGS_PASS(module));
    //}

    #undef BOOST_DI_INJECTOR_INSTALL_ARG

#endif

