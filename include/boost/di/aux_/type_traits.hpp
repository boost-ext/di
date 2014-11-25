//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_CPP_14_TYPE_TRAITS_HPP
#define BOOST_DI_AUX_CPP_14_TYPE_TRAITS_HPP

#include "boost/di/aux_/utility.hpp"

#include <type_traits>
#include <boost/mpl/aux_/yes_no.hpp>

#define BOOST_DI_HAS_TYPE(name) BOOST_DI_HAS_TYPE_IMPL(name)

#define BOOST_DI_HAS_TYPE_IMPL(name)                                            \
    template<class  > std::false_type has_##name##_impl(...);                   \
    template<class T> std::true_type has_##name##_impl(typename T::name*);      \
    template<class T> struct has_##name : decltype(has_##name##_impl<T>(0)) { }

#define BOOST_DI_HAS_METHOD(name) BOOST_DI_HAS_METHOD_IMPL(name)

#define BOOST_DI_HAS_METHOD_IMPL(name)                                                                              \
    struct has_##name##_impl { void name() { } };                                                                   \
    template<class T> std::false_type test(T*, non_type<void (has_##name##_impl::*)(), &T::name>* = 0);             \
    template<class> std::true_type test(...);                                                                       \
    template<class T, typename = void> struct has_##name : decltype(test<inherit<T, has_##name##_impl>>(0)) { };    \
    template<class T> struct has_##name<T, std::enable_if_t<!std::is_class<T>::value>> : std::false_type { }        \

namespace boost {
namespace di {
namespace aux {

    struct void_ {};

template <bool b, class T=void>
  using enable_if_t       = typename std::enable_if<b,T>::type;  // C++14

template <bool b, class T, class F>
  using conditional_t     = typename std::conditional<b,T,F>::type;  // C++14

template<class T, class... TArgs>
using is_constructible_t = typename std::is_constructible<T, TArgs...>::type;

template<class T>
using remove_pointer_t = typename std::remove_pointer<T>::type;

template<class T>
using remove_reference_t = typename std::remove_reference<T>::type;

template<class T>
using remove_accessors = std::remove_cv<aux::remove_pointer_t<aux::remove_reference_t<T>>>;

template<class T>
using remove_accessors_t = typename remove_accessors<T>::type;

template<class>
struct make_plain;

BOOST_DI_HAS_TYPE(element_type);
BOOST_DI_HAS_TYPE(named_type);

template<class T, class = void>
struct deref_type {
    using type = T;
};

template<class T>
struct deref_type<T, typename std::enable_if<has_element_type<T>::value>::type> {
    using type = typename T::element_type;
};

template<class T>
struct deref_type<T, typename std::enable_if<has_named_type<T>::value>::type> {
    using type = typename make_plain<typename T::named_type>::type;
};

template<class T>
struct make_plain
    : deref_type<
          typename remove_accessors<
              typename deref_type<
                  typename remove_accessors<T>::type
              >::type
          >::type
      >
{ };

template<class T>
using make_plain_t = typename make_plain<T>::type;

template<class T>
struct function_traits
    : function_traits<decltype(&T::operator())>
{ };

template<class R, class... TArgs>
struct function_traits<R(*)(TArgs...)> {
    using result_type = R;
    using base_type = none_t;
    using args = type_list<TArgs...>;
};

template<class R, class T, class... TArgs>
struct function_traits<R(T::*)(TArgs...)> {
    using result_type = R;
    using base_type = T;
    using args = type_list<TArgs...>;
};

template<class R, class T, class... TArgs>
struct function_traits<R(T::*)(TArgs...) const> {
    using result_type = R;
    using base_type = T;
    using args = type_list<TArgs...>;
};

    template<class T>
    class has_call_operator
    {
        struct base_impl { void operator()(...) { } };
        struct base
            : base_impl
            , std::conditional<std::is_class<T>::value, T, void_>::type
        { base() { } };

        template<class U>
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

    template<class C, class U>
    decltype(std::declval<C>().operator()(std::declval<U>()), std::true_type{}) test_has_call_operator(void*);

    template<class, class>
    std::false_type test_has_call_operator(...);

    BOOST_DI_HAS_TYPE(result_type);

    template<class T, class U>
    using has_lambda = bool_<decltype(test_has_call_operator<T, U>(0))::value && !has_result_type<T>::value>;

    namespace detail {

    template<
        class T
      , class TSignature
    >
    class is_convertible
    {
        template<class U>
        static mpl::aux::yes_tag test(non_type<TSignature, &U::operator()>*);

        template<class>
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
        class TValueType
      , class T
    >
    struct is_convertible_to_ref
        : detail::is_convertible<TValueType, const T&(TValueType::*)(const type<const T&>&) const>
    { };

    template<class, class = void>
    class has_call;

    template<class T>
    class has_call<T, void>
    {
        struct base_impl { void call() { } };
        struct base : T, base_impl { base() { } };

        template<class U>
        static mpl::aux::no_tag test(
            U*
          , non_type<void (base_impl::*)(), &U::call>* = 0
        );

        template<class>
        static mpl::aux::yes_tag test(...);

    public:
        typedef has_call type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<base>(0)) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<class T, class TAction>
    class has_call
    {
        template<class>
        struct void_ { };

        template<class S, class U>
        friend U& operator,(const U&, void_<S>);

        struct base : T
        {
            using T::call;
            mpl::aux::no_tag call(...) const;
        };

        template<class, class = void>
        struct base_call
            : std::false_type
        { };

        template<class TDummy>
        struct base_call<std::true_type, TDummy>
            : std::is_same<
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
          , value = base_call<bool_<has_call<T>::value> >::value
        );
    };

} // namespace aux
} // namespace boost
} // namespace di

#endif

