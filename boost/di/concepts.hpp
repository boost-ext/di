//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_HPP
#define BOOST_DI_CONCEPTS_HPP

#include "boost/di/aux_/meta.hpp"
#include "boost/di/concepts/annotate.hpp"
#include "boost/di/concepts/bind.hpp"
#include "boost/di/concepts/call_stack.hpp"
#include "boost/di/concepts/externals.hpp"
#include "boost/di/concepts/scope.hpp"
#include "boost/di/scopes/per_request.hpp"
#include "boost/di/scopes/singleton.hpp"

namespace boost {
namespace di {

template<typename TScope>
struct scope
    : concepts::scope<TScope>
{ };

template<typename TExpected, typename TGiven = TExpected>
struct bind
    : concepts::bind<TExpected, TGiven>
{ };

template<typename TExpected, typename TGiven = TExpected>
struct singleton
    : scope<scopes::singleton<> >::bind<bind<TExpected, TGiven> >
{ };

template<typename T>
struct singleton<T, T>
    : scope<scopes::singleton<> >::bind<T>
{ };

template<typename TExpected, typename TGiven = TExpected>
struct per_request
    : scope<scopes::per_request<> >::bind<bind<TExpected, TGiven> >
{ };

template<typename T>
struct per_request<T, T>
    : scope<scopes::per_request<> >::bind<T>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct singletons
    : scope<scopes::singleton<> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct per_requests
    : scope<scopes::per_request<> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<typename T>
struct external
    : concepts::externals<T>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct externals
    : concepts::externals<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<typename T>
struct annotate
    : concepts::annotate<T>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct call_stack
    : concepts::call_stack<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

} // namespace di
} // namespace boost

#endif

