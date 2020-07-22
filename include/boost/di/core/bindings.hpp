//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_BINDINGS_HPP
#define BOOST_DI_CORE_BINDINGS_HPP

#include "boost/di/aux_/compiler.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/fwd.hpp"

namespace core {

template <class T, class = typename aux::is_a<injector_base, T>::type>
struct bindings_impl;

template <class T>
struct bindings_impl<T, aux::true_type> {
  using type = typename T::deps;
};

template <class T>
struct bindings_impl<T, aux::false_type> {
  using type = aux::type_list<T>;
};

#if defined(__MSVC__)  // __pph__
template <class... Ts>
struct bindings : aux::join_t<typename bindings_impl<Ts>::type...> {};

template <class... Ts>
using bindings_t = typename bindings<Ts...>::type;
#else   // __pph__
template <class... Ts>
using bindings_t = aux::join_t<typename bindings_impl<Ts>::type...>;
#endif  // __pph__

}  // namespace core

#endif
