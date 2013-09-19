//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_MODULE_HPP
    #define BOOST_DI_MODULE_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/joint_view.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/and.hpp>
    #include <boost/mpl/back_inserter.hpp>
    #include <boost/mpl/is_sequence.hpp>
    #include <boost/mpl/placeholders.hpp>
    #include <boost/mpl/has_xxx.hpp>

    #include "boost/di/detail/module.hpp"
    #include "boost/di/detail/pool.hpp"
    #include "boost/di/scopes/deduce.hpp"
    #include "boost/di/policy.hpp"
    #include "boost/di/concepts.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (   \
        BOOST_DI_ITERATION_PARAMS(          \
            1                               \
          , BOOST_DI_LIMIT_SIZE             \
          , "boost/di/injector.hpp"         \
        )                                   \
    )

    namespace boost {
    namespace di {

    namespace detail {

    template<typename T>
    struct default_scope
        : scope<scopes::deduce>::bind<T>
    { };

    BOOST_MPL_HAS_XXX_TRAIT_DEF(deps)
    BOOST_MPL_HAS_XXX_TRAIT_DEF(policies)

    template<typename T>
    struct deps
    {
        typedef typename T::deps type;
    };

    template<typename T>
    struct policies
    {
        typedef typename T::policies type;
    };

    template<typename TSeq>
    struct concepts
        : mpl::fold<
              TSeq
            , mpl::vector0<>
            , mpl::copy<
                  mpl::if_<
                      mpl::is_sequence<mpl::_2>
                    , mpl::_2
                    , mpl::if_<
                          mpl::and_<has_deps<mpl::_2>, has_policies<mpl::_2> >
                        , mpl::joint_view<deps<mpl::_2>, policies<mpl::_2> >
                        , mpl::if_<
                              is_base_of<detail::policy_impl, mpl::_2>
                            , mpl::vector1<mpl::_2>
                            , default_scope<mpl::_2>
                          >
                      >
                  >
                , mpl::back_inserter<mpl::_1>
              >
          >::type
    { };

    } // namespace detail

    template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
    class injector
        : public detail::module<
              typename detail::concepts<
                  mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
              >::type
          >
    {
        typedef mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> deps_t;

        template<typename TSeq = mpl::vector0<> >
        struct joint_concepts
            : detail::concepts<mpl::joint_view<deps_t, TSeq> >::type
        { };

    public:
        injector() { }

        injector<> operator()() const {
            return injector<>();
        }

        #include BOOST_PP_ITERATE()
    };

    } // namespace di
    } // namespace boost

    #endif

#else
    template<BOOST_DI_TYPES(Args)>
    explicit injector(BOOST_DI_ARGS(Args, args))
        : detail::module<typename joint_concepts<>::type>(BOOST_DI_ARGS_PASS(args))
    { }

    template<BOOST_DI_TYPES(Args)>
    injector<joint_concepts<mpl::vector<BOOST_DI_TYPES_PASS(Args)> > >
    operator()(BOOST_DI_ARGS(Args, args)) const {
        return injector<joint_concepts<mpl::vector<BOOST_DI_TYPES_PASS(Args)> > >(
            BOOST_DI_ARGS_PASS(args)
        );
    }

#endif

