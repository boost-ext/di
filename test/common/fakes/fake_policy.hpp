//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_fake_policy_HPP
#define BOOST_DI_fake_policy_HPP

#include "boost/di/aux_/utility.hpp"

namespace boost { namespace di {

template<
    class T
  , class TDependency = aux::none_t
  , class TDeps = aux::none_t
  , bool TResolve = true
> struct fake_policy;

template<
    class T
  , class TDependency
  , class TDeps
> struct fake_policy<T, TDependency, TDeps, true> {
    struct arg {
        using type = T;
    };

    using dependency = TDependency;

    template<class TT, class Name, class TDefault>
    using resolve = TDependency;
};

template<
    class T
  , class TDependency
  , class TDeps
> struct fake_policy<T, TDependency, TDeps, false> {
    struct arg {
        using type = T;
    };

    using dependency = TDependency;

    template<class TT, class Name, class TDefault>
    using resolve = TDefault;
};

}} // boost::di

#endif

