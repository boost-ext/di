//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_FUSION_MODULE_HPP
    #define BOOST_DI_FUSION_MODULE_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/back_inserter.hpp>
    #include <boost/mpl/placeholders.hpp>
    #include "boost/di/detail/module.hpp"
    #include "boost/di/concepts.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (   \
        BOOST_DI_ITERATION_PARAMS(          \
            1                               \
          , BOOST_MPL_LIMIT_VECTOR_SIZE     \
          , "boost/di/fusion_module.hpp"    \
        )                                   \
    )

    namespace boost {
    namespace di {

    namespace detail {

    template<typename TDeps>
    struct fusion_deps
        : mpl::fold<
              TDeps
            , mpl::vector0<>
            , mpl::copy<
                  mpl::_2
                , mpl::back_inserter<mpl::_1>
              >
          >::type
    { };

    } // namespace detail

    template<typename TDeps = mpl::vector0<> >
    class fusion_module
        : public detail::module<
              typename detail::fusion_deps<TDeps>::type
            , TDeps
          >
    {
    public:
        fusion_module() { }

        #include BOOST_PP_ITERATE()

        fusion_module<> operator()() const {
            return fusion_module<>();
        }
    };

    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    explicit fusion_module(BOOST_DI_ARGS(Args, args))
        : detail::module<
              typename detail::fusion_deps<TDeps>::type
            , TDeps
          >
        (BOOST_DI_ARGS_FORWARD(args))
    { }

    template<BOOST_DI_TYPES(Args)>
    fusion_module<mpl::vector<BOOST_DI_TYPES_PASS(Args)> >
    operator()(BOOST_DI_ARGS(Args, args)) const {
        return fusion_module<mpl::vector<BOOST_DI_TYPES_PASS(Args)> >(
            BOOST_DI_ARGS_FORWARD(args));
    }

#endif

