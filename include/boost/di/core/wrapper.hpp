//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_WRAPPER_HPP
#define BOOST_DI_CORE_WRAPPER_HPP

#include "boost/di/aux_/compiler.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/concepts/creatable.hpp"

namespace core {

namespace successful {

template <class T, class TWrapper>
struct wrapper {
  inline operator T() noexcept { return wrapper_; }
  TWrapper wrapper_;
};

}  // namespace successful

template <class T, class TWrapper, class = int>
struct wrapper_impl {
  inline operator T() noexcept { return wrapper_; }
  TWrapper wrapper_;
};

template <class T, template <class...> class TWrapper, class TScope, class T_, class... Ts>
struct wrapper_impl<T, TWrapper<TScope, T_, Ts...>,
                    __BOOST_DI_REQUIRES(!aux::is_convertible<TWrapper<TScope, T_, Ts...>, T>::value)> {
  inline operator T() noexcept {
    return typename concepts::scoped<TScope, aux::remove_qualifiers_t<T_>>::template is_not_convertible_to<T>{};
  }
  TWrapper<TScope, T_, Ts...> wrapper_;
};

template <class T, class TWrapper>
using wrapper = wrapper_impl<T, TWrapper>;

}  // namespace core

#endif
