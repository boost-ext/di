//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_REF_HPP
#define BOOST_DI_AUX_REF_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/mpl.hpp"
#include <functional>

#if !defined(BOOST_DI_CFG_NO_BOOST)
    #include <boost/ref.hpp>
#endif

namespace boost {
namespace di {
namespace aux {

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
using remove_accessors = std::remove_cv<
   typename std::remove_pointer<
       typename std::remove_reference<T>::type
   >::type
>;

template<typename>
struct make_plain;

BOOST_DI_HAS_MEMBER_TYPE(element_type);
BOOST_DI_HAS_MEMBER_TYPE(named_type);

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
struct function_traits;
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

} // namespace aux
} // namespace boost
} // namespace di

#endif

