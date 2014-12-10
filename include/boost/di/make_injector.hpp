//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_MAKE_INJECTOR_HPP
#define BOOST_DI_MAKE_INJECTOR_HPP

#include "boost/di/injector.hpp"
#include "boost/di/config.hpp"

namespace boost { namespace di {

template<class TConfig = BOOST_DI_CFG, class... TArgs>
inline decltype(auto) make_injector(const TArgs&... args) noexcept {
    return injector_cfg<TConfig, TArgs...>(args...);
}

}} // boost::di

#endif

