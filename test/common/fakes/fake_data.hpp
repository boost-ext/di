//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_DATA_HPP
#define BOOST_DI_FAKE_DATA_HPP

#include "boost/di/aux_/utility.hpp"

namespace boost { namespace di {

template<
    class T
  , class TDependency = aux::none_t
  , class TDeps = aux::none_t
  , bool TResolve = true
> struct fake_data;

template<
    class T
  , class TDependency
  , class TDeps
> struct fake_data<T, TDependency, TDeps, true> {
    using type = T;

    TDependency dep;
    TDeps deps;

    template<class, class TDefault>
    decltype(auto) resolve() const noexcept {
        return TDependency{};
    }
};

template<
    class T
  , class TDependency
  , class TDeps
> struct fake_data<T, TDependency, TDeps, false> {
    using type = T;

    TDependency dep;
    TDeps deps;

    template<class, class TDefault>
    decltype(auto) resolve() const noexcept {
        return TDefault{};
    }
};

}} // boost::di

#endif

