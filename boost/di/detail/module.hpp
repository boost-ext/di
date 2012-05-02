//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_DETAIL_MODULE_HPP
    #define BOOST_DI_DETAIL_MODULE_HPP

    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/mpl/limits/vector.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/filter_view.hpp>
    #include <boost/mpl/joint_view.hpp>
    #include <boost/mpl/inherit_linearly.hpp>
    #include <boost/mpl/inherit.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/deref.hpp>
    #include <boost/mpl/begin_end.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/copy_if.hpp>
    #include <boost/mpl/back_inserter.hpp>
    #include "boost/di/aux/pool.hpp"
    #include "boost/di/aux/make_plain.hpp"
    #include "boost/di/aux/instance.hpp"
    #include "boost/di/aux/dependency.hpp"
    #include "boost/di/aux/converter.hpp"
    #include "boost/di/detail/creator.hpp"
    #include "boost/di/detail/visitor.hpp"
    #include "boost/di/scopes/per_request.hpp"
    #include "boost/di/concepts.hpp"
    #include "boost/di/policy.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (   \
        BOOST_DI_ITERATION_PARAMS(          \
            1                               \
          , BOOST_MPL_LIMIT_VECTOR_SIZE     \
          , "boost/di/detail/module.hpp"    \
        )                                   \
    )

    namespace boost {
    namespace di {
    namespace detail {

    //namespace detail {

    template<typename TDeps>
    struct dependencies
        : mpl::fold<
              typename mpl::fold<
                  TDeps
                , mpl::vector0<>
                , mpl::copy<
                      mpl::if_<
                          is_base_of<aux::instance, mpl::_2>
                        , mpl::vector0<>
                        , mpl::if_<
                              mpl::is_sequence<mpl::_2>
                            , mpl::_2
                            , per_request<mpl::_2>
                          >
                      >
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
          >::type
    { };

    //} // namespace detail

    template<
        typename TDeps
      , template<
            typename = typename dependencies<TDeps>::type
          , typename = void
        > class TPool = aux::pool
      , template<
            typename = typename dependencies<TDeps>::type
        > class TCreator = creator
      , template<
            typename = typename dependencies<TDeps>::type
        > class TVisitor = visitor
    >
    class module
    {
    public:
        typedef typename dependencies<TDeps>::type deps;
        typedef deps dependencies; //TODO
        typedef TPool<deps> pool;  //TODO

    private:
        struct entries
            : mpl::inherit_linearly<deps, mpl::inherit<mpl::_1, mpl::_2> >::type
        { };

        struct policies
            : mpl::deref<
                  typename mpl::begin<
                       mpl::joint_view<
                          mpl::filter_view<deps, is_base_of<policy, mpl::_1> >
                        , mpl::vector1<typename defaults<policy, specialized>::type>
                      >
                  >::type
              >::type
        { };

    public:
        module() { }

        #include BOOST_PP_ITERATE()

        template<typename T>
        T create() {
            typedef typename policies::template verify<deps, T>::type policies_t;
            return TCreator<>::template execute<T, mpl::vector0<> >(entries_, pool_);
        }

        template<typename T, typename Visitor>
        void visit(const Visitor& visitor) {
            typedef typename policies::template verify<deps, T>::type policies_t;
            TVisitor<>::template execute<T, mpl::vector0<> >(visitor);
        }

    private:
        TPool<deps> pool_;
        entries entries_;
    };

    } // namespace detail
    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_ARGS_TYPES(Args)>
    module(BOOST_DI_ARGS(Args, args))
        : pool_(BOOST_DI_ARGS_FORWARD(args))
    { }

#endif

