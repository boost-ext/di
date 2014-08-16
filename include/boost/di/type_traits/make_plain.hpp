//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_MAKE_PLAIN_HPP
#define BOOST_DI_TYPE_TRAITS_MAKE_PLAIN_HPP

#include "boost/di/aux_/mpl.hpp"
#include "boost/di/type_traits/remove_accessors.hpp"

#include <type_traits>

namespace boost {
namespace di {
namespace type_traits {

template<typename>
struct make_plain;

BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)
BOOST_MPL_HAS_XXX_TRAIT_DEF(named_type)

template<typename T, typename = void>
struct deref_type {
    using type = T;
};

template<typename T>
struct deref_type<T, typename std::enable_if<has_element_type<T>::value>::type> {
    using type = typename T::element_type;
};

template<typename T>
struct deref_type<T, typename std::enable_if<has_named_type<T>::value>::type> {
    using type = typename make_plain<typename T::named_type>::type;
};

template<typename T>
struct make_plain
    : deref_type<
          typename remove_accessors<
              typename deref_type<
                  typename remove_accessors<T>::type
              >::type
          >::type
      >
{ };

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

