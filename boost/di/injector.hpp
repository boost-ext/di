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
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/is_sequence.hpp>
    #include <boost/mpl/back_inserter.hpp>
    #include "boost/di/aux_/module.hpp"
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

    template<typename TSeq>
    struct modules
        : mpl::fold<
            TSeq
          , mpl::vector0<>
          , mpl::copy<
                mpl::if_<
                    mpl::is_sequence<boost::mpl::_2>
                  , mpl::_2
                  , typename mpl::vector<boost::mpl::_2>::type
                >
              , mpl::back_inserter<boost::mpl::_1>
            >
          >::type
    { };

    } // namespace detail

    template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
    class injector
        : public aux_::module<
              detail::modules<mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> >
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
    explicit injector(BOOST_DI_ARGS(M, module))
        : aux_::module<
              detail::modules<mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> >
          >
        (BOOST_DI_ARGS_PASS(module))
    { }

    #define BOOST_DI_INJECTOR_INSTALL_ARG(_, n, M)  \
        BOOST_PP_COMMA_IF(n)                        \
        const M##n& module##n = M##n()

    template<BOOST_DI_TYPES(M)>
    injector<
        mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
      , mpl::vector<BOOST_DI_TYPES_PASS(M)>
    >
    install(BOOST_PP_REPEAT(BOOST_PP_ITERATION(), BOOST_DI_INJECTOR_INSTALL_ARG, M)) {
        return injector<
            mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
          , mpl::vector<BOOST_DI_TYPES_PASS(M)>
        >(BOOST_DI_ARGS_PASS(module));
    }

    #undef BOOST_DI_INJECTOR_INSTALL_ARG

#endif

