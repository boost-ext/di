//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_DEPENDENCY_HPP
#define BOOST_DI_FAKE_DEPENDENCY_HPP

#include "boost/di/concepts/dependency.hpp"

#include "boost/di/concepts/type_traits/is_req_type.hpp"
#include "boost/di/concepts/type_traits/is_req_name.hpp"
#include "boost/di/concepts/type_traits/is_req_call.hpp"
#include "boost/di/concepts/type_traits/priority.hpp"

#include <boost/mpl/vector.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/lambda.hpp>

namespace boost {
namespace di {

struct no_name { };

template<typename T, typename TName, typename TCallStack>
struct get_bind
    : mpl::lambda<
         mpl::times<
             concepts::type_traits::priority<>
           , concepts::type_traits::is_req_type<T>
           , concepts::type_traits::is_req_name<TName>
           , concepts::type_traits::is_req_call<TCallStack>
         >
      >
{ };

template<typename T, typename TName>
struct get_bind<T, TName, mpl::vector0<>>
    : mpl::lambda<
          mpl::times<
              concepts::type_traits::priority<>
            , concepts::type_traits::is_req_type<T>
            , concepts::type_traits::is_req_name<TName>
          >
      >
{ };

template<typename T, typename TCallStack>
struct get_bind<T, no_name, TCallStack>
    : mpl::lambda<
          mpl::times<
              concepts::type_traits::priority<>
            , concepts::type_traits::is_req_type<T>
            , concepts::type_traits::is_req_call<TCallStack>
          >
      >
{ };

template<typename T>
struct get_bind<T, no_name, mpl::vector0<>>
    : mpl::lambda<
          mpl::times<
              concepts::type_traits::priority<>
            , concepts::type_traits::is_req_type<T>
          >
      >
{ };

template<
    typename TScope
  , typename TExpected
  , typename TGiven = TExpected
  , typename TName = no_name
  , typename TContext = mpl::vector0<>
  , typename TBind = typename get_bind<TExpected, TName, TContext>::type
>
struct fake_dependency
{
    typedef TScope scope;
    typedef TExpected expected;
    typedef TGiven given;
    typedef TBind bind;

    typedef typename concepts::dependency<
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
          , typename mpl::if_<is_same<Given, void>, TExpected, Expected>::type
          , typename mpl::if_<is_same<Given, void>, TGiven, Given>::type
        > other;
    };
};

} // namespace di
} // namespace boost

#endif

