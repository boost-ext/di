//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_BINDINGS_HPP
#define BOOST_DI_BINDINGS_HPP

#include "boost/di/aux_/mpl.hpp"
#include "boost/di/bindings/bind.hpp"
#include "boost/di/bindings/context.hpp"
#include "boost/di/bindings/scope.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/scopes/session.hpp"
#include "boost/di/scopes/shared.hpp"
#include "boost/di/scopes/unique.hpp"

namespace boost {
namespace di {

template<typename TExpected, typename TGiven = TExpected>
using bind = bindings::bind<TExpected, TGiven>;

template<typename TScope>
using scope = bindings::scope<TScope>;

template<typename... Ts>
using deduce = scope<scopes::deduce>::bind<Ts...>;

template<typename... Ts>
using unique = scope<scopes::unique>::bind<Ts...>;

template<typename... Ts>
using shared = scope<scopes::shared>::bind<Ts...>;

template<typename T, typename... Ts>
using session = scope<scopes::session<T>>::bind<Ts...>;

template<typename... Ts>
using context = bindings::context<Ts...>;

template<typename... Ts>
using any_of = type_list<Ts...>;

} // namespace di
} // namespace boost

#endif

