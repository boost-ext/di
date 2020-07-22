//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or unique at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_WRAPPERS_UNIQUE_HPP
#define BOOST_DI_WRAPPERS_UNIQUE_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/fwd.hpp"

namespace wrappers {

template <class TScope, class T>
struct unique {
  using scope = TScope;

  template <class I, __BOOST_DI_REQUIRES(aux::is_convertible<T, I>::value) = 0>
  inline operator I() const noexcept {
    return object;
  }

  inline operator T &&() noexcept { return static_cast<T&&>(object); }

  T object;
};

template <class TScope, class T>
struct unique<TScope, T*> {
  using scope = TScope;

#if defined(__MSVC__)  // __pph__
  explicit unique(T* object) : object(object) {}
#endif  // __pph__

  template <class I, __BOOST_DI_REQUIRES(aux::is_convertible<T, I>::value) = 0>
  inline operator I() const noexcept {
    struct scoped_ptr {
      aux::owner<T*> ptr;
      ~scoped_ptr() noexcept { delete ptr; }
    };
    return static_cast<T&&>(*scoped_ptr{object}.ptr);
  }

  template <class I, __BOOST_DI_REQUIRES(aux::is_convertible<T*, I*>::value) = 0>
  inline operator aux::owner<I*>() const noexcept {
    return object;
  }

  template <class I, __BOOST_DI_REQUIRES(aux::is_convertible<T*, const I*>::value) = 0>
  inline operator aux::owner<const I*>() const noexcept {
    return object;
  }

  template <class I, __BOOST_DI_REQUIRES(aux::is_convertible<T*, I*>::value) = 0>
  inline operator std::shared_ptr<I>() const noexcept {
    return std::shared_ptr<I>{object};
  }

  template <class I, __BOOST_DI_REQUIRES(aux::is_convertible<T*, I*>::value) = 0>
  inline operator boost::shared_ptr<I>() const noexcept {
    return boost::shared_ptr<I>{object};
  }

  template <class I, class D, __BOOST_DI_REQUIRES(aux::is_convertible<T*, I*>::value) = 0>
  inline operator std::unique_ptr<I, D>() const noexcept {
    return std::unique_ptr<I, D>{object};
  }

  T* object = nullptr;
};

}  // namespace wrappers

#endif
