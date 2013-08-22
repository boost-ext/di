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
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/not.hpp>
    #include <boost/mpl/or.hpp>
    #include <boost/mpl/and.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/find_if.hpp>
    #include <boost/mpl/begin_end.hpp>
    #include <boost/mpl/is_sequence.hpp>
    #include <boost/mpl/back_inserter.hpp>

    #include "boost/di/type_traits/has_traits.hpp"
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

    namespace aux {

    template<typename T>
    struct derived
    {
        typedef typename T::derived type;
    };

    template<
        typename TDeps
      , typename _1 =
            mpl::if_<
                  type_traits::has_element_type<mpl::_1> //is external
                , derived<mpl::_1>
                , mpl::_1
            >
    >
    struct generic_deps
        : mpl::transform<
              typename mpl::fold<
                  TDeps
                , mpl::vector0<>
                , mpl::copy<
                      mpl::if_<
                          mpl::is_sequence<mpl::_2>
                        , mpl::_2
                        , deduce_<mpl::_2>
                      >
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
            , _1
          >::type
    { };

    } // namespace aux

    template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
    class generic_module
        : public detail::module<
              typename aux::generic_deps<
                  mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
              >::type
          >
    {
        struct annotations
            : aux::generic_deps<
                  mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>, mpl::_1
              >
        { };

        template<
            typename TExternal
          , typename T
          , typename = void
        >
        struct is_external
            : mpl::false_
        { };

        template<
            typename TExternal
          , typename T
        >
        struct is_external<
            TExternal
          , T
          , typename enable_if<
                mpl::and_<
                    type_traits::has_name<TExternal>
                  , type_traits::has_element_type<TExternal>
                >
            >::type
        >
            : mpl::or_<
                  is_same<typename TExternal::name, T>
                , is_same<typename TExternal::element_type, T>
              >
        { };

        template<typename T>
        struct find_external_type
            : mpl::find_if<annotations, is_external<mpl::_1, T> >::type
        { };

        template<typename T>
        struct disable_if_external_not_found
            : disable_if<
                  is_same<
                      find_external_type<T>
                    , mpl::end<annotations>
                  >
                , typename find_external_type<T>::type::derived
              >
        { };

    public:
        generic_module() { }

        #include BOOST_PP_ITERATE()

        template<typename T, typename TValue>
        static typename disable_if_external_not_found<T>::type
        set(const TValue& value) {
            typedef typename find_external_type<T>::type annotation;
            return typename annotation::derived(value);
        }

        template<typename T, typename TValue>
        static typename disable_if_external_not_found<T>::type
        set(TValue& value) {
            typedef typename find_external_type<T>::type annotation;
            return typename annotation::derived(value);
        }

        template<typename T, typename TValue>
        static typename disable_if_external_not_found<T>::type
        set(shared_ptr<TValue> value) {
            typedef typename find_external_type<T>::type annotation;
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
              typename aux::generic_deps<
                  mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
              >::type
          >
        (BOOST_DI_ARGS_FORWARD(args))
    { }

#endif

