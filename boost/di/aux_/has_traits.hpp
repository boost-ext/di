//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_HAS_TRAIT_HPP
#define BOOST_DI_AUX_HAS_TRAIT_HPP

#include <boost/mpl/has_xxx.hpp>
#include "boost/di/config.hpp"

namespace boost {
namespace di {
namespace aux_ {

BOOST_MPL_HAS_XXX_TRAIT_DEF(BOOST_DI_CTOR_UNIQUE_NAME)
BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)
BOOST_MPL_HAS_XXX_TRAIT_DEF(value_type)
BOOST_MPL_HAS_XXX_TRAIT_DEF(externals)
BOOST_MPL_HAS_XXX_TRAIT_DEF(policies)
BOOST_MPL_HAS_XXX_TRAIT_DEF(deps)
BOOST_MPL_HAS_XXX_TRAIT_DEF(pool)

} // namespace aux_
} // namespace di
} // namespace boost

#endif

