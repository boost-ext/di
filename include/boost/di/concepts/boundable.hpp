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
#include "boost/di/core/transform.hpp"

namespace boost { namespace di {

template<class...>
struct bound_type {
    struct is_bound_more_than_once { };
    struct is_neither_a_dependency_nor_an_injector { };

    template<class>
    struct is_not_base_of { };

    template<class>
    struct is_not_convertible_to { };
};

namespace concepts {

template<class... TDeps>
using is_supported =
    std::is_same<
       aux::bool_list<aux::always<TDeps>{}...>
     , aux::bool_list<(core::is_injector<TDeps>{} || core::is_dependency<TDeps>{})...>
    >;

template<class...>
struct get_not_supported;

template<class T>
struct get_not_supported<T> {
    using type = T;
};

template<class T, class... TDeps>
struct get_not_supported<T, TDeps...>
    : std::conditional<
          core::is_injector<T>{} || core::is_dependency<T>{}
        , typename get_not_supported<TDeps...>::type
        , T
      >
{ };

template<class>
struct is_unique;

template<class T>
using unique_dependency = aux::pair<
    aux::pair<typename T::expected, typename T::name>
  , std::integral_constant<bool, T::scope::priority>
>;

template<class... TDeps>
struct is_unique<aux::type_list<TDeps...>>
    : aux::is_unique<unique_dependency<TDeps>...>
{ };

template<class... TDeps>
auto boundable_impl(aux::type_list<TDeps...>&&) ->
    std::integral_constant<bool,
        is_supported<TDeps...>{} && is_unique<core::transform_t<TDeps...>>{}
    >;

template<class I, class T>
auto boundable_impl(I&&, T&&) ->
    std::integral_constant<bool,
        std::is_convertible<T, I>{} || std::is_base_of<I, T>{}
    >;

template<class T, class... Ts> // any_of
auto boundable_impl(aux::type_list<Ts...>&&, T&&) ->
    std::integral_constant<
        bool
      , !std::is_same<
            aux::bool_list<aux::never<Ts>{}...>
          , aux::bool_list<std::is_base_of<Ts, T>{}...>
        >{}
    >;

template<class... TDeps> // di::injector
auto boundable_impl(aux::type<TDeps...>&&) ->
    is_unique<core::transform_t<TDeps...>>;

std::false_type boundable_impl(...);

template<class... TDeps>
constexpr auto boundable() {
    return decltype(boundable_impl(std::declval<TDeps>()...)){};
}

template<class>
struct get_is_unique_error_impl
    : std::true_type
{ };

template<class T, class TName, class TPriority>
struct get_is_unique_error_impl<aux::not_unique<aux::pair<aux::pair<T, TName>, TPriority>>> {
    using type = typename bound_type<T, TName>::is_bound_more_than_once;
};

template<class T>
struct get_is_unique_error_impl<aux::not_unique<T>> {
    using type = typename bound_type<T>::is_bound_more_than_once;
};

template<class>
struct get_is_unique_error;

template<class... TDeps>
struct get_is_unique_error<aux::type_list<TDeps...>>
    : get_is_unique_error_impl<typename aux::is_unique<unique_dependency<TDeps>...>::type>
{ };

template<class... TDeps>
using get_error =
    std::conditional_t<
        is_supported<TDeps...>{}
      , typename get_is_unique_error<core::transform_t<TDeps...>>::type
      , typename bound_type<typename get_not_supported<TDeps...>::type>::
            is_neither_a_dependency_nor_an_injector
    >;

template<class... TDeps>
auto boundable_error_impl(aux::type_list<TDeps...>&&) -> get_error<TDeps...>;

template<class I, class T>
auto boundable_error_impl(I&&, T&&) ->
    std::conditional_t<
        std::is_base_of<I, T>{} || std::is_convertible<T, I>{}
      , std::true_type
      , std::conditional_t<
            std::is_base_of<I, T>{}
          , typename bound_type<T>::template is_not_convertible_to<I>
          , typename bound_type<T>::template is_not_base_of<I>
        >
    >;

template<class T, class... Ts> // any_of
auto boundable_error_impl(aux::type_list<Ts...>&&, T&&) -> int;

template<class... TDeps> // di::injector
auto boundable_error_impl(aux::type<TDeps...>&&) ->
    //get_is_unique_error_impl<typename aux::is_unique<unique_dependency<TDeps>...>::type>;
    typename get_is_unique_error_impl<typename aux::is_unique<TDeps...>::type>::type;

std::false_type boundable_error_impl(...);

template<class... TDeps>
constexpr auto boundable_error() {
    return decltype(boundable_error_impl(std::declval<TDeps>()...)){};
}

template<class... TDeps>
using boundable_ = decltype(boundable_error_impl(std::declval<TDeps>()...));

}}} // boost::di::concepts

#endif

