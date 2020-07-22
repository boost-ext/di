//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_ARRAY_HPP
#define BOOST_DI_CORE_ARRAY_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/fwd.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/named_traits.hpp"
#include "boost/di/type_traits/rebind_traits.hpp"

namespace core {

template <class T, class... Ts>
struct array_impl : _ {
  using boost_di_inject__ = aux::type_list<Ts...>;
  explicit array_impl(type_traits::remove_named_t<Ts>&&... args)
      : array{static_cast<type_traits::remove_named_t<Ts>&&>(args)...} {}
  T array[sizeof...(Ts)];
};

template <class T, class... Ts>
struct array<T(), Ts...> : T {
  using value_type = typename aux::identity<T>::type::value_type;
  using array_t = array_impl<value_type, type_traits::rebind_traits_t<value_type, Ts>...>;
  using boost_di_inject__ = aux::type_list<array_t&&>;

  template <__BOOST_DI_REQUIRES(
                aux::is_constructible<T, std::move_iterator<value_type*>, std::move_iterator<value_type*>>::value) = 0>
  explicit array(array_t&& a)
      : T(std::move_iterator<value_type*>(a.array), std::move_iterator<value_type*>(a.array + sizeof...(Ts))) {}
};

template <class T>
struct array<T()> : T {
  using boost_di_inject__ = aux::type_list<>;
};

template <class T>
struct array<T> {};

}  // namespace core

namespace type_traits {
template <class _, class T, class _1, class... Ts>
struct ctor_traits__<core::array<_, Ts...>, T, _1, aux::false_type>
    : type_traits::ctor_traits__<core::array<aux::remove_smart_ptr_t<aux::remove_qualifiers_t<T>>(), Ts...>> {};
}  // namespace type_traits

#endif
