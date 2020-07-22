//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/fwd.hpp"

#if !defined(BOOST_DI_CFG_CTOR_LIMIT_SIZE)  // __pph__
#define BOOST_DI_CFG_CTOR_LIMIT_SIZE 10     // __pph__
#endif                                      // __pph__

namespace type_traits {

__BOOST_DI_HAS_TYPE(is_injectable, boost_di_inject__);

struct direct {};
struct uniform {};

template <class T, int>
using get = T;

template <template <class...> class, class, class, class = int>
struct ctor_impl;

template <template <class...> class TIsConstructible, class T>
struct ctor_impl<TIsConstructible, T, aux::index_sequence<>> : aux::type_list<> {};

template <template <class...> class TIsConstructible, class T>
struct ctor_impl<TIsConstructible, T, aux::index_sequence<0>,
                 __BOOST_DI_REQUIRES(TIsConstructible<T, core::any_type_1st_fwd<T>>::value)>
    : aux::type_list<core::any_type_1st_fwd<T>> {};

template <template <class...> class TIsConstructible, class T>
struct ctor_impl<TIsConstructible, T, aux::index_sequence<0>,
                 __BOOST_DI_REQUIRES(!TIsConstructible<T, core::any_type_1st_fwd<T>>::value)>
    : aux::conditional_t<TIsConstructible<T, core::any_type_1st_ref_fwd<T>>::value,
                         aux::type_list<core::any_type_1st_ref_fwd<T>>, aux::type_list<>> {};

template <template <class...> class TIsConstructible, class T, int... Ns>
struct ctor_impl<TIsConstructible, T, aux::index_sequence<Ns...>,
                 __BOOST_DI_REQUIRES((sizeof...(Ns) > 1) && TIsConstructible<T, get<core::any_type_fwd<T>, Ns>...>::value)>
    : aux::type_list<get<core::any_type_fwd<T>, Ns>...> {};

template <template <class...> class TIsConstructible, class T, int... Ns>
struct ctor_impl<TIsConstructible, T, aux::index_sequence<Ns...>,
                 __BOOST_DI_REQUIRES((sizeof...(Ns) > 1) && !TIsConstructible<T, get<core::any_type_fwd<T>, Ns>...>::value)>
    : aux::conditional<TIsConstructible<T, get<core::any_type_ref_fwd<T>, Ns>...>::value,
                       aux::type_list<get<core::any_type_ref_fwd<T>, Ns>...>,
                       typename ctor_impl<TIsConstructible, T, aux::make_index_sequence<sizeof...(Ns) - 1>>::type> {};

template <template <class...> class TIsConstructible, class T>
using ctor_impl_t = typename ctor_impl<TIsConstructible, T, aux::make_index_sequence<BOOST_DI_CFG_CTOR_LIMIT_SIZE>>::type;

template <class...>
struct ctor;

template <class T>
struct ctor<T, aux::type_list<>> : aux::pair<uniform, ctor_impl_t<aux::is_braces_constructible, T>> {};

template <class T, class... TArgs>
struct ctor<T, aux::type_list<TArgs...>> : aux::pair<direct, aux::type_list<TArgs...>> {};

template <class T, class, class = typename is_injectable<ctor_traits<T>>::type>
struct ctor_traits_impl;

template <class T, class = void, class = void, class = typename is_injectable<T>::type>
struct ctor_traits__;

template <class T, class _1, class _2>
struct ctor_traits__<T, _1, _2, aux::true_type> : aux::pair<T, aux::pair<direct, typename T::boost_di_inject__::type>> {};

template <class T, class _1, class _2>
struct ctor_traits__<T, _1, _2, aux::false_type> : ctor_traits_impl<T, _1> {};

template <class T, class _1, class... Ts>
struct ctor_traits__<T, _1, core::pool_t<Ts...>, aux::false_type> : aux::pair<T, aux::pair<uniform, aux::type_list<Ts...>>> {};

template <class T, class _>
struct ctor_traits_impl<T, _, aux::true_type>
    : aux::pair<T, aux::pair<direct, typename ctor_traits<T>::boost_di_inject__::type>> {};

template <class T, class _>
struct ctor_traits_impl<T, _, aux::false_type> : aux::pair<T, typename ctor_traits<T>::type> {};

}  // namespace type_traits

template <class T, class>
struct ctor_traits : type_traits::ctor<T, type_traits::ctor_impl_t<aux::is_constructible, T>> {};

template <class T>
struct ctor_traits<std::initializer_list<T>> {
  using boost_di_inject__ = aux::type_list<>;
};

template <class... Ts>
struct ctor_traits<std::tuple<Ts...>> {
  using boost_di_inject__ = aux::type_list<Ts...>;
};

template <class T>
struct ctor_traits<T, __BOOST_DI_REQUIRES(aux::is_same<std::char_traits<char>, typename T::traits_type>::value)> {
  using boost_di_inject__ = aux::type_list<>;
};

template <class T>
struct ctor_traits<T, __BOOST_DI_REQUIRES(!aux::is_class<T>::value)> {
  using boost_di_inject__ = aux::type_list<>;
};

#endif
