//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
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

template <class T>
struct unique {
  template <class I, BOOST_DI_REQUIRES(aux::is_convertible<T, I>::value) = 0>
  inline operator I() const noexcept {
    return object;
  }

  inline operator T &&() noexcept { return static_cast<T&&>(object); }

  T object;
};

template <class T>
struct unique<T*> {
#if defined(_MSC_VER)  // __pph__
  explicit unique(T* object) : object(object) {}
#endif  // __pph__

  template <class I, BOOST_DI_REQUIRES(aux::is_convertible<T, I>::value) = 0>
  inline operator I() const noexcept {
    struct scoped_ptr {
      aux::owner<T*> ptr;
      ~scoped_ptr() noexcept { delete ptr; }
    };
    return *scoped_ptr{object}.ptr;
  }

  template <class I, BOOST_DI_REQUIRES(aux::is_convertible<T*, I*>::value) = 0>
  inline operator aux::owner<I*>() const noexcept {
    return object;
  }

  template <class I, BOOST_DI_REQUIRES(aux::is_convertible<T*, const I*>::value) = 0>
  inline operator aux::owner<const I*>() const noexcept {
    return object;
  }

  template <class I, BOOST_DI_REQUIRES(aux::is_convertible<T*, I*>::value) = 0>
  inline operator std::shared_ptr<I>() const noexcept {
    return std::shared_ptr<I>{object};
  }

  template <class I, BOOST_DI_REQUIRES(aux::is_convertible<T*, I*>::value) = 0>
  inline operator boost::shared_ptr<I>() const noexcept {
    return boost::shared_ptr<I>{object};
  }

  template <class I, class D, BOOST_DI_REQUIRES(aux::is_convertible<T*, I*>::value) = 0>
  inline operator std::unique_ptr<I, D>() const noexcept {
    return std::unique_ptr<I, D>{object};
  }

  T* object = nullptr;
};

}  // wrappers

#endif
