//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_TYPE_TRAITS_CREATE_TRAITS_HPP
    #define BOOST_DI_TYPE_TRAITS_CREATE_TRAITS_HPP

    #include "boost/di/aux_/config.hpp"
    #include "boost/di/type_traits/ctor_traits.hpp"

    #include <boost/type.hpp>
    #include <boost/non_type.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/type_traits/is_class.hpp>
    #include <boost/type_traits/is_same.hpp>
    #include <boost/type_traits/is_abstract.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/bool.hpp>
    #include <boost/mpl/or.hpp>
    #include <boost/mpl/void.hpp>
    #include <boost/mpl/has_xxx.hpp>
    #include <boost/mpl/aux_/yes_no.hpp>

    namespace boost {

    namespace mpl {
        struct string_tag;
        template<typename> struct c_str;
    } // namespace mpl

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
        typedef has_value type;
        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test((base*)(0))) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename, typename = void>
    struct is_mpl_string
        : mpl::false_
    { };

    BOOST_MPL_HAS_XXX_TRAIT_DEF(tag)

    template<typename T>
    struct is_mpl_string<T, typename enable_if<has_tag<T> >::type>
        : is_same<mpl::string_tag, typename T::tag>
    { };

    template<typename T>
    struct is_explicit
    {
        BOOST_STATIC_CONSTANT(
            bool
          , value = has_value<T>::value || is_mpl_string<T>::value
        );
    };

    template<bool>
    struct policy
    { };

    template<typename TExpected, typename TGiven>
    TGiven* create_traits_impl(const policy<false>&) {
        return new TGiven();
    }

    template<typename TExpected, typename TGiven>
    typename disable_if<is_abstract<TGiven>, TExpected*>::type
    create_traits_impl(const policy<true>&) {
        return new TGiven();
    }

    template<typename TExpected, typename TGiven>
    typename enable_if<is_abstract<TGiven>, TExpected*>::type
    create_traits_impl(const policy<true>&) {
        return new TGiven();
    }

    template<typename TPolicy, typename TExpected, typename TGiven>
    typename disable_if<is_explicit<TGiven>, TExpected*>::type
    create_traits() {
        return create_traits_impl<TExpected, TGiven>(TPolicy());
    }

    template<typename T, typename TExpected, typename TGiven>
    typename enable_if<has_value<TGiven>, TExpected*>::type
    create_traits() {
        return new TExpected(TGiven::value);
    }

    template<typename T, typename TExpected, typename TGiven>
    typename enable_if<is_mpl_string<TGiven>, TExpected*>::type
    create_traits() {
        return new TExpected(mpl::c_str<TGiven>::value);
    }

    #define BOOST_PP_FILENAME_1 "boost/di/type_traits/create_traits.hpp"
    #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM(1)
    #include BOOST_PP_ITERATE()

    } // namespace type_traits
    } // namespace di
    } // namespace boost

    #endif

#else

    template<typename T, typename TExpected, typename TGiven, BOOST_DI_TYPES(Args)>
    TExpected* create_traits(BOOST_DI_ARGS(Args, args)) {
        return new TGiven(BOOST_DI_ARGS_PASS(args));
    }

    #undef BOOST_DI_CONVERT

#endif

