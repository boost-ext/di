//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
// // Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_BINDER_HPP
#define BOOST_DI_CORE_BINDER_HPP

#include "boost/di/aux_/mpl.hpp"
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
struct data {
    using type = T;
    using call_stack = TCallStack;
    using dependency = TDependency;
};

template<typename T, typename TCallStack, typename _>
using resolve_impl = pair<_, typename _::bind::template apply<data<T, TCallStack, _>>::type>;

template<typename T, typename TCallStack, typename _>
using eval_impl = typename _::bind::template eval<data<T, TCallStack, _>>::type;

template<typename>
struct binder;

template<typename... Ts>
struct binder<type_list<Ts...>> {
    template<
        typename T
      , typename TCallStack
      , typename TDefault =
            bindings::dependency<
                scopes::deduce
              , typename type_traits::make_plain<T>::type
            >
    >
    using resolve = typename greatest<
        pair<TDefault, int_<0>>
      , resolve_impl<T, TCallStack, Ts>...
    >::type::template rebind<typename scopes::deduce::rebind<T>::other>::other;

    template<
        typename T
      , typename TCallStack
    >
    using eval = typename or_<eval_impl<T, TCallStack, Ts>...>::type;
};

} // namespace core
} // namespace di
} // namespace boost

#endif

