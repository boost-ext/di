//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_MODULE_FUSION_HPP
    #define BOOST_DI_MODULE_FUSION_HPP

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
    #include "boost/di/module/aux/module.hpp"
    #include "boost/di/scope/per_request.hpp"
    #include "boost/di/scope/singleton.hpp"
    #include "boost/di/module/aux/bind.hpp"
    #include "boost/di/module/aux/scope.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, BOOST_MPL_LIMIT_VECTOR_SIZE, "boost/di/module/fusion.hpp"))

    namespace boost {
    namespace di {
    namespace module {

    template<typename TScope>
    struct scope
        : base::aux::scope<TScope>
    { };

    template<typename TExpected, typename TGiven = TExpected>
    struct bind
        : base::aux::bind<TExpected, TGiven>
    { };

    template<typename TExpected, typename TGiven = TExpected>
    struct singleton
        : scope<back::scopes::singleton>::bind< bind<TExpected, TGiven> >
    { };

    template<typename T> struct singleton<T, T>
        : scope<back::scopes::singleton>::bind<T>
    { };

    template<typename TExpected, typename TGiven = TExpected>
    struct per_request
        : scope<back::scopes::per_request>::bind< bind<TExpected, TGiven> >
    { };

    template<typename T>
    struct per_request<T, T>
        : scope<back::scopes::per_request>::bind<T>
    { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct singletons
        : scope<back::scopes::singleton>::bind<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
    { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct per_requests
        : scope<back::scopes::per_request>::bind<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>
    { };

    template
    <
        typename TSeq = boost::mpl::vector0<>
    >
    class module : public aux::module
    {
    public:
        typedef back::aux::pool<TSeq> pool;

        struct dependencies : boost::mpl::fold
            <
                TSeq,
                boost::mpl::vector0<>,
                boost::mpl::copy
                <
                    boost::mpl::if_
                    <
                        boost::is_base_of<base::aux::internal, boost::mpl::_2>,
                        boost::mpl::_2,
                        per_request<boost::mpl::_2>
                    >,
                    boost::mpl::back_inserter<boost::mpl::_1>
                >
            >::type
        { };

        module() { }

        #include BOOST_PP_ITERATE()

        module<> operator()() const { return module<>(); }

        const pool& get_pool() const { return pool_; }

    private:
        pool pool_;
    };

    } // namespace module
    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    module(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &arg))
        : pool_(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), arg))
    { }

    template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    module<boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), Arg)> >
    operator()(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &arg)) const
    {
        return module< boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), Arg)> >(
            BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), arg));
    }

#endif

