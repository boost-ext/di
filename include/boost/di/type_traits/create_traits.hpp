//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_TYPE_TRAITS_CREATE_TRAITS_HPP
    #define BOOST_DI_TYPE_TRAITS_CREATE_TRAITS_HPP

    #include <string>
    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/cat.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/type_traits/is_arithmetic.hpp>
    #include <boost/non_type.hpp>
    #include <boost/mpl/string.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/bool.hpp>
    #include <boost/mpl/or.hpp>
    #include <boost/mpl/aux_/yes_no.hpp>
    #include <boost/mpl/void.hpp>

    #include "boost/di/type_traits/ctor_traits.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (               \
        BOOST_DI_ITERATION_PARAMS(                      \
            1                                           \
          , BOOST_DI_LIMIT_SIZE                         \
          , "boost/di/type_traits/create_traits.hpp"    \
        )                                               \
    )

    namespace boost {
    namespace di {
    namespace type_traits {

    template<typename T>
    class has_value
    {
        struct helper { static int value; };
        struct base
            : helper
            , mpl::if_<
                  is_arithmetic<T>
                , mpl::void_
                , T
              >::type
        { };

        template<typename C> static mpl::aux::no_tag  test(non_type<const int*, &C::value>*);
        template<typename>   static mpl::aux::yes_tag test(...);

    public:
        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<base>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename>
    struct is_mpl_string
        : mpl::false_
    { };

    template<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, int C)>
    struct is_mpl_string<mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)> >
        : mpl::true_
    { };

    template<typename T>
    struct is_special
    {
        static const bool value =
            BOOST_PP_CAT(has_, BOOST_DI_CREATE)<T>::value or
            has_value<T>::value or
            is_mpl_string<T>::value;
    };

    template<typename TExpected, typename TGiven>
    typename enable_if<BOOST_PP_CAT(has_, BOOST_DI_CREATE)<TGiven>, TExpected*>::type
    create_traits() {
        return TGiven().BOOST_DI_CREATE();
    }

    template<typename TExpected, typename TGiven>
    typename disable_if<is_special<TGiven>, TExpected*>::type
    create_traits() {
        return new TGiven();
    }

    template<typename TExpected, typename TGiven>
    typename enable_if<has_value<TGiven>, TExpected*>::type
    create_traits() {
        return new TExpected(TGiven::value);
    }

    template<typename TExpected, typename TGiven>
    typename enable_if<is_mpl_string<TGiven>, TExpected*>::type
    create_traits() {
        return new TExpected(mpl::c_str<TGiven>::value);
    }

    #include BOOST_PP_ITERATE()

    } // namespace type_traits
    } // namespace di
    } // namespace boost

    #endif

#else

    template<typename TExpected, typename TGiven, BOOST_DI_TYPES(Args)>
    typename enable_if<BOOST_PP_CAT(has_, BOOST_DI_CREATE)<TGiven>, TExpected*>::type
    create_traits(BOOST_DI_ARGS(Args, args)) {
        return TGiven().BOOST_DI_CREATE(BOOST_DI_ARGS_FORWARD(args));
    }

    template<typename TExpected, typename TGiven, BOOST_DI_TYPES(Args)>
    typename disable_if<BOOST_PP_CAT(has_, BOOST_DI_CREATE)<TGiven>, TExpected*>::type
    create_traits(BOOST_DI_ARGS(Args, args)) {
        return new TGiven(BOOST_DI_ARGS_FORWARD(args));
    }

#endif

