//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_BOUNDABLE_HPP
#define BOOST_DI_CONCEPTS_BOUNDABLE_HPP

#include <type_traits>
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/pool.hpp"

namespace boost { namespace di {

template<class...>
struct bound_type {
    struct is_bound_more_than_once { };

    template<class>
    struct is_not_base_of_or_is_convertible_to { };
};

namespace concepts {

template<class T>
using unique_dependency = aux::pair<
    aux::pair<typename T::expected, typename T::name>
  , std::integral_constant<bool, T::scope::priority>
>;

std::false_type boundable_impl(...);
std::false_type boundable_error_impl(...);

template<class... Ts>
auto boundable_impl(aux::type_list<Ts...>&&) -> aux::is_unique<unique_dependency<Ts>...>;

template<class>
struct get_is_unique_error;

template<class T, class TName, class TPriority>
struct get_is_unique_error<aux::not_unique<aux::pair<aux::pair<T, TName>, TPriority>>> {
    using type = typename bound_type<T, TName>::is_bound_more_than_once;
};

template<class... Ts>
auto boundable_error_impl(aux::type_list<Ts...>&&) ->
    typename get_is_unique_error<typename aux::is_unique<unique_dependency<Ts>...>::type>::type;

template<class I, class T>
auto boundable_impl(I&&, T&&) -> std::integral_constant<bool,
    std::is_convertible<T, I>{} || std::is_base_of<I, T>{}
>;

template<class I, class T>
auto boundable_error_impl(I&&, T&&) -> typename bound_type<T>::template is_not_base_of_or_is_convertible_to<I>;

template<class T, class... Ts>
auto boundable_impl(aux::type_list<Ts...>&&, T&&) ->
    std::integral_constant<
        bool
      , !std::is_same<
            aux::bool_list<aux::never<Ts>{}...>
          , aux::bool_list<std::is_base_of<Ts, T>{}...>
        >{}
    >;

template<class T, class... Ts>
auto boundable_error_impl(aux::type_list<Ts...>&&, T&&) -> int;

template<class... Ts>
constexpr auto boundable_error() {
    return decltype(boundable_error_impl(std::declval<Ts>()...)){};
}

template<class... Ts>
constexpr auto boundable() {
    return decltype(boundable_impl(std::declval<Ts>()...)){};
}

}}} // boost::di::concepts

#endif

