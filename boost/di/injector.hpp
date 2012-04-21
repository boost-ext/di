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
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/mpl/limits/vector.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/set.hpp>
    #include <boost/mpl/insert.hpp>
    #include <boost/mpl/remove_if.hpp>
    #include <boost/mpl/filter_view.hpp>
    #include <boost/mpl/joint_view.hpp>
    #include <boost/mpl/begin_end.hpp>
    #include <boost/mpl/deref.hpp>
    #include <boost/mpl/push_back.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/if.hpp>
    #include "boost/di/aux/pool.hpp"
    #include "boost/di/aux/utility.hpp"
    #include "boost/di/detail/factory.hpp"
    #include "boost/di/policy.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (   \
        BOOST_DI_PARAMS(                    \
            1                               \
          , BOOST_MPL_LIMIT_VECTOR_SIZE     \
          , "boost/di/injector.hpp"         \
        )                                   \
    )

    namespace boost {
    namespace di {

    template<BOOST_DI_ARGS_TYPES_MPL(T)>
    class injector
    {
        typedef mpl::vector<BOOST_DI_ARGS_MPL(T)> sequence;

        struct modules
            : mpl::remove_if<
                  sequence
                , is_base_of<detail::policy, mpl::_>
              >::type
        { };

        struct policies
            : mpl::joint_view<
                mpl::filter_view<sequence, is_base_of<detail::policy, mpl::_> >,
                mpl::vector1<typename defaults<detail::policy, specialized>::type>
              >::type
        { };

        template<typename TSequence, typename TResult = mpl::set0<> >
        struct dependencies_impl
            : mpl::fold<
                TSequence,
                TResult,
                mpl::if_<
                    is_base_of<aux::module, mpl::_2>,
                    dependencies_impl<aux::get_dependencies<mpl::_2>, mpl::_1>,
                    mpl::insert<mpl::_1, mpl::_2>
                >
              >
        { };

        struct externals
            : mpl::fold<
                typename mpl::fold<
                    modules,
                    mpl::set<>,
                    mpl::insert< mpl::_1, aux::get_pool<mpl::_2> >
                >::type,
                mpl::vector0<>,
                mpl::push_back<mpl::_1, mpl::_2>
              >::type
        { };

        struct dependencies
            : mpl::fold<
                typename dependencies_impl<modules>::type,
                mpl::vector0<>,
                mpl::push_back<mpl::_1, mpl::_2>
              >::type
        { };

        typedef aux::pool<typename externals::type> pool;
        typedef typename mpl::deref<typename mpl::begin<policies>::type>::type policy;
        typedef detail::factory<typename dependencies::type, pool, policy> factory;

    public:
        injector()
            : pool_(), factory_(pool_)
        { }

        #include BOOST_PP_ITERATE()

        template<typename T> T create() {
            return factory_.create<T>();
        }

        template<typename T, typename Visitor>
        void visit(const Visitor& visitor) {
            return factory_.visit<T>(visitor);
        }

    private:
        pool pool_;
        factory factory_;
    };

    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_ARGS_TYPES(M)>
    injector(BOOST_DI_ARGS(M, module))
        : pool_(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), module, .get_pool() BOOST_PP_INTERCEPT))
        , factory_(pool_)
    { }

    #define BOOST_DI_MODULE_ARG(_, n, M) BOOST_PP_COMMA_IF(n) const M##n& module##n = M##n()

    template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename M)>
    injector<
        typename mpl::joint_view<
            modules,
            mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), M)>
        >::type
    >
    install(BOOST_PP_REPEAT(BOOST_PP_ITERATION(), BOOST_DI_MODULE_ARG, M)) {
        typedef injector<
            typename mpl::joint_view<
                modules,
                mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), M)>
            >::type
        > injector_t;

        return injector_t(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), module));
    }

    #undef BOOST_DI_MODULE_ARG

#endif

