//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_MAKE_INJECTOR_HPP
#define BOOST_DI_MAKE_INJECTOR_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/injector.hpp"
#include "boost/di/concepts/configurable.hpp"
#include "boost/di/config.hpp"

namespace boost { namespace di {

template<class TConfig = ::BOOST_DI_CFG
       , BOOST_DI_REQUIRES(decltype(concepts::configurable(std::declval<TConfig>())){})
       , class... TArgs
> inline auto make_injector(const TArgs&... args) noexcept {
    return detail::injector<TConfig, TArgs...>(args...);
}

}} // boost::di

#endif

