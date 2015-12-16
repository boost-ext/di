//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_MAKE_INJECTOR_HPP
#define BOOST_DI_MAKE_INJECTOR_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/concepts/configurable.hpp"
#include "boost/di/concepts/boundable.hpp"
#include "boost/di/core/injector.hpp"
#include "boost/di/config.hpp"

namespace detail {
auto make_injector = [](auto injector) {
  using injector_t = decltype(injector);
  struct i : injector_t {
    using injector_t::injector_t;
  };
  return i{static_cast<injector_t&&>(injector)};
};
}  // detail

template <class TConfig = BOOST_DI_CFG, class... TDeps,
          BOOST_DI_REQUIRES_MSG(concepts::boundable<aux::type_list<TDeps...>>) = 0,
          BOOST_DI_REQUIRES_MSG(concepts::configurable<TConfig>) = 0>
inline auto make_injector(TDeps... args) noexcept {
  return detail::make_injector(core::injector<TConfig, decltype(((TConfig*)0)->policies(0)), TDeps...>{
      core::init{}, static_cast<TDeps&&>(args)...});
}
#endif
