//
// Copyright (c) 2012-2018 Kanstantsin Chernik
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <memory>

#include "boost/di.hpp"

BOOST_DI_NAMESPACE_BEGIN
namespace extension {

//<<rebind only if expected and current type are same>>
template <class TExpected, class T>
struct injector_rebinder {
  template <class TInjector>
  auto rebind(TInjector& injector) {
    return std::move(injector);
  }
};

template <class T>
struct injector_rebinder<T, T> {
  template <class TInjector>
  auto rebind(TInjector& injector) {
    return make_injector(std::move(injector), bind<T>().in(unique)[override]);
  }
};

template <class T, class TFunc>
struct shared_factory_impl {
  shared_factory_impl(TFunc&& creation_func) : creation_func_(std::move(creation_func)), is_created_(false) {}

  shared_factory_impl(const shared_factory_impl& other)
      : creation_func_(std::move(other.creation_func_)), object_(other.object_), is_created_(other.is_created_) {}

  shared_factory_impl(shared_factory_impl&& other) noexcept
      : creation_func_(std::move(other.creation_func_)), object_(std::move(other.object_)), is_created_(other.is_created_) {}

  shared_factory_impl& operator=(shared_factory_impl&& other) noexcept {
    object_ = std::move(other.object_);
    return *this;
  }

  template <class TInjector, class TDependency>
  auto operator()(const TInjector& const_injector, const TDependency&) {
    if (!is_created_) {
      auto& injector = const_cast<TInjector&>(const_injector);
      auto rebound_injector = injector_rebinder<typename TDependency::expected, T>{}.rebind(injector);

      std::shared_ptr<T> object;
#ifdef __EXCEPTIONS
      try {
        object = creation_func_(rebound_injector);
      } catch (...) {
        injector = std::move(rebound_injector);
        throw;
      }
#else
      object = creation_func_(rebound_injector);
#endif
      injector = std::move(rebound_injector);

      //<<should be saved after move otherwise will be overriden>>
      object_ = object;
      is_created_ = true;
    }
    return object_;
  }

 private:
  TFunc&& creation_func_;
  std::shared_ptr<T> object_;
  bool is_created_;
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
