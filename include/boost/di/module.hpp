//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_MODULE_HPP
#define BOOST_DI_MODULE_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/back_inserter.hpp>

#include "boost/di/scopes/deduce.hpp"
#include "boost/di/detail/module.hpp"
#include "boost/di/config.hpp"

namespace boost {
namespace di {

template<typename T>
struct default_scope
    : scope<scopes::deduce>::bind<T>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
class module
    : public detail::module<
          typename mpl::fold<
              mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
            , mpl::vector0<>
            , mpl::copy<
                  mpl::if_<
                      mpl::is_sequence<mpl::_2>
                    , mpl::_2
                    , default_scope<mpl::_2>
                  >
                , mpl::back_inserter<mpl::_1>
              >
          >::type
      >
{ };

} // namespace di
} // namespace boost

#endif

