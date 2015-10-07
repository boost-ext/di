//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_TYPE_TRAITS_HPP
#define BOOST_DI_AUX_TYPE_TRAITS_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/fwd.hpp"

#define BOOST_DI_HAS_TYPE(name, call_name)                                          \
    template<class, class = int>                                                    \
    struct name : ::boost::di::aux::false_type { };                                 \
                                                                                    \
    template<class T>                                                               \
    struct name<T, typename ::boost::di::aux::valid_t<typename T::call_name>::type> \
        : ::boost::di::aux::true_type                                               \
    { }

#define BOOST_DI_HAS_METHOD(name, call_name)                                        \
    template<class T, class... TArgs>                                               \
    decltype(::boost::di::aux::declval<T>().call_name(                              \
             ::boost::di::aux::declval<TArgs>()...)                                 \
           , ::boost::di::aux::true_type())                                         \
    name##_impl(int);                                                               \
                                                                                    \
    template<class, class...>                                                       \
    ::boost::di::aux::false_type name##_impl(...);                                  \
                                                                                    \
    template<class T, class... TArgs>                                               \
    struct name : decltype(name##_impl<T, TArgs...>(0)) { }

#define BOOST_DI_REQUIRES(...) typename ::boost::di::aux::enable_if<__VA_ARGS__, int>::type // __pph__
#define BOOST_DI_REQUIRES_MSG(...) typename ::boost::di::aux::concept_check<__VA_ARGS__>::type // __pph__

namespace boost { namespace di { inline namespace v1 { namespace aux {

template<bool B, class T, class F>
struct conditional { using type = T; };

template<class T, class F>
struct conditional<false, T, F> { using type = F; };

template<bool B, class T, class F>
using conditional_t = typename conditional<B, T, F>::type;

template<class T, T V>
struct integral_constant {
    static constexpr T value = V;
    using type = integral_constant;
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template<class T> struct add_rvalue_reference { using type = T&&;};
template<> struct add_rvalue_reference<void> {typedef void type;};
template<> struct add_rvalue_reference<const void> {typedef const void type;};
template<> struct add_rvalue_reference<volatile void> {typedef volatile void type;};
template<> struct add_rvalue_reference<const volatile void> {typedef const volatile void type;};

template<class T> using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

template<class T>
typename add_rvalue_reference<T>::type declval();

template<bool B, class T = void>
struct enable_if { };

template<class T>
struct enable_if<true, T> { using type = T; };

template< bool B, class T = void >
using enable_if_t = typename enable_if<B,T>::type;

template<class T> struct remove_const          { typedef T type; };
template<class T> struct remove_const<const T> { typedef T type; };
template<class T>
using remove_const_t = typename remove_const<T>::type;

template<class T> struct remove_volatile             { typedef T type; };
template<class T> struct remove_volatile<volatile T> { typedef T type; };
template<class T>
using remove_volatile_t = typename remove_volatile<T>::type;

template<class T>
struct remove_cv {
    typedef typename remove_volatile<typename remove_const<T>::type>::type type;
};

template< class T > struct remove_pointer                    {typedef T type;};
template< class T > struct remove_pointer<T*>                {typedef T type;};
template< class T > struct remove_pointer<T* const>          {typedef T type;};
template< class T > struct remove_pointer<T* volatile>       {typedef T type;};
template< class T > struct remove_pointer<T* const volatile> {typedef T type;};

template< class T >
using remove_pointer_t = typename remove_pointer<T>::type;

template< class T > struct remove_reference      {typedef T type;};
template< class T > struct remove_reference<T&>  {typedef T type;};
template< class T > struct remove_reference<T&&> {typedef T type;};
template< class T >
using remove_reference_t = typename remove_reference<T>::type;

template<class T, class U>
struct is_same : false_type {};

template<class T>
struct is_same<T, T> : true_type {};

template<class T>
struct is_abstract : integral_constant<bool, __is_abstract(T)> { };

template<class T>
struct is_polymorphic : integral_constant<bool, __is_polymorphic(T)> { };

template<class T>
struct is_class : integral_constant<bool, __is_class(T)> { };

#if defined(__clang__) || defined(_MSC_VER) // __pph__
    template<class T, class U>
    struct is_convertible : integral_constant<bool, __is_convertible_to(T, U)> { };
#else // __pph__
  template<typename _From, typename _To>
    class __is_convertible_helper {
       template<typename _To1> static void __test_aux(_To1);

      template<typename _From1, typename _To1,
      typename = decltype(__test_aux<_To1>(declval<_From1>()))>
      static true_type __test(int);

      template<typename, typename> static false_type __test(...);

    public:
      typedef decltype(__test<_From, _To>(0)) type;
    };

  template<typename _From, typename _To>
    struct is_convertible : public __is_convertible_helper<_From, _To>::type
    { };
#endif // __pph__

template<class T, class U>
struct is_base_of : integral_constant<bool, __is_base_of(T, U)> { };

template<class T>
struct concept_check { static_assert(T::value, "constraint not satisfied"); };

template<>
struct concept_check<true_type> {
    using type = int;
};

template<class...>
using is_valid_expr = true_type;

template<class T, class... TArgs>
decltype(void(T(declval<TArgs>()...)), true_type{}) test_is_constructible(int);

template<class, class...>
false_type test_is_constructible(...);

template<class T, class... TArgs>
struct is_constructible : decltype(test_is_constructible<T, TArgs...>(0)) { };

template<class T, class... TArgs>
using is_constructible_t = typename is_constructible<T, TArgs...>::type;

template<class T>
struct is_copy_constructible : is_constructible<T, const T&> { };

template<class T>
struct is_default_constructible : is_constructible<T> { };

template<class T, class... TArgs>
decltype(void(T{declval<TArgs>()...}), true_type{}) test_is_braces_constructible(int);

template<class, class...>
false_type test_is_braces_constructible(...);

template<class T, class... TArgs>
using is_braces_constructible = decltype(test_is_braces_constructible<T, TArgs...>(0));

template<class T, class... TArgs>
using is_braces_constructible_t = typename is_braces_constructible<T, TArgs...>::type;

template<class T>
using remove_specifiers =
    remove_cv<remove_pointer_t<remove_reference_t<T>>>;

template<class T>
using remove_specifiers_t = typename remove_specifiers<T>::type;

template<class TSrc, class TDst, class U = remove_specifiers_t<TDst>>
using is_narrowed = integral_constant<bool,
    !is_class<TSrc>::value && !is_class<U>::value && !is_same<TSrc, U>::value
>;

template<class T>
struct deref_type {
    using type = T;
};

template<class T, class TDeleter>
struct deref_type<std::unique_ptr<T, TDeleter>> {
    using type = remove_specifiers_t<typename deref_type<T>::type>;
};

template<class T>
struct deref_type<std::shared_ptr<T>> {
    using type = remove_specifiers_t<typename deref_type<T>::type>;
};

template<class T>
struct deref_type<boost::shared_ptr<T>> {
    using type = remove_specifiers_t<typename deref_type<T>::type>;
};

template<class T>
struct deref_type<std::weak_ptr<T>> {
    using type = remove_specifiers_t<typename deref_type<T>::type>;
};

template<class T, class TAllocator>
struct deref_type<std::vector<T, TAllocator>> {
    using type = core::array<remove_specifiers_t<typename deref_type<T>::type>*[]>;
};

template<class TKey, class TCompare, class TAllocator>
struct deref_type<std::set<TKey, TCompare, TAllocator>> {
    using type = core::array<remove_specifiers_t<typename deref_type<TKey>::type>*[]>;
};

template<class T>
using decay_t = typename deref_type<remove_specifiers_t<T>>::type;

template<class T1, class T2>
struct is_same_or_base_of {
    static constexpr auto value =
        is_same<decay_t<T1>, decay_t<T2>>::value ||
        is_base_of<decay_t<T2>, decay_t<T1>>::value;
};

template<class>
struct function_traits;

template<class R, class... TArgs>
struct function_traits<R(*)(TArgs...)> {
    using result_type = R;
    using args = type_list<TArgs...>;
};

template<class R, class... TArgs>
struct function_traits<R(TArgs...)> {
    using result_type = R;
    using args = type_list<TArgs...>;
};

template<class R, class T, class... TArgs>
struct function_traits<R(T::*)(TArgs...)> {
    using result_type = R;
    using args = type_list<TArgs...>;
};

template<class R, class T, class... TArgs>
struct function_traits<R(T::*)(TArgs...) const> {
    using result_type = R;
    using args = type_list<TArgs...>;
};

template<class T>
using function_traits_t = typename function_traits<T>::args;

BOOST_DI_HAS_METHOD(is_callable_with, operator());

template<class, class...>
struct is_array : false_type { };

template<class T, class... Ts>
struct is_array<T[], Ts...> : true_type { };

template<class T, class = decltype(sizeof(T))>
true_type is_complete_impl(int);

template<class T>
false_type is_complete_impl(...);

template<class T>
struct is_complete : decltype(is_complete_impl<T>(0)) { };

template<class, class...>
struct is_unique_impl;

template<class...>
struct not_unique : false_type {
    using type = not_unique;
};

template<>
struct not_unique<> : true_type {
    using type = not_unique;
};

template<class T>
struct is_unique_impl<T> : not_unique<> { };

template<class T1, class T2, class... Ts>
struct is_unique_impl<T1, T2, Ts...>
    : conditional_t<
          is_base_of<type<T2>, T1>::value
        , not_unique<T2>
        , is_unique_impl<inherit<T1, type<T2>>, Ts...>
      >
{ };

template<class... Ts>
using is_unique = is_unique_impl<none_type, Ts...>;

}}}} // boost::di::v1::aux

#endif

