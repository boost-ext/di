//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_EXTERNALS_HPP
#define BOOST_DI_CONCEPTS_EXTERNALS_HPP

#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/has_xxx.hpp>

#include "boost/di/type_traits/is_same_base_of.hpp"
#include "boost/di/scopes/external.hpp"
#include "boost/di/concepts/dependency.hpp"
#include "boost/di/concepts/annotate.hpp"
#include "boost/di/config.hpp"

namespace boost {
namespace di {
namespace concepts {

namespace detail {

BOOST_MPL_HAS_XXX_TRAIT_DEF(name)

template<typename T, typename = void>
struct make_annotation
{
    typedef typename annotate<
        dependency<
            scopes::external
          , T
          , T
          , mpl::vector0<>
          , typename mpl::lambda<
                type_traits::is_same_base_of<T>
            >::type
        >
    >::template with<> type;
};

template<typename T>
struct make_annotation<T, typename enable_if<has_name<T> >::type>
{
    typedef typename annotate<
        typename T::template rebind<scopes::external>::other
    >::template with<typename T::name> type;
};

} // namespace detail

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct externals
    : mpl::transform<
          mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
        , detail::make_annotation<mpl::_1>
      >::type
{ };

} // namespace concepts
} // namespace front
} // namespace di

#endif

