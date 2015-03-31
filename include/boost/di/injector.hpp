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

template<bool... Ts>
using all = std::is_same<aux::bool_list<(Ts, true)...>, aux::bool_list<Ts...>>;

template<class... TDeps>
class injector
    : public REQUIRES<
          concepts::boundable<aux::type<TDeps...>>()
        , decltype(concepts::boundable_error<aux::type<TDeps...>>())
        , core::injector<::BOOST_DI_CFG, TDeps...>> {
public:
    template<class...>
    BOOST_DI_ATTR_ERROR("creatable constraint not satisfied")
    void t1(){}

    template<
        class TConfig
      , class... TArgs
      , REQUIRES<all<concepts::creatable_<core::injector<TConfig, TArgs...>, TDeps*>()...>{}> = 0
    > injector(const core::injector<TConfig, TArgs...>& injector) noexcept // non explicit
        : core::injector<::BOOST_DI_CFG, TDeps...>{injector}
    { }

    template<
        class TConfig
      , class... TArgs
      , REQUIRES<!all<concepts::creatable_<core::injector<TConfig, TArgs...>, TDeps*>()...>{}> = 0
    > injector(const core::injector<TConfig, TArgs...>& injector) noexcept // non explicit
        : core::injector<::BOOST_DI_CFG, TDeps...>{injector}
    {
        t1<TArgs...>();
    }
};

}} // boost::di

#endif

