//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_MEMORY_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_MEMORY_TRAITS_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/fwd.hpp"

namespace type_traits {

struct stack {};
struct heap {};

template <class T, class = int>
struct memory_traits {
  using type = stack;
};

template <class T>
struct memory_traits<T*> {
  using type = heap;
};

template <class T>
struct memory_traits<const T&> {
  using type = typename memory_traits<T>::type;
};

template <class T, class TDeleter>
struct memory_traits<std::unique_ptr<T, TDeleter>> {
  using type = heap;
};

template <class T>
struct memory_traits<std::shared_ptr<T>> {
  using type = heap;
};

template <class T>
struct memory_traits<boost::shared_ptr<T>> {
  using type = heap;
};

template <class T>
struct memory_traits<std::weak_ptr<T>> {
  using type = heap;
};

template <class T>
struct memory_traits<T, __BOOST_DI_REQUIRES(aux::is_polymorphic<T>::value)> {
  using type = heap;
};

}  // namespace type_traits

#endif
