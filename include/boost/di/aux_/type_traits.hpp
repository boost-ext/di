//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_TYPE_TRAITS_HPP
#define BOOST_DI_AUX_TYPE_TRAITS_HPP

#include <type_traits>
#include "boost/di/fwd.hpp"
#include "boost/di/aux_/utility.hpp"

#define BOOST_DI_HAS_TYPE(name, call_name)                          \
    template<class, class = void>                                   \
    struct name : std::false_type { };                              \
                                                                    \
    template<class T>                                               \
    struct name<                                                    \
        T, typename aux::void_t<typename T::call_name>::type        \
    > : std::true_type { }

#define BOOST_DI_HAS_METHOD(name, call_name)                        \
    template<class T, class... TArgs>                               \
    decltype(std::declval<T>().call_name(std::declval<TArgs>()...)  \
           , std::true_type())                                      \
    name##_impl(int);                                               \
                                                                    \
    template<class, class...>                                       \
    std::false_type name##_impl(...);                               \
                                                                    \
    template<class T, class... TArgs>                               \
    struct name : decltype(name##_impl<T, TArgs...>(0)) { }

#define BOOST_DI_REQUIRES__(...) ::std::enable_if_t<__VA_ARGS__>
#define BOOST_DI_REQUIRES(...) ::std::enable_if_t<__VA_ARGS__, int> = 0 // __pph__
#define BOOST_DI_REQUIRES_TYPE(...) typename ::boost::di::aux::concept_check<__VA_ARGS__>::type = 0 // __pph__

namespace boost { namespace di { inline namespace v1 { namespace aux {

template<class T>
struct concept_check { static_assert(T::value, "constraint not satisfied"); };

template<>
struct concept_check<std::true_type> {
    using type = int;
};

template<class...>
using is_valid_expr = std::true_type;

template<class T, class... TArgs>
decltype(void(T{std::declval<TArgs>()...}), std::true_type{})
test_is_braces_constructible(int);

template<class, class...>
std::false_type test_is_braces_constructible(...);

template<class T, class... TArgs>
using is_braces_constructible =
    decltype(test_is_braces_constructible<T, TArgs...>(0));

template<class T, class... TArgs>
using is_braces_constructible_t =
    typename is_braces_constructible<T, TArgs...>::type;

template<class T>
using remove_specifiers =
    std::remove_cv<std::remove_pointer_t<std::remove_reference_t<T>>>;

template<class T>
using remove_specifiers_t = typename remove_specifiers<T>::type;

template<class T>
struct deref_type {
    using type = T;
};

template<class T, class TDeleter>
struct deref_type<std::unique_ptr<T, TDeleter>> {
    using type = T;
};

template<class T>
struct deref_type<std::shared_ptr<T>> {
    using type = T;
};

template<class T>
struct deref_type<boost::shared_ptr<T>> {
    using type = T;
};

template<class T>
struct deref_type<std::weak_ptr<T>> {
    using type = T;
};

template<typename T>
using deref_type_t = typename deref_type<T>::type;

template<class T>
using decay = deref_type<remove_specifiers_t<deref_type_t<remove_specifiers_t<T>>>>;

template<class T>
using decay_t = typename decay<T>::type;

template<class T1, class T2>
struct is_same_or_base_of {
    static constexpr auto value =
        std::is_same<aux::decay_t<T1>, aux::decay_t<T2>>::value ||
        std::is_base_of<aux::decay_t<T2>, aux::decay_t<T1>>::value;
};

template<class>
struct function_traits;

template<class R, class... TArgs>
struct function_traits<R(*)(TArgs...)> {
    using result_type = R;
    using base_type = none_type;
    using args = type_list<TArgs...>;
};

template<class R, class... TArgs>
struct function_traits<R(TArgs...)> {
    using result_type = R;
    using base_type = none_type;
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
using function_traits_t = typename function_traits<T>::args;

BOOST_DI_HAS_METHOD(is_callable, operator());

}}}} // boost::di::v1::aux

#endif

