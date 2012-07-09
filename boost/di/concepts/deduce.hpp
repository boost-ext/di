//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_DEDUCE_HPP
#define BOOST_DI_CONCEPTS_DEDUCE_HPP

#include "boost/di/detail/dependency.hpp"
#include "boost/di/config.hpp"

namespace boost {
namespace di {
namespace concepts {

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct deduce
    : di::detail::dependency<
           mpl::_1
         , TExpected
         , TGiven
         , TContext
         , mpl::or_<
              is_base_of<mpl::_1, TExpected>
            , is_same<mpl::_1, TExpected>
           >
      >
{ };

} // namespace concepts
} // namespace front
} // namespace di

#endif

