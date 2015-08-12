//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_PROVIDABLE_HPP
#define BOOST_DI_CONCEPTS_PROVIDABLE_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/memory_traits.hpp"

namespace boost { namespace di { inline namespace v1 { namespace concepts {

struct get { };
struct is_creatable { };

template<class T>
struct provider {
    template<class = get, class = is_creatable>
    struct requires_ { BOOST_DI_CONCEPT_ASSERT(T, providable, "type requires `auto get() const` and `is_creatable`"); };
};

template<class T>
typename provider<T>::template requires_<> providable_impl(...);

template<class T>
auto providable_impl(T&& t) -> aux::is_valid_expr<
    decltype(t.template get<_, _>(type_traits::direct{}, type_traits::heap{}))
  , decltype(t.template get<_, _>(type_traits::direct{}, type_traits::heap{}, int{}))
  , decltype(t.template get<_, _>(type_traits::uniform{}, type_traits::stack{}))
  , decltype(t.template get<_, _>(type_traits::uniform{}, type_traits::stack{}, int{}))
  , decltype(T::template is_creatable<type_traits::direct, type_traits::heap, _>::value)
  , decltype(T::template is_creatable<type_traits::uniform, type_traits::stack, _, int>::value)
>;

template<class T>
struct providable__ {
    using type = decltype(providable_impl<T>(std::declval<T>()));
};

template<class T>
using providable = typename providable__<T>::type;

}}}} // boost::di::v1::concepts

#endif

