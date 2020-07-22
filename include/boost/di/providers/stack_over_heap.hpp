//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_PROVIDERS_STACK_OVER_HEAP_HPP
#define BOOST_DI_PROVIDERS_STACK_OVER_HEAP_HPP

#include "boost/di/concepts/creatable.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/memory_traits.hpp"

namespace providers {

class stack_over_heap {
 public:
  template <class TInitialization, class T, class... TArgs>
  struct is_creatable {
    static constexpr auto value = concepts::creatable<TInitialization, T, TArgs...>::value;
  };

  template <class T, class... TArgs>
  auto get(const type_traits::direct&, const type_traits::heap&, TArgs&&... args) const {
    return new T(static_cast<TArgs&&>(args)...);
  }

  template <class T, class... TArgs>
  auto get(const type_traits::uniform&, const type_traits::heap&, TArgs&&... args) const {
    return new T{static_cast<TArgs&&>(args)...};
  }

  template <class T, class... TArgs>
  auto get(const type_traits::direct&, const type_traits::stack&, TArgs&&... args) const {
    return T(static_cast<TArgs&&>(args)...);
  }

  template <class T, class... TArgs>
  auto get(const type_traits::uniform&, const type_traits::stack&, TArgs&&... args) const {
    return T{static_cast<TArgs&&>(args)...};
  }
};

}  // namespace providers

#endif
