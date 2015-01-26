//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_CALLABLE_HPP
#define BOOST_DI_CONCEPTS_CALLABLE_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/fwd.hpp"

namespace boost { namespace di { namespace concepts {

struct arg {
    using type = void;
    using name = no_name;
    using is_root = std::false_type;

    template<class, class, class>
    struct resolve;
};

std::false_type callable_impl(...);

template<class T, class TArg>
auto callable_impl(T&& t, TArg&& arg) -> aux::is_valid_expr<
    decltype(t(arg))
>;

template<class T, class TArg, class TDependency>
auto callable_impl(T&& t, TArg&& arg, TDependency&& dep) -> aux::is_valid_expr<
    decltype(t(arg, dep))
>;

template<class... T>
constexpr auto callable() {
    return std::is_same<
        aux::bool_list<aux::always<T>{}...>
      , aux::bool_list<(
            decltype(callable_impl(std::declval<T>(), arg{})){} ||
            decltype(callable_impl(std::declval<T>(), arg{}, arg{})){})...
        >
    >{};
}

}}} // boost::di::concepts

#endif

