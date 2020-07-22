//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_NAMED_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_NAMED_TRAITS_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/fwd.hpp"

namespace type_traits {

template <class T>
struct remove_named {
  using type = T;
};

template <class TName, class T>
struct remove_named<named<TName, T>> {
  using type = T;
};

template <class T>
using remove_named_t = typename remove_named<T>::type;

template <class T>
struct add_named {
  using type = named<no_name, T>;
};

template <class TName, class T>
struct add_named<named<TName, T>> {
  using type = named<TName, T>;
};

template <class T>
using add_named_t = typename add_named<T>::type;

template <class T>
struct named_decay {
  using type = aux::decay_t<T>;
};

template <class TName, class T>
struct named_decay<named<TName, T>> {
  using type = named<TName, aux::decay_t<T>>;
};

template <class T>
using named_decay_t = typename named_decay<T>::type;

}  // namespace type_traits

#endif
