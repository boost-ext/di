//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_BINDINGS_BIND_HPP
#define BOOST_DI_BINDINGS_BIND_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/bindings/detail/requires.hpp"
#include "boost/di/bindings/detail/when.hpp"
#include "boost/di/bindings/type_traits/is_required_name.hpp"
#include "boost/di/bindings/type_traits/is_required_priority.hpp"
#include "boost/di/bindings/type_traits/is_required_type.hpp"
#include "boost/di/scopes/deduce.hpp"

#include <boost/mpl/vector.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/if.hpp>

namespace boost {
namespace di {
namespace bindings {

namespace detail {

template<typename TExpected, typename TGiven>
struct get_expected
    : aux::mpl::if_<
          aux::mpl::is_sequence<TExpected>
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
          scopes::deduce
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
              scopes::deduce
            , typename detail::get_expected<TExpected, TGiven>::type
            , TGiven
            , detail::requires_<
                  type_traits::is_required_priority
                , type_traits::is_required_type<TExpected>
                , detail::when_<BOOST_DI_MPL_VECTOR_TYPES_PASS_MPL(T)>
              >
          >
    {
        template<typename TName>
        struct named
            : TDependency<
                  scopes::deduce
                , typename detail::get_expected<TExpected, TGiven>::type
                , TGiven
                , detail::requires_<
                      type_traits::is_required_priority
                    , type_traits::is_required_type<TExpected>
                    , type_traits::is_required_name<TName>
                    , detail::when_<BOOST_DI_MPL_VECTOR_TYPES_PASS_MPL(T)>
                  >
              >
        { };
    };

    template<typename TName>
    struct named
        : TDependency<
              scopes::deduce
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
                  scopes::deduce
                , typename detail::get_expected<TExpected, TGiven>::type
                , TGiven
                , detail::requires_<
                      type_traits::is_required_priority
                    , type_traits::is_required_type<TExpected>
                    , type_traits::is_required_name<TName>
                    , detail::when_<BOOST_DI_MPL_VECTOR_TYPES_PASS_MPL(T)>
                  >
              >
        { };
    };
};

} // namespace bindings
} // namespace di
} // namespace boost

#endif

