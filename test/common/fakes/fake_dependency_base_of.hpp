//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_DEPENDENCY_BASE_OF_HPP
#define BOOST_DI_FAKE_DEPENDENCY_BASE_OF_HPP

#include "boost/di/concepts/dependency.hpp"

#include "boost/di/type_traits/is_same_base_of.hpp"

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
  , typename TBind = typename mpl::lambda<
        type_traits::is_same_base_of<TExpected, mpl::_1>
    >::type
>
struct fake_dependency_base_of
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
        typedef fake_dependency_base_of<
            TScope
          , typename mpl::if_<is_same<Given, void>, TExpected, Expected>::type
          , typename mpl::if_<is_same<Given, void>, TGiven, Given>::type
        > other;
    };
};

} // namespace di
} // namespace boost

#endif

