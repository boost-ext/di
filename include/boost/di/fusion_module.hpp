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
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/back_inserter.hpp>
    #include <boost/mpl/placeholders.hpp>

    #include "boost/di/type_traits/has_traits.hpp"
    #include "boost/di/detail/module.hpp"
    #include "boost/di/detail/pool.hpp"
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

    template<typename TDeps = mpl::vector0<> >
    class fusion_module
        : public detail::module<TDeps>
    {
        template<typename TSeq>
        struct fusion_deps
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::if_<
                      type_traits::has_context<mpl::_2>
                    , mpl::push_back<mpl::_1, mpl::_2>
                    , mpl::_1
                  >
              >::type
        { };

        template<typename TSeq>
        struct ddd
            : mpl::fold<
                TSeq
              , mpl::vector0<>
              , mpl::copy<
                    mpl::if_<
                        mpl::is_sequence<boost::mpl::_2>
                      , mpl::_2
                      , typename mpl::vector1<boost::mpl::_2>::type
                    >
                  , mpl::back_inserter<boost::mpl::_1>
                >
              >::type
        { };

        template<typename TSeq, typename TExt, typename TPool>
        fusion_module<TSeq> create_fusion_module(const TPool&, typename boost::enable_if_c<mpl::size<TExt>::value == 0>::type* = 0) const {
            return fusion_module<TSeq>();
        }

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

//private:
    template<BOOST_DI_TYPES(Args)>
    explicit fusion_module(BOOST_DI_ARGS(Args, args))
        : detail::module<TDeps>
        (BOOST_DI_ARGS_FORWARD(args))
    { }

    template<typename TSeq, typename TExt, typename TPool>
    fusion_module<TSeq> create_fusion_module(const TPool& pool, typename boost::enable_if_c<mpl::size<TExt>::value == BOOST_PP_ITERATION()>::type* = 0) const {

        #define BOOST_DI_GET(z, n, _) BOOST_PP_COMMA_IF(n) pool.template get<typename boost::mpl::at_c<TExt, n>::type>()

        return fusion_module<TSeq>(
            BOOST_PP_REPEAT(
                BOOST_PP_ITERATION()
              , BOOST_DI_GET
              , ~
            )
        );

        #undef BOOST_DI_GET
    }

public:
    template<BOOST_DI_TYPES(Args)>
    fusion_module<typename ddd<mpl::vector<BOOST_DI_TYPES_PASS(Args)> >::type>
    operator()(BOOST_DI_ARGS(Args, args)) const {
        detail::pool<mpl::vector<BOOST_DI_TYPES_PASS(Args)> > pool(BOOST_DI_ARGS_FORWARD(args));
        return create_fusion_module<
            typename ddd<mpl::vector<BOOST_DI_TYPES_PASS(Args)> >::type
          , typename fusion_deps<mpl::vector<BOOST_DI_TYPES_PASS(Args)> >::type
        >(pool);
    }

#endif

