//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_DEPENDENCY_HPP
#define BOOST_DI_FAKE_DEPENDENCY_HPP

#include "boost/di/detail/dependency.hpp"

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/or.hpp>

namespace boost {
namespace di {

template<
    typename TScope
  , typename TExpected
  , typename TGiven = TExpected
  , typename TContext0 = mpl::na
  , typename TContext1 = mpl::na
  , typename TContext2 = mpl::na
  , typename TBind = is_same<mpl::_1, TExpected>
>
struct fake_dependency
{
    typedef TBind bind;
    typedef TExpected expected;
    typedef TGiven given;
    typedef mpl::vector<TContext0, TContext1, TContext2> context;
    typedef fake_dependency type;

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
          , TContext0
          , TContext1
          , TContext2
        > type;
    };

    template<typename>
    struct result_type
    {
        typedef TExpected type;
    };
};

} // namespace di
} // namespace boost

#endif

