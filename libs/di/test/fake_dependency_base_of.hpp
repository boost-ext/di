//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_DEPENDENCY_BASE_OF_HPP
#define BOOST_DI_FAKE_DEPENDENCY_BASE_OF_HPP

#include "boost/di/aux_/dependency.hpp"

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>
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
  , typename TBind =
        mpl::or_<
            is_base_of<mpl::_1, TExpected>
          , is_same<mpl::_1, TExpected>
        >
>
struct fake_dependency_base_of
{
    typedef TBind bind;
    typedef TExpected expected;
    typedef TGiven given;
    typedef mpl::vector<TContext0, TContext1, TContext2> context;
    typedef typename aux_::dependency<
        TScope
      , TExpected
      , TGiven
      , typename mpl::if_<
            mpl::empty<context>
          , mpl::vector0<>
          , context
        >::type
      , TBind
    > type;

    template<
        typename Expected
      , typename Given
    >
    struct rebind
    {
        typedef fake_dependency_base_of<
            TScope
          , Expected
          , Given
          , TContext0
          , TContext1
          , TContext2
        > type;
    };
};

} // namespace di
} // namespace boost

#endif


