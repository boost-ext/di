//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_ARRAY_HPP
#define BOOST_DI_CORE_ARRAY_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/type_traits/rebind_traits.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/fwd.hpp"

namespace core {

template <class T, int N>
struct array_impl {
  T array_[N];
};

template <class C, class... Ts>
struct array<C(), Ts...> : array_impl<typename C::value_type, sizeof...(Ts)>, C {
  using value_type = typename C::value_type;
  using array_t = array_impl<value_type, sizeof...(Ts)>;
  using array_t::array_;
  using boost_di_inject__ = aux::type_list<type_traits::rebind_traits_t<value_type, Ts>...>;

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

  template <BOOST_DI_REQUIRES(
                aux::is_constructible<C, std::move_iterator<value_type*>, std::move_iterator<value_type*>>::value) = 0>
  explicit array(remove_named_t<type_traits::rebind_traits_t<value_type, Ts>>... args)
      : array_t{{static_cast<remove_named_t<type_traits::rebind_traits_t<value_type, Ts>>&&>(args)...}},
        C(std::move_iterator<value_type*>(array_), std::move_iterator<value_type*>(array_ + sizeof...(Ts))) {}
};

template <class C>
struct array<C()> : C {
  using boost_di_inject__ = aux::type_list<>;
};

}  // core

namespace type_traits {
template <class _, class T, class... Ts>
struct ctor_traits__<core::array<_, Ts...>, T, aux::false_type>
    : type_traits::ctor_traits__<core::array<aux::remove_smart_ptr_t<aux::remove_qualifiers_t<T>>(), Ts...>> {};
}  // type_traits

#endif
