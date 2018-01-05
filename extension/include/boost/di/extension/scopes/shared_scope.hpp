//
// Copyright (c) 2012-2018 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <memory>

#include "boost/di.hpp"

//<<by default shared scope is synchronized>>
#if !defined(BOOST_DI_THREAD_SAFE_EXTENTIONS)
#define BOOST_DI_THREAD_SAFE_EXTENTIONS true
#endif

#if defined(BOOST_DI_THREAD_SAFE_EXTENTIONS)
#include <mutex>
#endif

BOOST_DI_NAMESPACE_BEGIN
namespace extension {

class shared_scope {
 public:
  template <class, class T>
  class scope {
   public:
    scope() noexcept = default;

#if defined(BOOST_DI_THREAD_SAFE_EXTENTIONS)
    //<<lock mutex so that move will be synchronized>>
    explicit scope(scope&& other) noexcept : scope(std::move(other), std::lock_guard<std::mutex>(other.mutex_)) {}
    //<<syncronized move constructor>>
    scope(scope&& other, const std::lock_guard<std::mutex>&) noexcept : object_(std::move(other.object_)) {}
#endif

    template <class T_>
    using is_referable = typename wrappers::shared<shared_scope, T>::template is_referable<T_>;

    template <class, class, class TProvider>
    static auto try_create(const TProvider& provider)
        -> decltype(wrappers::shared<shared_scope, T>{std::shared_ptr<T>{provider.get()}});

    template <class, class, class TProvider>
    auto create(const TProvider& provider) {
      if (!object_) {
#if defined(BOOST_DI_THREAD_SAFE_EXTENTIONS)
        std::lock_guard<std::mutex> lock(mutex_);
        if (!object_)
#endif
          object_ = std::shared_ptr<T>{provider.get()};
      }
      return wrappers::shared<shared_scope, T>{object_};
    }

   private:
#if defined(BOOST_DI_THREAD_SAFE_EXTENTIONS)
    std::mutex mutex_;
#endif
    std::shared_ptr<T> object_;
  };
};

static constexpr shared_scope shared{};

}  // extension
BOOST_DI_NAMESPACE_END
