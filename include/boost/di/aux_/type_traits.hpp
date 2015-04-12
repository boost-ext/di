//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_TYPE_TRAITS_HPP
#define BOOST_DI_AUX_TYPE_TRAITS_HPP

#define __has_include(...) 0

#include <memory>
#include <type_traits>
#include "boost/di/aux_/utility.hpp"
#if (__has_include(<boost/shared_ptr.hpp>))
    #include <boost/shared_ptr.hpp>
#endif

#define BOOST_DI_HAS_TYPE(name)                                     \
    template<class, class = void>                                   \
    struct has_##name : std::false_type { };                        \
                                                                    \
    template<class T>                                               \
    struct has_##name<T, typename aux::void_t<typename T::name>::type>             \
        : std::true_type                                            \
    { }

#define BOOST_DI_HAS_METHOD(name, call_name)                        \
    template<class T, class... TArgs>                               \
    decltype(std::declval<T>().call_name(std::declval<TArgs>()...)  \
           , std::true_type())                                      \
    has_##name##_impl(int);                                         \
                                                                    \
    template<class, class...>                                       \
    std::false_type has_##name##_impl(...);                         \
                                                                    \
    template<class T, class... TArgs>                               \
    struct has_##name : decltype(has_##name##_impl<T, TArgs...>(0)) {}

#if defined(__clang__)
    #define BOOST_DI_UNUSED __attribute__((unused))
    #define BOOST_DI_ATTR_ERROR(...) [[deprecated(__VA_ARGS__)]]
#elif defined(__GNUC__)
    #define BOOST_DI_UNUSED __attribute__((unused))
    #define BOOST_DI_ATTR_ERROR(...) __attribute__ ((error(__VA_ARGS__)))
#elif defined(_MSC_VER)
    #define BOOST_DI_UNUSED
    #define BOOST_DI_ATTR_ERROR(...) __declspec(deprecated(__VA_ARGS__))
#endif

#define BOOST_DI_REQUIRES(...) \
    typename std::enable_if<__VA_ARGS__, int>::type = 0

#define BOOST_DI_REQUIRES_T(...) \
    std::enable_if_t<__VA_ARGS__>

#define BOOST_DI_REQUIRES_MSG(...) \
    typename constraint_not_satisfied<__VA_ARGS__>::type = 0

#define BOOST_DI_REQUIRES_MSG_T(...) \
    constraint_not_satisfied<__VA_ARGS__>::type

namespace boost { namespace di {

template<class...>
struct constraint_not_satisfied { };

template<>
struct constraint_not_satisfied<std::true_type> {
    using type = int;
};

template<class T>
struct constraint_not_satisfied<std::true_type, T> {
    using type = T;
};

namespace aux {

template<class...>
using is_valid_expr = std::true_type;

template<class>
struct is_smart_ptr : std::false_type { };

template<class T, class TDeleter>
struct is_smart_ptr<std::unique_ptr<T, TDeleter>>
    : std::true_type
{ };

template<class T>
struct is_smart_ptr<std::shared_ptr<T>>
    : std::true_type
{ };

#if (__has_include(<boost/shared_ptr.hpp>))
    template<class T>
    struct is_smart_ptr<boost::shared_ptr<T>>
        : std::true_type
    { };
#endif

template<class T>
struct is_smart_ptr<std::weak_ptr<T>>
    : std::true_type
{ };

template<class T, class... TArgs>
decltype(void(T{std::declval<TArgs>()...}), std::true_type{})
test_is_braces_constructible(int);

template<class, class...>
std::false_type test_is_braces_constructible(...);

template<class T, class... TArgs>
struct is_braces_constructible :
    decltype(test_is_braces_constructible<T, TArgs...>(0)) {};

template<class T, class... TArgs>
using is_braces_constructible_t =
    typename is_braces_constructible<T, TArgs...>::type;

template<class T>
struct remove_accessors :
    std::remove_cv<std::remove_pointer_t<std::remove_reference_t<T>>>{};

template<class T>
using remove_accessors_t = typename remove_accessors<T>::type;

template<class, class = void>
struct deref_type;

template<typename T>
using deref_type_t = typename deref_type<T>::type;

template<class T, class>
struct deref_type {
    using type = T;
};

template<class T>
struct deref_type<T, std::enable_if_t<is_smart_ptr<T>::value>> {
    using type = typename T::element_type;
};

template<class T>
using decay =
    deref_type<remove_accessors_t<deref_type_t<remove_accessors_t<T>>>>;

template<class T>
using decay_t = typename decay<T>::type;

template<class T1, class T2>
struct is_same_or_base_of {
    static constexpr auto value =
        std::is_same<aux::decay_t<T1>, aux::decay_t<T2>>::value ||
        std::is_base_of<aux::decay_t<T2>, aux::decay_t<T1>>::value;
};

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

template<class R, class... TArgs>
struct function_traits<R(TArgs...)> {
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

}}} // boost::di::aux

#endif

