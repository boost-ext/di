//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_DETAIL_REQUIRES_HPP
#define BOOST_DI_CONCEPTS_DETAIL_REQUIRES_HPP

#include "boost/di/aux_/config.hpp"

#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/times.hpp>
#include <boost/mpl/integral_c.hpp>

namespace boost {
namespace di {
namespace concepts {
namespace detail {

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
class requires
{
    template<
        typename TBind
      , typename T
      , typename TCallStack
      , typename TScope
    >
    struct apply_bind
        : TBind::template apply<
              T
            , TCallStack
            , TScope
          >::type
    { };

public:
    typedef requires type;

    template<typename T, typename TCallStack, typename TScope>
    struct apply
        : mpl::second<
              typename mpl::fold<
                  mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
                , mpl::pair<mpl::integral_c<long, 1>, mpl::integral_c<long, 1> >
                , mpl::pair<
                      mpl::times<mpl::first<mpl::_1>, mpl::integral_c<long, 100> >
                    , mpl::times<mpl::first<mpl::_1>, mpl::second<mpl::_1>, apply_bind<mpl::_2, T, TCallStack, TScope> >
                  >
              >::type
          >
    { };
};

} // namespace detail
} // namespace concepts
} // namespace di
} // namespace boost

#endif

