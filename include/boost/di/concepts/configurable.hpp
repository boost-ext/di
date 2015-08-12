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

template<class T>
struct config {
    template<class = provider<providable_type(...)>, class = policies<callable_type(...)>>
    struct requires_ { BOOST_DI_CONCEPT_ASSERT(T, configurable, "configuration has to be providable and callable"); };
};

std::false_type configurable_impl(...);

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
struct get_configurable_error<std::true_type, T> {
    using type = T;
};

template<class T>
struct get_configurable_error<T, std::true_type> {
    using type = T;
};

template<>
struct get_configurable_error<std::true_type, std::true_type>
    : std::true_type
{ };

template<class T>
auto is_configurable(const std::true_type&) {
    return typename get_configurable_error<
        decltype(providable<decltype(T::provider(std::declval<T>()))>())
      , decltype(callable<decltype(T::policies(std::declval<T>()))>())
    >::type{};
}

template<class T>
auto is_configurable(const std::false_type&) {
    return typename config<T>::template requires_<>{};
}

template<class T>
struct configurable__ {
    using type = decltype(is_configurable<T>(decltype(configurable_impl(std::declval<T>())){}));
};

template<class T>
using configurable = typename configurable__<T>::type;

}}}} // boost::di::v1::concepts

#endif

