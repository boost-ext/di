//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_PROVIDABLE_HPP
#define BOOST_DI_CONCEPTS_PROVIDABLE_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/memory_traits.hpp"

namespace concepts {

struct get {};
struct is_creatable {};

template <class>
struct provider {
  template <class...>
  struct requires_ : aux::false_type {};
};

template <class T>
typename provider<T>::template requires_<get, is_creatable> providable_impl(...);

template <class T>
auto providable_impl(T&& t)
    -> aux::is_valid_expr<decltype(t.template get<_>(type_traits::direct{}, type_traits::heap{})),
                          decltype(t.template get<_>(type_traits::direct{}, type_traits::heap{}, int{})),
                          decltype(t.template get<_>(type_traits::uniform{}, type_traits::stack{})),
                          decltype(t.template get<_>(type_traits::uniform{}, type_traits::stack{}, int{})),
                          decltype(T::template is_creatable<type_traits::direct, type_traits::heap, _>::value),
                          decltype(T::template is_creatable<type_traits::uniform, type_traits::stack, _, int>::value)>;

template <class T>
struct providable__ {
  using type = decltype(providable_impl<T>(aux::declval<T>()));
};

template <class T>
using providable = typename providable__<T>::type;

}  // namespace concepts

#endif
