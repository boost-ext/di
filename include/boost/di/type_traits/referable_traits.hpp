//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_REFERABLE_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_REFERABLE_TRAITS_HPP

#include "boost/di/aux_/compiler_specific.hpp"
#include "boost/di/aux_/type_traits.hpp"

namespace boost { namespace di { inline namespace v1 { namespace type_traits {

template<class T, class>
struct referable_traits {
    using type = T;
};

template<class T, class TDependency>
struct referable_traits<T&, TDependency> {
    using type = std::conditional_t<TDependency::template is_referable<T&>::value, T&, T>;
};

template<class T, class TDependency>
struct referable_traits<const T&, TDependency> {
    using type = std::conditional_t<TDependency::template is_referable<const T&>::value, const T&, T>;
};

BOOST_DI_WKND(BOOST_DI_MSVC)(
    template<class T, class TDependency>
    struct referable_traits<T&&, TDependency> {
        using type = std::conditional_t<TDependency::template is_referable<T&&>::value, T&&, T>;
    };
)()

template<class T, class TDependency>
using referable_traits_t = typename referable_traits<T, TDependency>::type;

}}}} // boost::di::v1::type_traits

#endif

