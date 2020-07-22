//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "boost/di.hpp"

BOOST_DI_NAMESPACE_BEGIN
namespace extension {

/*<<simple `lazy` implementation>>*/
template <class T>
class lazy {
  template <class TInjector>
  static auto create(const void *injector) {
    return ((TInjector *)injector)->template create<T>();
  }

 public:
  using boost_di_inject__ = inject<self<T>>;

  template <class TInjector>
  explicit lazy(const TInjector &i) noexcept : injector_((void *)&i), f(create<TInjector>) {}

  T get() const { return (*f)(injector_); }

 private:
  const void *injector_ = nullptr;
  T (*f)(const void *) = nullptr;
};

}  // namespace extension
BOOST_DI_NAMESPACE_END
