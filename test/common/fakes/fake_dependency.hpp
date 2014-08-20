//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_DEPENDENCY_HPP
#define BOOST_DI_FAKE_DEPENDENCY_HPP

#include "boost/di/bindings/dependency.hpp"

#include "boost/di/named.hpp"
#include "boost/di/bindings/detail/requires.hpp"
#include "boost/di/bindings/detail/when.hpp"
#include "boost/di/bindings/type_traits/is_required_name.hpp"
#include "boost/di/bindings/type_traits/is_required_priority.hpp"
#include "boost/di/bindings/type_traits/is_required_type.hpp"

namespace boost {
namespace di {

template<typename, typename, typename>
struct get_bind;

template<typename T, typename TName, typename... Ts>
struct get_bind<T, TName, type_list<Ts...>>
    : bindings::detail::requires_<
          bindings::type_traits::is_required_priority
        , bindings::type_traits::is_required_type<T>
        , bindings::type_traits::is_required_name<TName>
        , bindings::detail::when_<Ts...>
      >
{ };

template<typename T, typename TName>
struct get_bind<T, TName, type_list<>>
    : bindings::detail::requires_<
          bindings::type_traits::is_required_priority
        , bindings::type_traits::is_required_type<T>
        , bindings::type_traits::is_required_name<TName>
        , bindings::detail::when_<>
      >
{ };

template<typename T, typename... Ts>
struct get_bind<T, no_name, type_list<Ts...>>
    : bindings::detail::requires_<
          bindings::type_traits::is_required_priority
        , bindings::type_traits::is_required_type<T>
        , bindings::detail::when_<Ts...>
      >
{ };

template<typename T>
struct get_bind<T, no_name, type_list<>>
    : bindings::detail::requires_<
          bindings::type_traits::is_required_priority
        , bindings::type_traits::is_required_type<T>
        , bindings::detail::when_<>
      >
{ };

template<
    typename TScope
  , typename TExpected
  , typename TGiven = TExpected
  , typename TName = no_name
  , typename TContext = type_list<>
  , typename TBind = typename get_bind<TExpected, TName, TContext>::type
>
struct fake_dependency {
    typedef TScope scope;
    typedef TExpected expected;
    typedef TGiven given;
    typedef TBind bind;

    typedef typename bindings::dependency<
        TScope
      , TExpected
      , TGiven
      , TBind
    > type;

    template<
        typename Expected = void
      , typename Given = void
    >
    struct rebind {
        typedef fake_dependency<
            TScope
          , typename std::conditional<std::is_same<Given, void>::value, TExpected, Expected>::type
          , typename std::conditional<std::is_same<Given, void>::value, TGiven, Given>::type
        > other;
    };
};

} // namespace di
} // namespace boost

#endif

