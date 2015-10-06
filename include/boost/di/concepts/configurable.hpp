//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_CONFIGURABLE_HPP
#define BOOST_DI_CONCEPTS_CONFIGURABLE_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/concepts/providable.hpp"
#include "boost/di/concepts/callable.hpp"

namespace boost { namespace di { inline namespace v1 { namespace concepts {

template<class> struct policies { };
struct providable_type { };
struct callable_type { };

template<class>
struct config {
    template<class...>
    struct requires_ : aux::false_type { };
};

aux::false_type configurable_impl(...);

template<class T>
auto configurable_impl(T&& t) -> aux::is_valid_expr<
    decltype(T::provider(static_cast<const T&>(t)))
  , decltype(T::policies(static_cast<const T&>(t)))
>;

template<class T1, class T2>
struct get_configurable_error
    : aux::type_list<T1, T2>
{ };

template<class T>
struct get_configurable_error<aux::true_type, T> {
    using type = T;
};

template<class T>
struct get_configurable_error<T, aux::true_type> {
    using type = T;
};

template<>
struct get_configurable_error<aux::true_type, aux::true_type>
    : aux::true_type
{ };

template<class T>
auto is_configurable(const aux::true_type&) {
    return typename get_configurable_error<
        decltype(providable<decltype(T::provider(aux::declval<T>()))>())
      , decltype(callable<decltype(T::policies(aux::declval<T>()))>())
    >::type{};
}

template<class T>
auto is_configurable(const aux::false_type&) {
    return typename config<T>::template requires_<provider<providable_type(...)>, policies<callable_type(...)>>{};
}

template<class T>
struct configurable__ {
    using type = decltype(is_configurable<T>(decltype(configurable_impl(aux::declval<T>())){}));
};

template<class T>
using configurable = typename configurable__<T>::type;

}}}} // boost::di::v1::concepts

#endif

