//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(_MSC_VER)
#ifndef BOOST_DI_CONCEPTS_CALLABLE_HPP
#define BOOST_DI_CONCEPTS_CALLABLE_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/core/dependency.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/fwd.hpp"

namespace boost { namespace di {

template<class...>
struct policy {
    struct is_not_callable { };
    struct is_not_configurable { };
};

namespace concepts {

struct arg {
    using type = void;
    using name = no_name;
    using is_root = std::false_type;

    template<class, class, class>
    struct resolve;
};

struct ctor { };

std::false_type callable_impl(...);

template<class T, class TArg>
auto callable_impl(T&& t, TArg&& arg) -> aux::is_valid_expr<
    decltype(t(arg))
>;

template<class T, class TArg, class TDependency, class... TCtor>
auto callable_impl(T&& t, TArg&& arg, TDependency&& dep, TCtor&&... ctor) -> aux::is_valid_expr<
    decltype(t(arg, dep, ctor...))
>;

template<class...>
struct is_callable_impl;

template<class T, class... Ts>
struct is_callable_impl<T, Ts...> {
    using type = std::conditional_t<
        decltype(callable_impl(std::declval<T>(), arg{}))::value ||
        decltype(callable_impl(std::declval<T>(), arg{}, core::dependency<scopes::deduce, T>{}, ctor{}))::value
      , typename is_callable_impl<Ts...>::type
      , typename policy<T>::is_not_callable
    >;
};

template<>
struct is_callable_impl<>
    : std::true_type
{ };

template<class... Ts>
struct is_callable
    : is_callable_impl<Ts...>
{ };

template<class... Ts>
struct is_callable<core::pool<aux::type_list<Ts...>>>
    : is_callable_impl<Ts...>
{ };

template<>
struct is_callable<void> { // auto
    using type = policy<>::is_not_configurable;
};

template<class... Ts>
using callable = typename is_callable<Ts...>::type;

}}} // boost::di::concepts

#endif

#endif
