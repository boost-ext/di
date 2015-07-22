//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_COPYABLE_HPP
#define BOOST_DI_CORE_COPYABLE_HPP

#include "boost/di/aux_/type_traits.hpp"

namespace boost { namespace di { inline namespace v1 { namespace core {

template<class>
struct copyable;

template<class T>
struct copyable_impl : std::conditional<
    std::is_default_constructible<typename T::creator>::value
  , aux::type_list<>
  , aux::type_list<T>
> { };

template<class... TDeps>
struct copyable<aux::type_list<TDeps...>>
    : aux::join<typename copyable_impl<TDeps>::type...>
{ };

template<class TDeps>
using copyable_t = typename copyable<TDeps>::type;

}}}} // boost::di::v1::core

#endif

