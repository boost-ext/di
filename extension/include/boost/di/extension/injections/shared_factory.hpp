//
// Copyright (c) 2012-2020 Kanstantsin Chernik
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <memory>

#include "boost/di.hpp"
#include "boost/di/extension/injections/extensible_injector.hpp"
#if !defined(BOOST_DI_NOT_THREAD_SAFE)
#include <mutex>
#endif

BOOST_DI_NAMESPACE_BEGIN
namespace extension {

constexpr auto no_recursion = true;

template <bool NoRecursion>
struct injector_rebinder {
  template <class TDependency, class TInjector>
  auto& rebind(TInjector& injector) {
    return injector;
  }
};

template <>
struct injector_rebinder<no_recursion> {
  template <class TDependency, class TInjector>
  auto rebind(TInjector& injector) {
    return make_injector(make_extensible(injector), TDependency{});
  }
};

template <class T, bool NoRecursion, class TFunc>
struct shared_factory_impl {
  shared_factory_impl(TFunc&& creation_func) : creation_func_(std::move(creation_func)), is_created_(false) {}

  shared_factory_impl(const shared_factory_impl& other)
      : creation_func_(std::move(other.creation_func_)), object_(other.object_), is_created_(other.is_created_) {}

#if !defined(BOOST_DI_NOT_THREAD_SAFE)
  //<<lock mutex so that move will be synchronized>>
  explicit shared_factory_impl(shared_factory_impl&& other) noexcept
      : shared_factory_impl(std::move(other), std::lock_guard<std::mutex>(other.mutex_)) {}
  //<<synchronized move constructor>>
  shared_factory_impl(shared_factory_impl&& other, const std::lock_guard<std::mutex>&) noexcept
      : creation_func_(std::move(other.creation_func_)), object_(std::move(other.object_)), is_created_(other.is_created_) {}
#else
  shared_factory_impl(shared_factory_impl&& other) noexcept
      : creation_func_(std::move(other.creation_func_)), object_(std::move(other.object_)), is_created_(other.is_created_) {}
#endif
  shared_factory_impl& operator=(shared_factory_impl&& other) noexcept {
    object_ = std::move(other.object_);
    return *this;
  }

  template <class TInjector, class TDependency>
  auto operator()(const TInjector& const_injector, const TDependency&) {
    if (!is_created_) {
#if !defined(BOOST_DI_NOT_THREAD_SAFE)
      std::lock_guard<std::mutex> lock(mutex_);
      if (!is_created_)
#endif
      {
        is_created_ = true;

        using override_dep = core::dependency<scopes::unique, typename TDependency::expected, T, no_name, core::override>;
        //<<rebind to avoid recursion>>
        auto& orig_injector = const_cast<TInjector&>(const_injector);
        const auto& rebound_injector = injector_rebinder<NoRecursion>{}.template rebind<override_dep>(orig_injector);
        object_ = creation_func_(rebound_injector);
      }
    }
    return object_;
  }

 private:
  TFunc&& creation_func_;
  std::shared_ptr<T> object_;
  bool is_created_;
#if !defined(BOOST_DI_NOT_THREAD_SAFE)
  std::mutex mutex_;
#endif
};

template <class T, bool NoRecursion = false, class TFunc>
auto shared_factory(TFunc&& creation_func) {
  return shared_factory_impl<T, NoRecursion, TFunc>(std::move(creation_func));
}

template <class T, bool NoRecursion = false, class TFunc>
auto conditional_shared_factory(TFunc condition_func) {
  return shared_factory<T, NoRecursion>([&](const auto& injector) {
    std::shared_ptr<T> object;
    if (condition_func()) {
      object = injector.template create<std::shared_ptr<T>>();
    }
    return object;
  });
}

}  // namespace extension
BOOST_DI_NAMESPACE_END
