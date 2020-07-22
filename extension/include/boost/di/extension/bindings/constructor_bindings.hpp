//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <type_traits>

#include "boost/di.hpp"

BOOST_DI_NAMESPACE_BEGIN
namespace extension {

/*<<to constructor extension>>*/
template <class... TCtor>
struct constructor_impl {
  template <class TInjector, class T,
            std::enable_if_t<concepts::creatable<type_traits::direct, typename T::expected, TCtor...>::value, int> = 0>
  auto operator()(const TInjector& injector, const T&) const {
    return new typename T::expected{injector.template create<TCtor>()...};
  }
};

template <class... TCtor>
struct constructor : constructor_impl<TCtor...> {};

}  // namespace extension
BOOST_DI_NAMESPACE_END
