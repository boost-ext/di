//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_BIND_HPP
#define BOOST_DI_CONCEPTS_BIND_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/concepts/detail/requires.hpp"
#include "boost/di/concepts/detail/when.hpp"
#include "boost/di/concepts/type_traits/is_required_name.hpp"
#include "boost/di/concepts/type_traits/is_required_priority.hpp"
#include "boost/di/concepts/type_traits/is_required_type.hpp"

#include <boost/mpl/vector.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/if.hpp>

namespace boost {
namespace di {
namespace concepts {

namespace detail {

template<typename TExpected, typename TGiven>
struct get_expected
    : mpl::if_<
          mpl::is_sequence<TExpected>
        , TGiven
        , TExpected
      >
{ };

} // namespace detail

template<
    typename TExpected
  , typename TGiven
  , template<
        typename
      , typename
      , typename
      , typename
    > class TDependency
>
struct bind
    : TDependency<
          mpl::_1
        , typename detail::get_expected<TExpected, TGiven>::type
        , TGiven
        , detail::requires_<
              type_traits::is_required_priority
            , type_traits::is_required_type<TExpected>
          >
      >
{
    template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
    struct when
        : TDependency<
              mpl::_1
            , typename detail::get_expected<TExpected, TGiven>::type
            , TGiven
            , detail::requires_<
                  type_traits::is_required_priority
                , type_traits::is_required_type<TExpected>
                , detail::when_<mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> >
              >
          >
    {
        template<typename TName>
        struct named
            : TDependency<
                  mpl::_1
                , typename detail::get_expected<TExpected, TGiven>::type
                , TGiven
                , detail::requires_<
                      type_traits::is_required_priority
                    , type_traits::is_required_type<TExpected>
                    , type_traits::is_required_name<TName>
                    , detail::when_<mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> >
                  >
              >
        { };
    };

    template<typename TName>
    struct named
        : TDependency<
              mpl::_1
            , typename detail::get_expected<TExpected, TGiven>::type
            , TGiven
            , detail::requires_<
                  type_traits::is_required_priority
                , type_traits::is_required_type<TExpected>
                , type_traits::is_required_name<TName>
              >
          >
    {
        template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
        struct when
            : TDependency<
                  mpl::_1
                , typename detail::get_expected<TExpected, TGiven>::type
                , TGiven
                , detail::requires_<
                      type_traits::is_required_priority
                    , type_traits::is_required_type<TExpected>
                    , type_traits::is_required_name<TName>
                    , detail::when_<mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> >
                  >
              >
        { };
    };
};

} // namespace concepts
} // namespace di
} // namespace boost

#endif

