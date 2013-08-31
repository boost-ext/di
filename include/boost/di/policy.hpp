//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICY_HPP
#define BOOST_DI_POLICY_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/void.hpp>

#include "boost/di/config.hpp"

namespace boost {
namespace di {

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
class policy
{
    template<
        typename TDeps
      , typename T
      , typename TPolicy
    >
    struct verify_impl
        : TPolicy::template verify<TDeps, T>::type
    { };

public:
    typedef policy type;
    typedef mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> policy_type;

    template<
        typename TDeps
      , typename T
    >
    struct verify
        : mpl::fold<
              policy_type
            , mpl::void_
            , verify_impl<TDeps, T, mpl::_2>
          >::type
    {
        typedef verify type;
    };
};

} // namespace di
} // namespace boost

#endif

