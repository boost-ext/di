//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_CONFIGURABLE_HPP
#define BOOST_DI_CONCEPTS_CONFIGURABLE_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/concepts/providable.hpp"

namespace boost { namespace di {

template<class>
struct config_type {
    struct is_not_configurable { };
};

namespace concepts {

std::false_type configurable_impl(...);
std::true_type configurable_error_impl(...);

template<class T>
auto configurable_impl(T&& t) -> aux::is_valid_expr<
    decltype(t.provider())
  , decltype(t.policies())
>;

template<class T>
auto configurable_error_impl(T&&) -> std::conditional_t<
    decltype(configurable_impl(std::declval<T>())){}
  , std::true_type
  , typename config_type<T>::is_not_configurable
>;

template<class T>
constexpr auto configurable_(const std::true_type&) {
    return providable<decltype(std::declval<T>().provider())>(); // && policies -> pool<type_list<...>>
}

template<class T>
constexpr auto configurable_(const std::false_type&) {
    return false;
}

template<class T>
using configurable = decltype(configurable_error_impl(std::declval<T>()));

}}} // boost::di::concepts

#endif

