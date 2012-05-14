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
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/not.hpp>
    #include <boost/mpl/or.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/find_if.hpp>
    #include <boost/mpl/begin_end.hpp>
    #include <boost/mpl/is_sequence.hpp>
    #include <boost/mpl/back_inserter.hpp>
    #include "boost/di/aux/instance.hpp"
    #include "boost/di/aux/has_traits.hpp"
    #include "boost/di/detail/module.hpp"
    #include "boost/di/concepts.hpp"
    #include "boost/di/config.hpp"

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

    template<typename T>
    struct derived
    {
        typedef typename T::derived type;
    };

    template<typename TDeps, typename T = derived<mpl::_1> >
    struct generic_deps
        : mpl::transform<
              typename mpl::fold<
                  TDeps
                , mpl::vector0<>
                , mpl::copy<
                      mpl::if_<
                          mpl::is_sequence<mpl::_2>
                        , mpl::_2
                        , per_request<mpl::_2>
                      >
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
            , mpl::if_<
                  aux_::has_element_type<mpl::_1> //is instance
                , T
                , mpl::_1
              >
          >::type
    { };

    } // namespace detail

    template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
    class generic_module
        : public detail::module<
              typename detail::generic_deps<mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> >::type
          >
    {
        struct annotations
            : detail::generic_deps<mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>, mpl::_1>
        { };

        template<
            typename TInstance
          , typename T
          , typename = void
        >
        struct is_same_instance
            : mpl::false_
        { };

        template<
            typename TInstance
          , typename T
        >
        struct is_same_instance<
            TInstance
          , T
          , typename enable_if<aux_::has_element_type<TInstance> >::type
        >
            : mpl::or_<
                  is_same<typename TInstance::name, T>
                , is_same<typename TInstance::element_type, T>
              >
        { };

        template<typename T>
        struct find_instance_type
            : mpl::find_if<annotations, is_same_instance<mpl::_1, T> >::type
        { };

        template<typename T>
        struct disable_if_instance_not_found
            : disable_if<
                  is_same<
                      find_instance_type<T>
                    , mpl::end<annotations>
                  >
                , typename find_instance_type<T>::type::derived
              >
        { };

    public:
        generic_module() { }

        #include BOOST_PP_ITERATE()

        template<typename T, typename TValue>
        static typename disable_if_instance_not_found<T>::type
        set(TValue value) {
            typedef typename find_instance_type<T>::type annotation;
            return typename annotation::derived(value);
        }
    };

    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    explicit generic_module(BOOST_DI_ARGS(Args, args))
        : detail::module<
              typename detail::generic_deps<mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> >::type
          >
        (BOOST_DI_ARGS_FORWARD(args))
    { }

#endif

