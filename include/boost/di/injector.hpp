//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_INJECTOR_HPP
#define BOOST_DI_INJECTOR_HPP

#include "boost/di/core/injector.hpp"
#include "boost/di/concepts/boundable.hpp"
#include "boost/di/config.hpp"

namespace boost { namespace di {

template<class... TDeps>
class injector
    : public BOOST_DI_REQUIRES_RET
        (concepts::boundable<core::bindings_t<TDeps...>>())
        (core::injector<::BOOST_DI_CFG, TDeps...>) {
public:
    template<
        class TConfig
      , class... TArgs
        BOOST_DI_REQUIRES(concepts::creatable<typename core::injector<TConfig, TArgs...>::deps, TConfig, TDeps...>())
    > injector(const core::injector<TConfig, TArgs...>& injector) noexcept // non explicit
        : core::injector<::BOOST_DI_CFG, TDeps...>{core::init{}, injector}
    { }
};

}} // boost::di

#endif

