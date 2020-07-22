//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_CALLABLE_HPP
#define BOOST_DI_CONCEPTS_CALLABLE_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/core/dependency.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/fwd.hpp"
#include "boost/di/scopes/deduce.hpp"

namespace concepts {

struct call_operator_with_one_argument {};

template <class>
struct policy {
  template <class>
  struct requires_ : aux::false_type {};
};

struct arg_wrapper {
  using type = _;
  using expected = _;
  using given = _;
  using name = no_name;
  using arity = aux::integral_constant<int, 0>;
  using scope = scopes::deduce;
  using is_root = aux::false_type;

  template <class, class, class>
  struct resolve;
};

aux::false_type callable_impl(...);

template <class T, class TArg>
auto callable_impl(const T&& t, TArg&& arg) -> aux::is_valid_expr<decltype(t(arg))>;

template <class...>
struct is_callable_impl;

template <class T, class... Ts>
struct is_callable_impl<T, Ts...> {
  using callable_with_arg = decltype(callable_impl(aux::declval<T>(), arg_wrapper{}));
  using type = aux::conditional_t<callable_with_arg::value, typename is_callable_impl<Ts...>::type,
                                  typename policy<T>::template requires_<call_operator_with_one_argument>>;
};

template <>
struct is_callable_impl<> : aux::true_type {};

template <class... Ts>
struct is_callable : is_callable_impl<Ts...> {};

template <class... Ts>
struct is_callable<core::pool<aux::type_list<Ts...>>> : is_callable_impl<Ts...> {};

template <>
struct is_callable<void> {  // auto
  using type = policy<void>::requires_<call_operator_with_one_argument>;
};

template <class... Ts>
using callable = typename is_callable<Ts...>::type;

}  // namespace concepts

#endif
