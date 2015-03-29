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
    : public REQUIRES<
          concepts::boundable<aux::type<TDeps...>>()
        , decltype(concepts::boundable_error<aux::type<TDeps...>>())
        , core::injector<::BOOST_DI_CFG, TDeps...>> {
public:
    template<class TConfig, class... TArgs>
    injector(const core::injector<TConfig, TArgs...>& injector) noexcept // non explicit
        : core::injector<::BOOST_DI_CFG, TDeps...>{injector}
    { }
};

}} // boost::di

#endif

