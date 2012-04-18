//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_FRONT_BASE_MODULE_HPP
    #define BOOST_DI_FRONT_BASE_MODULE_HPP

    #include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/type_traits/is_same.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/limits/vector.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/not.hpp>
    #include <boost/mpl/or.hpp>
    #include <boost/mpl/find_if.hpp>
    #include <boost/mpl/back_inserter.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/transform.hpp>
    #include <boost/mpl/begin_end.hpp>
    #include "di/back/aux/pool.hpp"
    #include "di/back/aux/utility.hpp"
    #include "di/back/aux/instance.hpp"
    #include "di/back/module.hpp"
    #include "di/back/scopes/singleton.hpp"
    #include "di/back/scopes/per_request.hpp"
    #include "di/front/base/aux/internal.hpp"
    #include "di/front/base/aux/scope.hpp"
    #include "di/front/base/aux/bind.hpp"
    #include "di/front/base/aux/externals.hpp"
    #include "di/front/base/aux/annotate.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, BOOST_MPL_LIMIT_VECTOR_SIZE, "di/front/base/module.hpp"))

    namespace di
    {
    namespace front
    {
    namespace base
    {

    template<typename TScope> struct scope : aux::scope<TScope> { };
    template<typename TExpected, typename TGiven = TExpected> struct bind : aux::bind<TExpected, TGiven> { };

    template<typename TExpected, typename TGiven = TExpected> struct singleton : scope<back::scopes::singleton>::bind< bind<TExpected, TGiven> > { };
    template<typename T> struct singleton<T, T> : scope<back::scopes::singleton>::bind<T> { };

    template<typename TExpected, typename TGiven = TExpected> struct per_request : scope<back::scopes::per_request>::bind< bind<TExpected, TGiven> > { };
    template<typename T> struct per_request<T, T> : scope<back::scopes::per_request>::bind<T> { };

    template<typename T> struct External : aux::externals<T> { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct singletons : scope<back::scopes::singleton>::bind<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct per_requests : scope<back::scopes::per_request>::bind<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    struct externals : aux::externals<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> { };

    template<typename T> struct annotate : aux::annotate<T> { };

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
    class module : public back::module
    {
        template<typename TInstance, typename T> struct is_same_instance : boost::mpl::or_
            <
                boost::is_same<typename TInstance::name, T>,
                boost::is_same<typename TInstance::value_type, T>
            >
        { };

        template<typename TSeq, typename T> struct find_instance_type
            : boost::mpl::find_if<TSeq, is_same_instance<boost::mpl::_1, T> >::type
        { };

        template<typename T, typename Enable = void>
        struct make_annotation
        {
            typedef typename annotate<back::aux::instance<T> >::template with<> type;
        };

        template<typename T>
        struct make_annotation<T, typename boost::enable_if<boost::is_base_of<base::aux::internal, T> >::type>
        {
            typedef typename T::template rebind<back::scopes::singleton>::type dependency;
            typedef back::aux::instance<typename dependency::expected, typename dependency::context> instance;
            typedef typename annotate<instance>::template with<typename T::name> type;
        };

        struct externals : boost::mpl::transform
            <
                typename boost::mpl::fold
                <
                    boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
                    boost::mpl::vector0<>,
                    boost::mpl::copy
                    <
                        boost::mpl::if_
                        <
                            boost::is_base_of<aux::detail::externals, boost::mpl::_2>,
                            boost::mpl::_2,
                            boost::mpl::vector0<>
                        >,
                        boost::mpl::back_inserter<boost::mpl::_1>
                    >
                >::type,
                make_annotation<boost::mpl::_1>
            >::type
        { };

        struct instances : boost::mpl::transform
            <
                externals,
                back::aux::get_derived<boost::mpl::_1>
            >::type
        { };

    public:
        typedef back::aux::pool<typename instances::type> pool;

        struct dependencies : boost::mpl::fold
            <
                boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
                boost::mpl::vector0<>,
                boost::mpl::copy
                <
                    boost::mpl::if_
                    <
                        boost::is_base_of<aux::detail::externals, boost::mpl::_2>,
                        boost::mpl::vector0<>,
                        boost::mpl::if_
                        <
                            boost::mpl::is_sequence<boost::mpl::_2>,
                            boost::mpl::_2,
                            per_request<boost::mpl::_2>
                        >
                    >,
                    boost::mpl::back_inserter<boost::mpl::_1>
                >
            >::type
        { };

        module() { }

        #include BOOST_PP_ITERATE()

        template<typename T, typename Tvalue>
        inline static typename boost::disable_if<boost::is_same<find_instance_type<externals, T>, boost::mpl::end<externals> >, typename find_instance_type<externals, T>::type::derived>::type
        set(Tvalue value)
        {
            typedef typename find_instance_type<externals, T>::type annotation;
            return typename annotation::derived(value);
        }

        const pool& get_pool() const { return pool_; }

    private:
        pool pool_;
    };

    } // namespace base
    } // namespace front
    } // namespace di

    #endif

#else

    template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    module(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &arg))
        : pool_(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), arg))
    { }

#endif

