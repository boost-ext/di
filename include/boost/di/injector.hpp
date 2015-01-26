//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_INJECTOR_HPP
#define BOOST_DI_INJECTOR_HPP

#include "boost/di/core/injector.hpp"
#include "boost/di/config.hpp"

namespace boost { namespace di {

template<class TConfig, class... TDeps>
class injector_ : public core::injector<TConfig, TDeps...> {
    using self = core::injector<TConfig, TDeps...>;

public:
    using deps = typename self::deps;

    template<class... TArgs>
    explicit injector_(const TArgs&... args) noexcept
        : self{core::pass_arg(args)...}
    { }

    template<
        class TConfig_
      , class... TDeps_
      //, BOOST_DI_REQUIRES(
            //concepts::creatable<typename core::injector<TConfig_, TDeps_...>::deps, TConfig, TDeps...>()
        //)
    > injector_(const core::injector<TConfig_, TDeps_...>& injector) noexcept // non explicit
        : self{injector}
    { }

    template<class T, BOOST_DI_REQUIRES(concepts::creatable<deps, TConfig, T>())>
    T create() const {
        return self::template create_impl<T>();
    }

    template<class TAction>
    void call(const TAction& action) {
        self::call_impl(action, deps{});
    }
};

template<class... TDeps>
using injector = injector_<config, TDeps...>;

}} // boost::di

#endif

