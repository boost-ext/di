//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_REBIND_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_REBIND_TRAITS_HPP

#include "boost/di/fwd.hpp"

namespace type_traits {

template <class, class T>
struct rebind_traits {
  using type = T;
};

template <class T, class TName, class _>
struct rebind_traits<T, named<TName, _>> {
  using type = named<TName, T>;
};

template <class T, class D, class U>
struct rebind_traits<std::unique_ptr<T, D>, U> {
  using type = std::unique_ptr<U, D>;
};

template <class T, class D, class TName, class _>
struct rebind_traits<std::unique_ptr<T, D>, named<TName, _>> {
  using type = named<TName, std::unique_ptr<T, D>>;
};

template <class T, class U>
struct rebind_traits<std::shared_ptr<T>, U> {
  using type = std::shared_ptr<U>;
};

template <class T, class TName, class _>
struct rebind_traits<std::shared_ptr<T>, named<TName, _>> {
  using type = named<TName, std::shared_ptr<T>>;
};

template <class T, class U>
struct rebind_traits<std::weak_ptr<T>, U> {
  using type = std::weak_ptr<U>;
};

template <class T, class TName, class _>
struct rebind_traits<std::weak_ptr<T>, named<TName, _>> {
  using type = named<TName, std::weak_ptr<T>>;
};

template <class T, class U>
struct rebind_traits<boost::shared_ptr<T>, U> {
  using type = boost::shared_ptr<U>;
};

template <class T, class TName, class _>
struct rebind_traits<boost::shared_ptr<T>, named<TName, _>> {
  using type = named<TName, boost::shared_ptr<T>>;
};

template <class T, class U>
using rebind_traits_t = typename rebind_traits<T, U>::type;

}  // namespace type_traits

#endif
