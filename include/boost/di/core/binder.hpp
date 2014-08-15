//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
// // Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_BINDER_HPP
#define BOOST_DI_CORE_BINDER_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/type_traits/make_plain.hpp"
#include "boost/di/bindings/dependency.hpp"
#include "boost/di/scopes/deduce.hpp"

namespace boost {
namespace di {
namespace core {

template<
    typename T
  , typename TCallStack
  , typename TDependency
>
struct data
{
    typedef T type;
    typedef TCallStack call_stack;
    typedef TDependency dependency;
};

template<typename T, typename TCallStack, typename _>
using calculate = pair<_, typename _::bind::template apply<data<T, TCallStack, _>>::type>;

template<typename>
class binder;

template<typename... Ts>
class binder<type_list<Ts...>>
{
public:
    template<
        typename T
      , typename TCallStack
      , typename TDefault =
            bindings::dependency<
                scopes::deduce
              , typename type_traits::make_plain<T>::type
            >
    >
    using resolve = typename
        greatest<pair<TDefault, int_<0>>, calculate<T, TCallStack, Ts>...>::type
              ::template rebind<typename scopes::deduce::rebind<T>::other>::other;
};

} // namespace core
} // namespace di
} // namespace boost

#endif

