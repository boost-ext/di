//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_POLICY_HPP
#define BOOST_DI_FAKE_POLICY_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/fwd.hpp"

template <class, class TDependency = aux::none_type, class TDeps = aux::none_type, bool TResolve = true, bool IsRoot = false>
struct fake_policy;

template <class T, class TDependency, class TDeps, bool IsRoot>
struct fake_policy<T, TDependency, TDeps, true, IsRoot> {
  using type = T;
  using name = no_name;
  using dependency = TDependency;
  using is_root = aux::integral_constant<bool, IsRoot>;

  template <class, class, class>
  using resolve = TDependency;
};

template <class T, class TDependency, class TDeps, bool IsRoot>
struct fake_policy<T, TDependency, TDeps, false, IsRoot> {
  using type = T;
  using name = no_name;
  using dependency = TDependency;
  using is_root = aux::integral_constant<bool, IsRoot>;

  template <class, class, class TDefault>
  using resolve = TDefault;
};

#endif
