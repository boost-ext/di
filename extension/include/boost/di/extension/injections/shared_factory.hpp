//
// Copyright (c) 2012-2018 Kanstantsin Chernik
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <memory>

#include "boost/di.hpp"
#include "extensible_injector.hpp"
#if BOOST_DI_THREAD_SAFE == true
#include <mutex>
#endif

BOOST_DI_NAMESPACE_BEGIN
namespace extension {

//<<rebind only if expected and current type are same>>
template <class TExpected, class T>
struct injector_rebinder {
  template <class TInjector>
  auto& rebind(TInjector& injector) {
    return injector;
  }
};

template <class T>
struct injector_rebinder<T, T> {
  template <class TInjector>
  auto rebind(TInjector& injector) {
    return make_injector(make_extensible(injector), bind<T>().in(unique)[override]);
  }
};

template <class T, class TFunc>
struct shared_factory_impl {
  shared_factory_impl(TFunc&& creation_func) : creation_func_(std::move(creation_func)), is_created_(false) {}

  shared_factory_impl(const shared_factory_impl& other)
      : creation_func_(std::move(other.creation_func_)), object_(other.object_), is_created_(other.is_created_) {}

#if BOOST_DI_THREAD_SAFE == true
  //<<lock mutex so that move will be synchronized>>
  explicit shared_factory_impl(shared_factory_impl&& other) noexcept
      : shared_factory_impl(std::move(other), std::lock_guard<std::mutex>(other.mutex_)) {}
  //<<syncronized move constructor>>
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
#if BOOST_DI_THREAD_SAFE == true
      std::lock_guard<std::mutex> lock(mutex_);
      if (!is_created_)
#endif
      {
        is_created_ = true;
        auto& injector = const_cast<TInjector&>(const_injector);
        const auto& rebound_injector = injector_rebinder<typename TDependency::expected, T>{}.rebind(injector);
        object_ = creation_func_(rebound_injector);
      }
    }
    return object_;
  }

 private:
  TFunc&& creation_func_;
  std::shared_ptr<T> object_;
  bool is_created_;
#if BOOST_DI_THREAD_SAFE == true
  std::mutex mutex_;
#endif
};

template <class T, class TFunc>
auto shared_factory(TFunc&& creation_func) {
  return shared_factory_impl<T, TFunc>(std::move(creation_func));
}

template <class T, class TFunc>
auto conditional_shared_factory(TFunc condition_func) {
  return shared_factory<T>([&](const auto& injector) {
    std::shared_ptr<T> object;
    if (condition_func()) {
      object = injector.template create<std::shared_ptr<T>>();
    }
    return object;
  });
}

}  // extension
BOOST_DI_NAMESPACE_END
