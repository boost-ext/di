//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_CONFIGURABLE_HPP
#define BOOST_DI_CONCEPTS_CONFIGURABLE_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/concepts/callable.hpp"
#include "boost/di/concepts/providable.hpp"

namespace concepts {

template <class>
struct policies {};
struct providable_type {};
struct callable_type {};

template <class>
struct config {
  template <class...>
  struct requires_ : aux::false_type {};
};

template <class TConfig>
struct injector {
  using config = TConfig;
  using deps = aux::type_list<>;
  template <class T>
  T create() const;
};

aux::false_type configurable_impl(...);

template <class T>
auto configurable_impl(T &&) -> aux::is_valid_expr<decltype(aux::declval<T>().provider((injector<T>*)0)),
                                                   decltype(aux::declval<T>().policies((injector<T>*)0))>;

template <class T1, class T2>
struct get_configurable_error : aux::type_list<T1, T2> {};

template <class T>
struct get_configurable_error<aux::true_type, T> {
  using type = T;
};

template <class T>
struct get_configurable_error<T, aux::true_type> {
  using type = T;
};

template <>
struct get_configurable_error<aux::true_type, aux::true_type> : aux::true_type {};

template <class T>
auto is_configurable(const aux::true_type&) {
  return typename get_configurable_error<decltype(providable<decltype(aux::declval<T>().provider((injector<T>*)0))>()),
                                         decltype(callable<decltype(aux::declval<T>().policies((injector<T>*)0))>())>::type{};
}

template <class T>
auto is_configurable(const aux::false_type&) {
  return typename config<T>::template requires_<provider<providable_type(...)>, policies<callable_type(...)>>{};
}

template <class T>
struct configurable__ {
  using type = decltype(is_configurable<T>(decltype(configurable_impl(aux::declval<T>())){}));
};

template <class T>
using configurable = typename configurable__<T>::type;

}  // namespace concepts

#endif
