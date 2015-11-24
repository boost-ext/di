//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_PROVIDER_HPP
#define BOOST_DI_FAKE_PROVIDER_HPP

#include "boost/di/type_traits/memory_traits.hpp"
#include "common/fakes/fake_injector.hpp"

template <class T = aux::none_type>
struct fake_provider {
  auto get(const type_traits::heap& = {}) const noexcept {
    ++provide_calls();
    return new T{};
  }

  auto get(const type_traits::stack&) const noexcept {
    ++provide_calls();
    return T{};
  }

  static auto& provide_calls() {
    static int calls = 0;
    return calls;
  }

  fake_injector<T>* injector_ = nullptr;
  using injector_t = fake_injector<T>;
};

#endif
