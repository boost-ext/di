//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_BINDINGS_BIND_HPP
#define BOOST_DI_BINDINGS_BIND_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/mpl.hpp"
#include "boost/di/bindings/dependency.hpp"
#include "boost/di/bindings/detail/requires.hpp"
#include "boost/di/bindings/detail/when.hpp"
#include "boost/di/bindings/type_traits/is_required_name.hpp"
#include "boost/di/bindings/type_traits/is_required_type.hpp"
#include "boost/di/scopes/deduce.hpp"

namespace boost {
namespace di {
namespace bindings {

namespace detail {

template<typename TExpected, typename>
struct get_expected {
    using type = TExpected;
};

template<typename... Ts, typename TGiven>
struct get_expected<type_list<Ts...>, TGiven> {
    using type = TGiven;
};

} // namespace detail

template<
    typename TExpected
  , typename TGiven
>
struct bind
    : dependency<
          scopes::deduce
        , typename detail::get_expected<TExpected, TGiven>::type
        , TGiven
        , type_traits::is_required_type<TExpected>
      >
{
    template<typename... Ts>
    struct when
        : dependency<
              scopes::deduce
            , typename detail::get_expected<TExpected, TGiven>::type
            , TGiven
            , detail::requires_<
                  type_traits::is_required_type<TExpected>
                , detail::when_<Ts...>
              >
          >
    {
        template<typename TName>
        struct named
            : dependency<
                  scopes::deduce
                , typename detail::get_expected<TExpected, TGiven>::type
                , TGiven
                , detail::requires_<
                      type_traits::is_required_type<TExpected>
                    , type_traits::is_required_name<TName>
                    , detail::when_<Ts...>
                  >
              >
        { };
    };

    template<typename TName>
    struct named
        : dependency<
              scopes::deduce
            , typename detail::get_expected<TExpected, TGiven>::type
            , TGiven
            , detail::requires_<
                  type_traits::is_required_type<TExpected>
                , type_traits::is_required_name<TName>
              >
          >
    {
        template<typename... Ts>
        struct when
            : dependency<
                  scopes::deduce
                , typename detail::get_expected<TExpected, TGiven>::type
                , TGiven
                , detail::requires_<
                      type_traits::is_required_type<TExpected>
                    , type_traits::is_required_name<TName>
                    , detail::when_<Ts...>
                  >
              >
        { };
    };
};

} // namespace bindings
} // namespace di
} // namespace boost

#endif

