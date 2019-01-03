//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_PROVIDER_HPP
#define BOOST_DI_FAKE_PROVIDER_HPP

#include "boost/di/type_traits/memory_traits.hpp"
#include "common/fakes/fake_config.hpp"
#include "common/fakes/fake_injector.hpp"

template <class T = aux::none_type, class TInjector = fake_injector<T>, class TConfig = fake_config<>>
struct fake_provider {
  using injector = TInjector;
  using config = TConfig;

  static auto& provide_calls() {
    static int calls = 0;
    return calls;
  }

  auto get(const type_traits::heap& = {}) const noexcept {
    ++provide_calls();
    return new T{};
  }

  auto get(const type_traits::stack&) const noexcept {
    ++provide_calls();
    return T{};
  }

  auto& super() const { return *injector_; }
  auto& cfg() const { return injector_->cfg(); }

  TInjector* injector_ = nullptr;
};

#endif
