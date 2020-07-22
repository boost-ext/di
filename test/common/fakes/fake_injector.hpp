//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_INJECTOR_HPP
#define BOOST_DI_FAKE_INJECTOR_HPP

#include <type_traits>
#include "boost/di/aux_/utility.hpp"
#include "boost/di/fwd.hpp"
#include "common/fakes/fake_config.hpp"
#include "common/fakes/fake_pool.hpp"

template <class... TDeps>
struct fake_injector : core::injector_base {
  using deps = fake_pool<TDeps...>;
  using config = fake_config<>;

  template <class T>
  auto create() const noexcept {
    return create_impl(aux::type<T>{});
  }

 protected:
  template <class, class = no_name, class = std::false_type>
  struct is_creatable : std::true_type {};

  template <class, class>
  struct try_create {
    using type = void;
  };

  template <class T>
  auto create_impl(const aux::type<T>&) const noexcept {
    return T{};
  }

  template <class TName, class T>
  auto create_impl(const aux::type<named<TName, T>>&) const noexcept {
    return T{};
  }

  template <class T>
  auto create_successful_impl(const aux::type<T>&) const noexcept {
    return T{};
  }

  template <class TName, class T>
  auto create_successful_impl(const aux::type<named<TName, T>>&) const noexcept {
    return T{};
  }

  config& cfg() { return config_; }

 private:
  config config_;
};

#endif
