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
struct copyable_impl;

template<class T>
struct to_be_copied : std::conditional<
    std::is_default_constructible<typename T::creator>::value
  , aux::type_list<>
  , aux::type_list<T>
> { };

template<class... TDeps>
struct copyable_impl<aux::type_list<TDeps...>>
    : aux::join<typename to_be_copied<TDeps>::type...>
{ };

template<class TDeps>
using copyable = typename copyable_impl<TDeps>::type;

}}}} // boost::di::v1::core

#endif

