//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_BOUNDABLE_HPP
#define BOOST_DI_CONCEPTS_BOUNDABLE_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/aux_/type_traits.hpp"
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
       aux::bool_list<aux::always<TDeps>::value...>
     , aux::bool_list<(core::is_injector<TDeps>::value || core::is_dependency<TDeps>::value)...>
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
          core::is_injector<T>::value || core::is_dependency<T>::value
        , typename get_not_supported<TDeps...>::type
        , T
      >
{ };

template<class>
struct is_unique;

template<class T>
using unique_dependency = aux::pair<
    aux::pair<typename T::expected, typename T::name>
  , typename T::priority
>;

template<class... TDeps>
struct is_unique<aux::type_list<TDeps...>>
    : aux::is_unique<unique_dependency<TDeps>...>
{ };

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
using get_bindings_error =
    std::conditional_t<
        is_supported<TDeps...>::value
      , typename get_is_unique_error<core::transform_t<TDeps...>>::type
      , typename bound_type<typename get_not_supported<TDeps...>::type>::
            is_neither_a_dependency_nor_an_injector
    >;

template<class... Ts>
using get_any_of_error =
    std::conditional_t<
        std::is_same<
            aux::bool_list<aux::always<Ts>::value...>
          , aux::bool_list<std::is_same<std::true_type, Ts>::value...>
        >::value
      , std::true_type
      , aux::type_list<Ts...>
    >;

template<class I, class T> // expected -> given
auto boundable_impl(I&&, T&&) ->
    std::conditional_t<
        std::is_base_of<I, T>::value || std::is_convertible<T, I>::value
      , std::true_type
      , std::conditional_t<
            std::is_base_of<I, T>::value
          , typename bound_type<T>::template is_not_convertible_to<I>
          , typename bound_type<T>::template is_not_base_of<I>
        >
    >;

template<class... TDeps> // bindings
auto boundable_impl(aux::type_list<TDeps...>&&) ->
#if defined(_MSC_VER)
    std::true_type;
#else
    get_bindings_error<TDeps...>;
#endif

template<class T, class... Ts> // any_of
auto boundable_impl(aux::type_list<Ts...>&&, T&&) ->
    get_any_of_error<decltype(boundable_impl(std::declval<Ts>(), std::declval<T>()))...>;

template<class... TDeps> // injector
auto boundable_impl(aux::type<TDeps...>&&) ->
    typename get_is_unique_error_impl<typename aux::is_unique<TDeps...>::type>::type;

std::true_type boundable_impl(...);

template<class... Ts>
using boundable = decltype(boundable_impl(std::declval<Ts>()...));

}}} // boost::di::concepts

#endif

