//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_HPP
#define BOOST_DI_CONCEPTS_HPP

#include "boost/di/aux_/preprocessed/concepts.hpp"
#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/preprocessed/concepts/bind.hpp"
#include "boost/di/aux_/preprocessed/concepts/dependency.hpp"
#include "boost/di/aux_/preprocessed/concepts/scope.hpp"
#include "boost/di/aux_/preprocessed/scopes/deduce.hpp"
#include "boost/di/aux_/preprocessed/scopes/session.hpp"
#include "boost/di/aux_/preprocessed/scopes/shared.hpp"
#include "boost/di/aux_/preprocessed/scopes/unique.hpp"
#include <string>
#include <boost/mpl/int.hpp>

namespace boost {
namespace di {

template< typename T0 = ::boost::mpl::na , typename T1 = ::boost::mpl::na , typename T2 = ::boost::mpl::na , typename T3 = ::boost::mpl::na , typename T4 = ::boost::mpl::na , typename T5 = ::boost::mpl::na , typename T6 = ::boost::mpl::na , typename T7 = ::boost::mpl::na , typename T8 = ::boost::mpl::na , typename T9 = ::boost::mpl::na , typename T10 = ::boost::mpl::na , typename T11 = ::boost::mpl::na , typename T12 = ::boost::mpl::na , typename T13 = ::boost::mpl::na , typename T14 = ::boost::mpl::na , typename T15 = ::boost::mpl::na , typename T16 = ::boost::mpl::na , typename T17 = ::boost::mpl::na , typename T18 = ::boost::mpl::na , typename T19 = ::boost::mpl::na >
struct call_stack
    : mpl::vector< T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19>
{ };

template<typename TScope>
struct scope
    : concepts::scope<TScope, concepts::dependency>
{ };

template< typename T0 = ::boost::mpl::na , typename T1 = ::boost::mpl::na , typename T2 = ::boost::mpl::na , typename T3 = ::boost::mpl::na , typename T4 = ::boost::mpl::na , typename T5 = ::boost::mpl::na , typename T6 = ::boost::mpl::na , typename T7 = ::boost::mpl::na , typename T8 = ::boost::mpl::na , typename T9 = ::boost::mpl::na , typename T10 = ::boost::mpl::na , typename T11 = ::boost::mpl::na , typename T12 = ::boost::mpl::na , typename T13 = ::boost::mpl::na , typename T14 = ::boost::mpl::na , typename T15 = ::boost::mpl::na , typename T16 = ::boost::mpl::na , typename T17 = ::boost::mpl::na , typename T18 = ::boost::mpl::na , typename T19 = ::boost::mpl::na >
struct deduce
    : scope<scopes::deduce>::bind< T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19>
{ };

template< typename T0 = ::boost::mpl::na , typename T1 = ::boost::mpl::na , typename T2 = ::boost::mpl::na , typename T3 = ::boost::mpl::na , typename T4 = ::boost::mpl::na , typename T5 = ::boost::mpl::na , typename T6 = ::boost::mpl::na , typename T7 = ::boost::mpl::na , typename T8 = ::boost::mpl::na , typename T9 = ::boost::mpl::na , typename T10 = ::boost::mpl::na , typename T11 = ::boost::mpl::na , typename T12 = ::boost::mpl::na , typename T13 = ::boost::mpl::na , typename T14 = ::boost::mpl::na , typename T15 = ::boost::mpl::na , typename T16 = ::boost::mpl::na , typename T17 = ::boost::mpl::na , typename T18 = ::boost::mpl::na , typename T19 = ::boost::mpl::na >
struct unique
    : scope<scopes::unique<> >::bind< T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19>
{ };

template< typename T0 = ::boost::mpl::na , typename T1 = ::boost::mpl::na , typename T2 = ::boost::mpl::na , typename T3 = ::boost::mpl::na , typename T4 = ::boost::mpl::na , typename T5 = ::boost::mpl::na , typename T6 = ::boost::mpl::na , typename T7 = ::boost::mpl::na , typename T8 = ::boost::mpl::na , typename T9 = ::boost::mpl::na , typename T10 = ::boost::mpl::na , typename T11 = ::boost::mpl::na , typename T12 = ::boost::mpl::na , typename T13 = ::boost::mpl::na , typename T14 = ::boost::mpl::na , typename T15 = ::boost::mpl::na , typename T16 = ::boost::mpl::na , typename T17 = ::boost::mpl::na , typename T18 = ::boost::mpl::na , typename T19 = ::boost::mpl::na >
struct shared
    : scope<scopes::shared<> >::bind< T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19>
{ };

template< typename T0 = ::boost::mpl::na , typename T1 = ::boost::mpl::na , typename T2 = ::boost::mpl::na , typename T3 = ::boost::mpl::na , typename T4 = ::boost::mpl::na , typename T5 = ::boost::mpl::na , typename T6 = ::boost::mpl::na , typename T7 = ::boost::mpl::na , typename T8 = ::boost::mpl::na , typename T9 = ::boost::mpl::na , typename T10 = ::boost::mpl::na , typename T11 = ::boost::mpl::na , typename T12 = ::boost::mpl::na , typename T13 = ::boost::mpl::na , typename T14 = ::boost::mpl::na , typename T15 = ::boost::mpl::na , typename T16 = ::boost::mpl::na , typename T17 = ::boost::mpl::na , typename T18 = ::boost::mpl::na , typename T19 = ::boost::mpl::na >
struct session
    : scope<scopes::session<> >::bind< T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19>
{ };

template<typename TExpected, typename TGiven = TExpected>
struct bind
    : concepts::bind<TExpected, TGiven, concepts::dependency>
{ };

template<int N>
struct bind_int
    : bind<int, mpl::int_<N> >
{ };

template<typename T>
struct bind_string
    : bind<std::string, T>
{ };

}
}

#endif

