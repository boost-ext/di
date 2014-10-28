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
#include "boost/di/scopes/deduce.hpp"

namespace boost {
namespace di {
namespace bindings {

namespace detail {

BOOST_DI_HAS_MEMBER_TYPE(name);

template<typename T, typename = void>
struct get_name {
    struct no_name { };
    using type = no_name;
};

template<typename T>
struct get_name<T, std::enable_if<has_name<T>{}>> {
    using type = typename T::name;
};

template<typename TName>
struct is_required_name {
    template<typename T>
    using apply = std::is_same<
        typename get_name<
            typename di::type_traits::remove_accessors<
                typename T::type
             >::type
        >::type
      , TName
    >;

    template<typename>
    using eval = std::true_type;
};

template<typename TValueType, template<typename...> class TComparator = std::is_same>
struct is_required_type {
    template<typename T>
    using apply = TComparator<
        TValueType
      , typename di::type_traits::make_plain<typename T::type>::type
    >;

    template<typename>
    using eval = std::false_type;
};

template<typename TValueType, template<typename...> class TComparator = std::is_same>
struct is_required_type__ {
    template<typename T>
    using apply = TComparator<
        TValueType
      , typename di::type_traits::make_plain<T>::type
    >;

    template<typename>
    using eval = std::false_type;
};

template<typename... Ts, template<typename...> class TComparator>
struct is_required_type__<type_list<Ts...>, TComparator> {
    template<typename T>
    using apply = or_<
        TComparator<
            typename di::type_traits::make_plain<typename T::type>::type
          , Ts
        >...
    >;

    template<typename>
    using eval = std::false_type;
};

template<typename TExpected, typename>
struct get_expected {
    using type = TExpected;
};

template<typename... Ts, typename TGiven>
struct get_expected<type_list<Ts...>, TGiven> {
    using type = TGiven;
};

template<typename TExpected, typename TGiven>
struct is_required_type_ : is_required_type__<TExpected, std::is_same>
{ };

template<typename T>
struct is_required_type_<T, T> : is_required_type__<T, std::is_base_of>
{ };

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
        , detail::is_required_type_<TExpected, TGiven>
      >
{
    template<typename TName>
    struct named
        : dependency<
              scopes::deduce
            , typename detail::get_expected<TExpected, TGiven>::type
            , TGiven
            , detail::requires_<
                  detail::is_required_type_<TExpected, TGiven>
                , detail::is_required_name<TName>
              >
          >
    { };
};

} // namespace bindings
} // namespace di
} // namespace boost

#endif

