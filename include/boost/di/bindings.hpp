//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_BINDINGS_HPP
#define BOOST_DI_BINDINGS_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/bindings/bind.hpp"
#include "boost/di/bindings/call_stack.hpp"
#include "boost/di/bindings/dependency.hpp"
#include "boost/di/bindings/scope.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/scopes/session.hpp"
#include "boost/di/scopes/shared.hpp"
#include "boost/di/scopes/unique.hpp"

#include <string>

namespace boost {
namespace di {

template<typename TExpected, typename TGiven = TExpected>
struct bind
    : bindings::bind<TExpected, TGiven, bindings::dependency>
{ };

template<bool V>
struct bind_bool
    : bind<bool, aux::mpl::bool_<V> >
{ };

template<int N>
struct bind_int
    : bind<int, aux::mpl::int_<N> >
{ };

template<typename T>
struct bind_string
    : bind<std::string, T>
{ };

template<typename TScope>
struct scope
    : bindings::scope<TScope, bindings::dependency>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct deduce
    : scope<scopes::deduce>::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct unique
    : scope<scopes::unique<> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct shared
    : scope<scopes::shared<> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct session
    : scope<scopes::session<> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct call_stack
    : bindings::call_stack<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct any_of
    : BOOST_DI_MPL_VECTOR_TYPES_PASS_MPL(T)
{ };

} // namespace di
} // namespace boost

#endif

