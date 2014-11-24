//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_CPP_14_TYPE_TRAITS_HPP
#define BOOST_DI_AUX_CPP_14_TYPE_TRAITS_HPP

#include "boost/di/aux_/utility.hpp"
#include <functional>

#include <boost/mpl/has_xxx.hpp>
    #include <boost/type.hpp>
    #include <boost/non_type.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/type_traits/is_same.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/type_traits/remove_reference.hpp>
    #include <boost/type_traits/remove_pointer.hpp>
    #include <boost/type_traits/remove_cv.hpp>
    #include <boost/type_traits/is_class.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/bool.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/or.hpp>
    #include <boost/mpl/placeholders.hpp>
    #include <boost/mpl/aux_/yes_no.hpp>
    #include <boost/ref.hpp>

namespace boost {
namespace di {
namespace aux {

template <bool b, class T=void>
  using enable_if_t       = typename std::enable_if<b,T>::type;  // C++14

template <bool b, class T, class F>
  using conditional_t     = typename std::conditional<b,T,F>::type;  // C++14

template<typename T, typename... TArgs>
using is_constructible_t = typename std::is_constructible<T, TArgs...>::type;

template<typename T>
using remove_pointer_t = typename std::remove_pointer<T>::type;

template<typename T>
using remove_reference_t = typename std::remove_reference<T>::type;

template<typename>
struct is_reference_wrapper
    : std::false_type
{ };

template<typename T>
struct is_reference_wrapper<::std::reference_wrapper<T>>
    : std::true_type
{ };

#if !defined(BOOST_DI_CFG_NO_BOOST)
    template<typename T>
    struct is_reference_wrapper<::boost::reference_wrapper<T>>
        : std::true_type
    { };
#endif

template<typename T>
struct unwrap_reference {
    using type = T;
};

template<typename T>
struct unwrap_reference<::std::reference_wrapper<T>> {
    using type = T;
};

#if !defined(BOOST_DI_CFG_NO_BOOST)
    template<typename T>
    struct unwrap_reference<::boost::reference_wrapper<T>> {
        using type = T;
    };
#endif

template<typename T>
using remove_accessors = std::remove_cv<aux::remove_pointer_t<aux::remove_reference_t<T>>>;

template<typename T>
using remove_accessors_t = typename remove_accessors<T>::type;

template<typename>
struct make_plain;

//BOOST_DI_HAS_MEMBER_TYPE(element_type);
//BOOST_DI_HAS_MEMBER_TYPE(named_type);

BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)
BOOST_MPL_HAS_XXX_TRAIT_DEF(named_type)

template<typename T, typename = void>
struct deref_type {
    using type = T;
};

template<typename T>
struct deref_type<T, typename std::enable_if<has_element_type<T>::value>::type> {
    using type = typename T::element_type;
};

template<typename T>
struct deref_type<T, typename std::enable_if<has_named_type<T>::value>::type> {
    using type = typename make_plain<typename T::named_type>::type;
};

template<typename T>
struct make_plain
    : deref_type<
          typename remove_accessors<
              typename deref_type<
                  typename remove_accessors<T>::type
              >::type
          >::type
      >
{ };

template<typename T>
using make_plain_t = typename make_plain<T>::type;

template<typename T>
struct function_traits
    : function_traits<decltype(&T::operator())>
{ };

template<typename R, typename... TArgs>
struct function_traits<R(*)(TArgs...)> {
    using result_type = R;
    using base_type = none_t;
    using args = type_list<TArgs...>;
};

template<typename R, typename T, typename... TArgs>
struct function_traits<R(T::*)(TArgs...)> {
    using result_type = R;
    using base_type = T;
    using args = type_list<TArgs...>;
};

template<typename R, typename T, typename... TArgs>
struct function_traits<R(T::*)(TArgs...) const> {
    using result_type = R;
    using base_type = T;
    using args = type_list<TArgs...>;
};

template<typename T, typename... Args>
decltype(void(T{std::declval<Args>()...}), std::true_type()) test(int);

template<typename, typename...>
std::false_type test(...);

template<typename T, typename... Args>
struct is_braces_constructible : decltype(test<T, Args...>(0))
{ };




    template<typename T>
    class has_configure
    {
        struct base_impl { void configure() { } };
        struct base
            : base_impl
            , mpl::if_<is_class<T>, T, mpl::void_>::type
        { base() { } };

        template<typename U>
        static mpl::aux::no_tag test(
            U*
          , non_type<void (base_impl::*)(), &U::configure>* = 0
        );

        template<typename>
        static mpl::aux::yes_tag test(...);

    public:
        typedef has_configure type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<base>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    #if !defined(BOOST_DI_INJECTOR)
        #define BOOST_DI_INJECTOR boost_di_injector__
    #endif

    template<typename T>
    class BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)
    {
        struct base_impl { void BOOST_DI_INJECTOR(...) { } };
        struct base
            : base_impl
            , mpl::if_<is_class<T>, T, mpl::void_>::type
        { base() { } };

        template<typename U>
        static mpl::aux::no_tag test(
            U*
          , non_type<void (base_impl::*)(...), &U::BOOST_DI_INJECTOR>* = 0
        );

        static mpl::aux::yes_tag test(...);

    public:
        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test((base*)0)) == sizeof(mpl::aux::yes_tag)
        );

        typedef bool_<value> type;
    };

    template<typename T>
    class has_call_operator
    {
        struct base_impl { void operator()(...) { } };
        struct base
            : base_impl
            , mpl::if_<is_class<T>, T, mpl::void_>::type
        { base() { } };

        template<typename U>
        static mpl::aux::no_tag test(
            U*
          , non_type<void (base_impl::*)(...), &U::operator()>* = 0
        );

        static mpl::aux::yes_tag test(...);

    public:
        typedef has_call_operator type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test((base*)0)) == sizeof(mpl::aux::yes_tag)
        );
    };

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
          , value = sizeof(test((base*)0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename C, typename U>
    decltype(std::declval<C>().operator()(std::declval<U>()), std::true_type{}) test_has_call_operator(void*);

    template<typename, typename>
    std::false_type test_has_call_operator(...);

    BOOST_MPL_HAS_XXX_TRAIT_DEF(result_type)

    template<typename T, typename U>
    using has_lambda = bool_<decltype(test_has_call_operator<T, U>(0))::value && !has_result_type<T>::value>;

    namespace detail {

    template<
        typename T
      , typename TSignature
    >
    class is_convertible
    {
        template<typename U>
        static mpl::aux::yes_tag test(non_type<TSignature, &U::operator()>*);

        template<typename>
        static mpl::aux::no_tag test(...);

    public:
        typedef is_convertible type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    } // namespace detail

    template<
        typename TValueType
      , typename T
    >
    struct is_convertible_to_ref
        : detail::is_convertible<TValueType, const T&(TValueType::*)(const boost::type<const T&>&) const>
    { };

    template<typename T, typename U = mpl::_1>
    struct is_same_base_of
        : mpl::or_<
              is_base_of<U, T>
            , is_same<U, T>
          >
    { };


    template<typename, typename = void>
    class has_call;

    template<typename T>
    class has_call<T, void>
    {
        struct base_impl { void call() { } };
        struct base : T, base_impl { base() { } };

        template<typename U>
        static mpl::aux::no_tag test(
            U*
          , non_type<void (base_impl::*)(), &U::call>* = 0
        );

        template<typename>
        static mpl::aux::yes_tag test(...);

    public:
        typedef has_call type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<base>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename T, typename TAction>
    class has_call
    {
        template<typename>
        struct void_ { };

        template<typename S, typename U>
        friend U& operator,(const U&, void_<S>);

        struct base : T
        {
            using T::call;
            mpl::aux::no_tag call(...) const;
        };

        template<typename, typename = void>
        struct base_call
            : mpl::false_
        { };

        template<typename TDummy>
        struct base_call<mpl::true_, TDummy>
            : is_same<
                  decltype(
                     ((((base*)0)->call(*(TAction*)0)), void_<T>())
                  )
                , void_<T>
              >
        { };

    public :
        typedef has_call type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = base_call<mpl::bool_<has_call<T>::value> >::value
        );
    };

} // namespace aux
} // namespace boost
} // namespace di

#endif

