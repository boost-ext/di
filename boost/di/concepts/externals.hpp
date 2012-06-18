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

#include "boost/di/aux_/meta_config.hpp"
#include "boost/di/aux_/convertible.hpp"
#include "boost/di/scopes/singleton.hpp"
#include "boost/di/concepts/annotate.hpp"

namespace boost {
namespace di {
namespace concepts {

namespace detail {

template<typename T, typename Enable = void>
struct make_annotation
{
    typedef typename annotate<aux_::convertible<T> >::template with<> type;
};

template<typename T>
struct make_annotation<
    T
  , typename enable_if<is_base_of<annotate<>::with<>, T> >::type
>
{
    typedef typename T::template
        rebind<scopes::singleton<> >::type dependency;

    typedef aux_::convertible<
        typename dependency::expected
      , typename dependency::context
    > convertible;

    typedef typename annotate<convertible>::template
        with<typename T::name> type;
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

