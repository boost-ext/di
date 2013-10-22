//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_HPP
#define BOOST_DI_CONCEPTS_HPP

#include "boost/di/aux_/meta.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/concepts/bind.hpp"
#include "boost/di/concepts/dependency.hpp"
#include "boost/di/concepts/scope.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/scopes/session.hpp"
#include "boost/di/scopes/shared.hpp"
#include "boost/di/scopes/singleton.hpp"
#include "boost/di/scopes/thread.hpp"
#include "boost/di/scopes/unique.hpp"

#include <memory>
#include <string>
#include <vector>
#include <boost/mpl/int.hpp>

namespace boost {
namespace di {

template<int N>
struct int_ : mpl::int_<N>
{
    operator int() const { return N; }
};

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct call_stack
    : mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<typename TScope>
struct scope
    : concepts::scope<TScope, concepts::dependency>
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
struct thread
    : scope<scopes::thread<scopes::deduce> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct shared
    : scope<scopes::shared<> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct singleton
    : scope<scopes::singleton<> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct session
    : scope<scopes::session<> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<typename TExpected, typename TGiven = TExpected>
struct bind
    : concepts::bind<TExpected, TGiven, concepts::dependency>
{ };

template<int N>
struct bind_int
    : bind<int, int_<N> >
{ };

template<typename T>
struct bind_string
    : bind<std::string, T>
{ };

} // namespace di
} // namespace boost

#endif

