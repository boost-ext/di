//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_HPP
#define BOOST_DI_CONCEPTS_HPP

#include <string>
#include <boost/mpl/int.hpp>
#include "boost/di/concepts/annotate.hpp"
#include "boost/di/concepts/bind.hpp"
#include "boost/di/concepts/externals.hpp"
#include "boost/di/concepts/scope.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/scopes/per_request.hpp"
#include "boost/di/scopes/singleton.hpp"
#include "boost/di/scopes/session.hpp"
#include "boost/di/detail/dependency.hpp"
#include "boost/di/named.hpp"
#include "boost/di/config.hpp"

namespace boost {
namespace di {

template<
    typename TExpected
  , typename TGiven = TExpected
>
struct bind
    : concepts::bind<TExpected, TGiven, detail::dependency, named>
{ };

template<int N>
struct bind_int
    : bind<int, mpl::int_<N> >
{ };

template<typename T>
struct bind_string
    : bind<std::string, T>
{ };

template<typename TScope>
struct scope
    : concepts::scope<TScope, detail::dependency, named>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct deduce
    : scope<scopes::deduce>::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct per_request
    : scope<scopes::per_request<> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct singleton
    : scope<scopes::singleton<> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct session
    : scope<scopes::session<> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct external
    : concepts::externals<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<typename T>
struct annotate
    : concepts::annotate<T>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct call_stack
    : mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

} // namespace di
} // namespace boost

#endif

