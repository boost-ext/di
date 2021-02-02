//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <memory>

#include "boost/di.hpp"
#include "boost/di/extension/injections/extensible_injector.hpp"

BOOST_DI_NAMESPACE_BEGIN
namespace extension {

template <class T, class... TArgs>
struct ifactory {
  virtual ~ifactory() noexcept = default;
  virtual std::unique_ptr<T> create(TArgs&&...) const = 0;
};

template <class, class, class>
struct factory_impl;

template <class TInjector, class T, class I, class... TArgs>
struct factory_impl<TInjector, T, ifactory<I, TArgs...>> : ifactory<I, TArgs...> {
  explicit factory_impl(const TInjector& injector) : injector_(const_cast<TInjector&>(injector)) {}

  std::unique_ptr<I> create(TArgs&&... args) const override {
    // clang-format off
    auto injector = make_injector(
      make_extensible(injector_)
#if (__clang_major__ == 3) && (__clang_minor__ > 4) || defined(__GCC__) || defined(__MSVC__)
    , bind<TArgs>().to(std::forward<TArgs>(args))[override]...
#else // wknd for clang 3.4
    , core::dependency<scopes::instance, TArgs, TArgs, no_name, core::override>(std::forward<TArgs>(args))...
#endif
    );
    // clang-format on

    return injector.template create<std::unique_ptr<T>>();
  }

 private:
  TInjector& injector_;
};

template <class T>
struct factory {
  template <class TInjector, class TDependency>
  auto operator()(const TInjector& injector, const TDependency&) const
      -> std::shared_ptr<factory_impl<TInjector, T, typename TDependency::expected>> {
    static auto sp = std::make_shared<factory_impl<TInjector, T, typename TDependency::expected>>(injector);
    return sp;
  }
};

}  // namespace extension
BOOST_DI_NAMESPACE_END
