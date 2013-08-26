//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_HAS_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_HAS_TRAITS_HPP

#include <boost/mpl/has_xxx.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/mpl/aux_/yes_no.hpp>

#include "boost/di/config.hpp"

namespace boost {
namespace di {
namespace type_traits {

BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)
BOOST_MPL_HAS_XXX_TRAIT_DEF(result_type)
BOOST_MPL_HAS_XXX_TRAIT_DEF(value_type)
BOOST_MPL_HAS_XXX_TRAIT_DEF(policy_type)
BOOST_MPL_HAS_XXX_TRAIT_DEF(externals)
BOOST_MPL_HAS_XXX_TRAIT_DEF(context)
BOOST_MPL_HAS_XXX_TRAIT_DEF(types)
BOOST_MPL_HAS_XXX_TRAIT_DEF(deps)
BOOST_MPL_HAS_XXX_TRAIT_DEF(name)

template<typename T>
class BOOST_PP_CAT(has_, BOOST_DI_CONSTRUCTOR)
{
    template<typename C> static mpl::aux::yes_tag test(BOOST_TYPEOF(&C::BOOST_DI_CONSTRUCTOR));
    template<typename>   static mpl::aux::no_tag  test(...);

public:
    static const bool value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag);
};

template<typename T>
class BOOST_PP_CAT(has_, BOOST_DI_CREATE)
{
    template<typename C> static mpl::aux::yes_tag test(BOOST_TYPEOF(&C::BOOST_DI_CREATE));
    template<typename>   static mpl::aux::no_tag  test(...);

public:
    static const bool value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag);
};

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

