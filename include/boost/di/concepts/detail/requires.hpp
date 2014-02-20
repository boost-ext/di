//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_DETAIL_REQUIRES_HPP
#define BOOST_DI_CONCEPTS_DETAIL_REQUIRES_HPP

#include "boost/di/aux_/config.hpp"

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/times.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/max_element.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/integral_c.hpp>

namespace boost {
namespace di {
namespace concepts {
namespace detail {

template<
    typename T1 = mpl::na
  , typename T2 = mpl::na
  , typename T3 = mpl::na
  , typename T4 = mpl::na
>
struct requires
    : mpl::second<
          typename mpl::fold<
              mpl::vector<T1, T2, T3, T4>
            , mpl::pair<mpl::integral_c<long, 1>, mpl::integral_c<long, 1> >
            , mpl::pair<
                  mpl::times<mpl::first<mpl::_1>, mpl::integral_c<long, 100> >
                , mpl::times<mpl::first<mpl::_1>, mpl::second<mpl::_1>, mpl::_2>
              >
          >::type
      >
{ };

} // namespace detail
} // namespace concepts
} // namespace di
} // namespace boost

#endif

