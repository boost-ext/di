//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_MAKE_INJECTOR_HPP
#define BOOST_DI_MAKE_INJECTOR_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/concepts/configurable.hpp"
#include "boost/di/concepts/boundable.hpp"
#include "boost/di/core/injector.hpp"
#include "boost/di/config.hpp"

namespace boost { namespace di {

template<
     template<class> class TConfig = ::BOOST_DI_CFG
   , class... TDeps
   , REQUIRES<concepts::configurable<TConfig<_>>() &&
              concepts::boundable<core::bindings_t<TDeps...>>()
            , errors<decltype(concepts::boundable_error<core::bindings_t<TDeps...>>())
                   , decltype(concepts::configurable_error<TConfig<_>>())
              >
     > = 0
> inline auto make_injector(const TDeps&... args) noexcept {
    return core::injector<TConfig, TDeps...>{core::init{}, args...};
}

}} // boost::di

#endif

