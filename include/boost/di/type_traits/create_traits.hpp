//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_TYPE_TRAITS_CREATE_TRAITS_HPP
    #define BOOST_DI_TYPE_TRAITS_CREATE_TRAITS_HPP

    #include "boost/di/aux_/meta.hpp"
    #include "boost/di/type_traits/ctor_traits.hpp"

    #include <string>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/cat.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/type_traits/is_class.hpp>
    #include <boost/non_type.hpp>
    #include <boost/mpl/string.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/bool.hpp>
    #include <boost/mpl/or.hpp>
    #include <boost/mpl/void.hpp>
    #include <boost/mpl/aux_/yes_no.hpp>

    namespace boost {
    namespace di {
    namespace type_traits {

    template<typename T>
    class has_value
    {
        struct base_impl { int value; };
        struct base
            : base_impl
            , mpl::if_<is_class<T>, T, mpl::void_>::type
        { base() { } };

        template<typename U>
        static mpl::aux::no_tag test(
            U*
          , non_type<int base_impl::*, &U::value>* = 0
        );

        static mpl::aux::yes_tag test(...);

    public:
        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test((base*)(0))) == sizeof(mpl::aux::yes_tag)
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
    struct is_explicit
    {
        BOOST_STATIC_CONSTANT(
            bool
          , value = BOOST_PP_CAT(has_, BOOST_DI_CREATE)<T>::value ||
                    has_value<T>::value ||
                    is_mpl_string<T>::value
        );
    };

    template<typename TExpected, typename TGiven>
    typename enable_if<BOOST_PP_CAT(has_, BOOST_DI_CREATE)<TGiven>, TExpected*>::type
    create_traits() {
        return TGiven().BOOST_DI_CREATE();
    }

    template<typename TExpected, typename TGiven>
    typename disable_if<is_explicit<TGiven>, TExpected*>::type
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

    #define BOOST_PP_FILENAME_1 "boost/di/type_traits/create_traits.hpp"
    #define BOOST_PP_ITERATION_LIMITS BOOST_DI_LIMITS_BEGIN(1)
    #include BOOST_PP_ITERATE()

    } // namespace type_traits
    } // namespace di
    } // namespace boost

    #endif

#else

    template<typename TExpected, typename TGiven, BOOST_DI_TYPES(Args)>
    typename enable_if<BOOST_PP_CAT(has_, BOOST_DI_CREATE)<TGiven>, TExpected*>::type
    create_traits(BOOST_DI_ARGS(Args, args)) {
        return TGiven().BOOST_DI_CREATE(BOOST_DI_ARGS_PASS(args));
    }

    template<typename TExpected, typename TGiven, BOOST_DI_TYPES(Args)>
    typename disable_if<BOOST_PP_CAT(has_, BOOST_DI_CREATE)<TGiven>, TExpected*>::type
    create_traits(BOOST_DI_ARGS(Args, args)) {
        return new TGiven(BOOST_DI_ARGS_PASS(args));
    }

#endif

