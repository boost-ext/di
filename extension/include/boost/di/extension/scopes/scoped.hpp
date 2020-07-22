//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <memory>

#include "boost/di.hpp"

BOOST_DI_NAMESPACE_BEGIN
namespace extension {

namespace detail {
class scoped {
 public:
  template <class, class T>
  class scope {
   public:
    template <class T_, class>
    using is_referable = typename wrappers::shared<scoped, T>::template is_referable<T_>;

    scope &operator=(scope &&other) noexcept {
      this->object_ = other.object_;
      other.object_ = nullptr;
      return *this;
    }

    template <class, class, class TProvider, class T_ = aux::decay_t<decltype(aux::declval<TProvider>().get())>>
    static decltype(wrappers::shared<scoped, T_>{std::shared_ptr<T_>{std::shared_ptr<T_>{aux::declval<TProvider>().get()}}})
    try_create(const TProvider &);

    template <class T_, class, class TProvider>
    auto create(const TProvider &provider) {
      return create_impl<aux::decay_t<decltype(provider.get())>>(provider);
    }

    scope() = default;
    scope(scope &&other) noexcept : object_(other.object_) { other.object_ = nullptr; }
    ~scope() noexcept { delete object_; }

   private:
    template <class, class TProvider>
    auto create_impl(const TProvider &provider) {
      if (!object_) {
        object_ = new std::shared_ptr<T>{provider.get()};
      }
      return wrappers::shared<scoped, T, std::shared_ptr<T> &>{*object_};
    }

    std::shared_ptr<T> *object_ = nullptr;
  };
};
}  // namespace detail

static constexpr detail::scoped scoped{};

}  // namespace extension
BOOST_DI_NAMESPACE_END
