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

template<typename T, typename TName, typename TCallStack>
struct get_bind
    : bindings::detail::requires_<
          bindings::type_traits::is_required_priority
        , bindings::type_traits::is_required_type<T>
        , bindings::type_traits::is_required_name<TName>
        , bindings::detail::when_<TCallStack>
      >
{ };

template<typename T, typename TName>
struct get_bind<T, TName, aux::mpl::vector0<>>
    : bindings::detail::requires_<
          bindings::type_traits::is_required_priority
        , bindings::type_traits::is_required_type<T>
        , bindings::type_traits::is_required_name<TName>
      >
{ };

template<typename T, typename TCallStack>
struct get_bind<T, no_name, TCallStack>
    : bindings::detail::requires_<
          bindings::type_traits::is_required_priority
        , bindings::type_traits::is_required_type<T>
        , bindings::detail::when_<TCallStack>
      >
{ };

template<typename T>
struct get_bind<T, no_name, aux::mpl::vector0<>>
    : bindings::detail::requires_<
          bindings::type_traits::is_required_priority
        , bindings::type_traits::is_required_type<T>
      >
{ };

template<
    typename TScope
  , typename TExpected
  , typename TGiven = TExpected
  , typename TName = no_name
  , typename TContext = aux::mpl::vector0<>
  , typename TBind = typename get_bind<TExpected, TName, TContext>::type
>
struct fake_dependency
{
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
    struct rebind
    {
        typedef fake_dependency<
            TScope
          , typename aux::mpl::if_<is_same<Given, void>, TExpected, Expected>::type
          , typename aux::mpl::if_<is_same<Given, void>, TGiven, Given>::type
        > other;
    };
};

} // namespace di
} // namespace boost

#endif

