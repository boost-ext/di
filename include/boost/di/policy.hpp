//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICY_HPP
#define BOOST_DI_POLICY_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/void.hpp>

#include "boost/di/detail/meta.hpp"

namespace boost {
namespace di {

namespace detail { class policy_impl { }; } // namespace detail

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
class policy : detail::policy_impl
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
    template<typename TDeps, typename T>
    struct verify
        : mpl::fold<
              mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
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

