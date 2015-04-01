//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_CALLABLE_HPP
#define BOOST_DI_CONCEPTS_CALLABLE_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/core/dependency.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/fwd.hpp"

namespace boost { namespace di { namespace concepts {

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

template<class... Ts>
using callable =
    typename std::is_same<
        aux::bool_list<aux::always<Ts>{}...>
      , aux::bool_list<(
            decltype(callable_impl(std::declval<Ts>(), arg{})){} ||
            decltype(callable_impl(std::declval<Ts>(), arg{}, core::dependency<scopes::deduce, Ts>{}, ctor{})){})...
        >
    >::type;

}}} // boost::di::concepts

#endif

