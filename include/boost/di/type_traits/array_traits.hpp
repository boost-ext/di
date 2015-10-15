//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_ARRAY_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_ARRAY_TRAITS_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/fwd.hpp"

namespace boost { namespace di { inline namespace v1 { namespace type_traits {

template<class, class T>
struct array_traits {
    using type = T;
};

template<class T, class U, class... Ts>
struct array_traits<T, core::array<U[], Ts...>> {
    using type = core::array<T, Ts...>;
};

template<class T, class U, class... Ts>
struct array_traits<const T&, core::array<U[], Ts...>> {
    using type = core::array<T, Ts...>;
};

template<class T, class U, class... Ts>
struct array_traits<T&, core::array<U[], Ts...>> {
    using type = core::array<T, Ts...>;
};

template<class T, class U, class... Ts>
struct array_traits<std::shared_ptr<T>, core::array<U[], Ts...>> {
    using type = core::array<T, Ts...>;
};

template<class T, class U>
using array_traits_t = typename array_traits<T, U>::type;

}}}} // boost::di::v1::type_traits

#endif

