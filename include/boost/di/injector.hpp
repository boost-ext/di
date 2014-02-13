//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_MODULE_HPP
    #define BOOST_DI_MODULE_HPP

    #include "boost/di/aux_/meta.hpp"
    #include "boost/di/detail/module.hpp"
    #include "boost/di/detail/pool.hpp"
    #include "boost/di/scopes/deduce.hpp"
    #include "boost/di/concepts.hpp"

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/utility/enable_if.hpp>
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

    namespace boost {
    namespace di {

    namespace detail {

    template<typename T>
    struct default_scope
        : scope<scopes::deduce>::bind<T>
    { };

    BOOST_MPL_HAS_XXX_TRAIT_DEF(deps)

    template<typename T>
    struct deps
    {
        typedef typename T::deps type;
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
                          has_deps<mpl::_2>
                        , deps<mpl::_2>
                        , mpl::if_<
                              has_assert_policy<mpl::_2>
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
        template<typename TSeq = mpl::vector0<> >
        struct joint_concepts
            : detail::concepts<
                  mpl::joint_view<
                      mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
                    , TSeq
                  >
              >::type
        { };

    public:
        injector() { }

        injector<> operator()() const {
            return injector<>();
        }

        #define BOOST_PP_FILENAME_1 "boost/di/injector.hpp"
        #define BOOST_PP_ITERATION_LIMITS BOOST_DI_TYPES_MPL_LIMIT_FROM(1)
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

