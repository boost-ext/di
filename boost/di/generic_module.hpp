//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_GENERIC_MODULE_HPP
    #define BOOST_DI_GENERIC_MODULE_HPP

    #include <boost/type_traits/is_same.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/limits/vector.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/not.hpp>
    #include <boost/mpl/or.hpp>
    #include <boost/mpl/find_if.hpp>
    #include <boost/mpl/begin_end.hpp>
    #include "boost/di/aux/instance.hpp"
    #include "boost/di/detail/module.hpp"
    #include "boost/di/config.hpp"

    #include "boost/di/concepts.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (   \
        BOOST_DI_ITERATION_PARAMS(          \
            1                               \
          ,  BOOST_MPL_LIMIT_VECTOR_SIZE    \
          , "boost/di/generic_module.hpp"   \
        )                                   \
    )

    namespace boost {
    namespace di {

    namespace detail {

    template<typename T, typename Enable = void>
    struct make_annotation
    {
        typedef typename annotate<aux::instance<T> >::template with<> type;
    };

    template<typename T>
    struct make_annotation<T, typename enable_if<is_base_of<concepts::annotate<>::with<>, T> >::type>
    {
        typedef typename T::template rebind<scopes::singleton>::type dependency;
        typedef aux::instance<typename dependency::expected, typename dependency::context> instance;
        typedef typename annotate<instance>::template with<typename T::name> type;
    };

    template<typename TDeps>
    struct externals
        : mpl::transform<
              typename mpl::fold<
                  TDeps
                , mpl::vector0<>
                , mpl::copy<
                      mpl::if_<
                          is_base_of<concepts::detail::externals, mpl::_2>
                        , mpl::_2
                        , mpl::vector0<>
                      >
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
            , make_annotation<mpl::_1>
          >::type
    { };

    template<typename TDeps>
    struct generic_deps
        : mpl::fold<
              TDeps
            , mpl::vector0<>
            , mpl::copy<
                  mpl::if_<
                      is_base_of<concepts::detail::externals, mpl::_2>
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
    { };

    template<typename T>
    struct get_derived
    {
        typedef typename T::derived type;
    };

    template<typename TDeps>
    struct instances
        : mpl::transform<
              externals<TDeps>
            , get_derived<mpl::_1>
          >::type
    { };

    } // namespace detail

    template<BOOST_DI_ARGS_TYPES_MPL(T)>
    class generic_module
        : public detail::module<
              typename detail::generic_deps<mpl::vector<BOOST_DI_ARGS_MPL(T)> >::type
            , typename detail::instances<mpl::vector<BOOST_DI_ARGS_MPL(T)> >::type
          >
    {
        typedef typename detail::externals<mpl::vector<BOOST_DI_ARGS_MPL(T)> >::type externals_;

        template<typename TInstance, typename T>
        struct is_same_instance
            : mpl::or_<
                  is_same<typename TInstance::name, T>
                , is_same<typename TInstance::element_type, T>
              >
        { };

        template<
            typename T
          , typename TExternals = typename detail::externals<
                mpl::vector<BOOST_DI_ARGS_MPL(T)>
            >::type
        >
        struct find_instance_type
            : mpl::find_if<TExternals, is_same_instance<mpl::_1, T> >::type
        { };

        template<
            typename T
          , typename TExternals = typename detail::externals<
                mpl::vector<BOOST_DI_ARGS_MPL(T)>
            >::type
        >
        struct disable_if_instance_not_found
            : disable_if<
                  is_same<
                      find_instance_type<T, TExternals>
                    , mpl::end<TExternals>
                  >
                , typename find_instance_type<T, TExternals>::type::derived
              >
        { };

    public:
        generic_module() { }

        #include BOOST_PP_ITERATE()

        template<typename T, typename TValue>
        static typename disable_if_instance_not_found<T>::type
        set(TValue value) {
            typedef typename find_instance_type<T, externals_>::type annotation;
            return typename annotation::derived(value);
        }
    };

    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_ARGS_TYPES(Args)>
    generic_module(BOOST_DI_ARGS(Args, args))
        : detail::module<
              typename detail::generic_deps<mpl::vector<BOOST_DI_ARGS_MPL(T)> >::type
            , typename detail::instances<mpl::vector<BOOST_DI_ARGS_MPL(T)> >::type
          >
        (BOOST_DI_ARGS_FORWARD(args))

    { }

#endif

