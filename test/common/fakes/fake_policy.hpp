//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_POLICY_HPP
#define BOOST_DI_FAKE_POLICY_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/fwd.hpp"

template<
    class T
  , class TDependency = aux::none_type
  , class TDeps = aux::none_type
  , bool  TResolve = true
> struct fake_policy;

template<
    class T
  , class TDependency
  , class TDeps
> struct fake_policy<T, TDependency, TDeps, true> {
    using type = T;
    using name = no_name;
    using dependency = TDependency;
    using is_root = std::false_type;

    template<class, class, class>
    using resolve = TDependency;
};

template<
    class T
  , class TDependency
  , class TDeps
> struct fake_policy<T, TDependency, TDeps, false> {
    using type = T;
    using name = no_name;
    using dependency = TDependency;
    using is_root = std::false_type;

    template<class, class, class TDefault>
    using resolve = TDefault;
};

#endif

