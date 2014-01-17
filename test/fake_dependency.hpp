//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_DEPENDENCY_HPP
#define BOOST_DI_FAKE_DEPENDENCY_HPP

#include "boost/di/concepts/dependency.hpp"

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/lambda.hpp>

namespace boost {
namespace di {

template<
    typename TScope
  , typename TExpected
  , typename TGiven = TExpected
  , typename TName = void
  , typename TContext0 = mpl::na
  , typename TContext1 = mpl::na
  , typename TContext2 = mpl::na
  , typename TBind = typename mpl::lambda<
        mpl::and_<
            is_same<TExpected, mpl::_1>
          , is_same<TName, mpl::_2>
        >
    >::type
>
struct fake_dependency
{
    typedef TBind bind;
    typedef TExpected expected;
    typedef TGiven given;
    typedef TName name;
    typedef TScope scope;
    typedef mpl::vector<TContext0, TContext1, TContext2> context;

    typedef typename concepts::dependency<
        TScope
      , TExpected
      , TGiven
      , TName
      , typename mpl::if_<mpl::empty<context>, mpl::vector0<>, context>::type
      , TBind
    > type;

    template<
        typename Expected = void
      , typename Given = void
      , typename Context = void
    >
    struct rebind
    {
        typedef fake_dependency<
            TScope
          , typename mpl::if_<is_same<Given, void>, TExpected, Expected>::type
          , typename mpl::if_<is_same<Given, void>, TGiven, Given>::type
          , TName
          , TContext0
          , TContext1
          , TContext2
        > other;
    };
};

} // namespace di
} // namespace boost

#endif

