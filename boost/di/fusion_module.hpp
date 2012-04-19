//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_MODULE_fusion_module_HPP
    #define BOOST_DI_MODULE_fusion_module_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/enum_binary_params.hpp>
    #include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
    #include <boost/typeof/typeof.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/mpl/limits/vector.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/back_inserter.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/if.hpp>
    #include "boost/di/aux/pool.hpp"
    #include "boost/di/scopes/per_request.hpp"
    #include "boost/di/scopes/singleton.hpp"
    #include "boost/di/concepts.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, BOOST_MPL_LIMIT_VECTOR_SIZE, "boost/di/fusion_module.hpp"))

    namespace boost {
    namespace di {

    template<
        typename TSequence = mpl::vector0<>
    >
    class fusion_module : public aux::module
    {
    public:
        typedef aux::pool<TSequence> pool;

        struct dependencies
            : mpl::fold<
                  TSequence
                , mpl::vector0<>
                , mpl::copy<
                      mpl::if_<
                          is_base_of<aux::internal, mpl::_2>
                        , mpl::_2
                        , per_request<mpl::_2>
                      >
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        fusion_module() { }

        #include BOOST_PP_ITERATE()

        fusion_module<> operator()() const { return fusion_module<>(); }

        const pool& get_pool() const { return pool_; }

    private:
        pool pool_;
    };

    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    fusion_module(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &arg))
        : pool_(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), arg))
    { }

    template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    fusion_module<mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), Arg)> >
    operator()(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &arg)) const
    {
        return fusion_module< mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), Arg)> >(
            BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), arg));
    }

#endif

