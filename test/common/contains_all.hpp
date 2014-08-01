//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONTAINS_ALL_HPP
#define BOOST_DI_CONTAINS_ALL_HPP

#include <boost/utility/enable_if.hpp>

namespace boost {
namespace di {

template<
    typename TSrc
  , typename TDst
  , typename = void
>
struct contains_all
    : aux::mpl::empty<
          typename aux::mpl::fold<
              TSrc
            , aux::mpl::vector<>
            , aux::mpl::if_<
                  aux::mpl::contains<TDst, aux::mpl::_2>
                , aux::mpl::_1
                , aux::mpl::push_back<aux::mpl::_1, aux::mpl::_2>
              >
          >::type
      >
{ };

template<typename TSrc, typename TDst>
struct contains_all<
    TSrc
  , TDst
  , typename enable_if<
        aux::mpl::not_equal_to<
            aux::mpl::size<TSrc>
          , aux::mpl::size<TDst>
        >
    >::type
>
    : aux::mpl::false_
{ };

} // namespace di
} // namespace boost

#endif

