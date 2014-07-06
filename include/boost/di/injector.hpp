//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_INJECTOR_HPP
    #define BOOST_DI_INJECTOR_HPP

    #include "boost/di/aux_/config.hpp"
    #include "boost/di/core/module.hpp"
    #include "boost/di/core/pool.hpp"
    #include "boost/di/scopes/deduce.hpp"
    #include "boost/di/concepts.hpp"
    #include "boost/di/type_traits/has_configure.hpp"
    #include "boost/di/type_traits/function_traits.hpp"

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/joint_view.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/or.hpp>
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
    struct is_module
        : mpl::or_<
              has_deps<T>
            , type_traits::has_configure<T>
          >
    { };

    template<typename T, typename = void>
    struct get_module
    {
        typedef T type;
    };

    template<typename T>
    struct get_module<T, typename enable_if<type_traits::has_configure<T> >::type>
    {
        typedef typename type_traits::function_traits<
            BOOST_DI_FEATURE_DECLTYPE(&T::configure)
        >::result_type type;
    };

    template<typename T, typename = void>
    struct get_deps
    {
        typedef typename T::deps type;
    };

    template<typename T>
    struct get_deps<T, typename enable_if<type_traits::has_configure<T> >::type>
    {
        typedef typename get_module<T>::type::deps type;
    };

    template<
        typename TSeq
      , typename TDefaultScope = default_scope<mpl::_2>
    >
    struct concepts
        : mpl::fold<
              TSeq
            , mpl::vector0<>
            , mpl::copy<
                  mpl::if_<
                      mpl::is_sequence<mpl::_2>
                    , mpl::_2
                    , mpl::if_<
                          is_module<mpl::_2>
                        , get_deps<mpl::_2>
                        , TDefaultScope
                      >
                  >
                , mpl::back_inserter<mpl::_1>
              >
          >::type
    { };

    } // namespace detail

    template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
    class injector
        : public core::module<
              typename detail::concepts<
                  BOOST_DI_MPL_VECTOR_TYPES_PASS_MPL(T)
              >::type
          >
    {
        template<typename TSeq = mpl::vector0<> >
        struct joint_concepts
            : detail::concepts<
                  mpl::joint_view<
                      BOOST_DI_MPL_VECTOR_TYPES_PASS_MPL(T)
                    , TSeq
                  >
              >::type
        { };

    public:
        injector() { }

        #define BOOST_PP_FILENAME_1 "boost/di/injector.hpp"
        #define BOOST_PP_ITERATION_LIMITS BOOST_DI_TYPES_MPL_LIMIT_FROM(1)
        #include BOOST_PP_ITERATE()

    private:
        template<typename T>
        const T& pass_arg(const T& arg, typename disable_if<type_traits::has_configure<T> >::type* = 0) const {
            return arg;
        }

        template<typename T>
        typename detail::get_module<T>::type
        pass_arg(const T& arg
               , typename enable_if<type_traits::has_configure<T> >::type* = 0) const {
            return arg.configure();
        }
    };

    } // namespace di
    } // namespace boost

    #endif

#else

    #define BOOST_DI_PASS_ARG(_, n, arg) BOOST_PP_COMMA_IF(n) pass_arg(arg##n)

    template<BOOST_DI_TYPES(TArgs)>
    explicit injector(BOOST_DI_ARGS(TArgs, args))
        : core::module<typename joint_concepts<>::type>(
            BOOST_PP_REPEAT(BOOST_PP_ITERATION(), BOOST_DI_PASS_ARG, args)
          )
    { }

    #undef BOOST_DI_PASS_ARG

#endif

