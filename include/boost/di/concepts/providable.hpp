//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_PROVIDABLE_HPP
#define BOOST_DI_CONCEPTS_PROVIDABLE_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/memory_traits.hpp"

namespace boost { namespace di { namespace concepts {

std::false_type providable_impl(...);

template<class T>
auto providable_impl(T&& t) -> aux::is_valid_expr<
    decltype(t.template get<_, _>(type_traits::direct{}, type_traits::heap{}))
  , decltype(t.template get<_, _>(type_traits::direct{}, type_traits::heap{}, int{}))
  , decltype(t.template get<_, _>(type_traits::uniform{}, type_traits::stack{}))
  , decltype(t.template get<_, _>(type_traits::uniform{}, type_traits::stack{}, int{}))
>;

template<class T>
constexpr auto providable() {
    return decltype(providable_impl(std::declval<T>())){};
}

}}} // boost::di::concepts

#endif

