//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_DEPENDENCY_HPP
#define BOOST_DI_FAKE_DEPENDENCY_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include "boost/di/aux_/dependency.hpp"

namespace boost {
namespace di {

template<
    typename TScope
  , typename TExpected
  , typename TGiven = TExpected
  , typename TContext0 = mpl::na
  , typename TContext1 = mpl::na
  , typename TContext2 = mpl::na
>
struct fake_dependency
{
    typedef mpl::vector<TContext0, TContext1, TContext2> context;
    typedef typename aux_::dependency<
        TScope
      , TExpected
      , TGiven
      , typename mpl::if_<mpl::empty<context>, mpl::vector0<>, context>::type
    > type;
};

template<
    typename TScope
  , typename TExpected
  , typename TGiven = TExpected
  , typename TContext0 = mpl::na
  , typename TContext1 = mpl::na
  , typename TContext2 = mpl::na
>
struct fake_dependency_base_of
{
    typedef mpl::vector<TContext0, TContext1, TContext2> context;
    typedef typename aux_::dependency<
        TScope
      , TExpected
      , TGiven
      , typename mpl::if_<mpl::empty<context>, mpl::vector0<>, context>::type
      , mpl::or_<
            is_base_of<mpl::_1, TExpected>
          , is_same<mpl::_1, TExpected>
        >
    > type;
};

} // namespace di
} // namespace boost

#endif

