//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_BINDINGS_HPP
#define BOOST_DI_BINDINGS_HPP

#include "boost/di/aux_/compiler.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/dependency.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/scopes/session.hpp"
#include "boost/di/scopes/singleton.hpp"
#include "boost/di/scopes/shared.hpp"
#include "boost/di/scopes/unique.hpp"
#include "boost/di/concepts/boundable.hpp"

namespace boost { namespace di { inline namespace v1 {

namespace detail {
template<class... Ts, BOOST_DI_REQUIRES(aux::is_unique<Ts...>::value)>
auto any_of() {
    return aux::type_list<Ts...>{};
}
} // namespace detail

template<class T1, class T2, class... Ts>
using any_of = decltype(detail::any_of<T1, T2, Ts...>());

template<
    class TExpected
  , class TGiven = TExpected
  , BOOST_DI_REQUIRES_MSG(concepts::boundable<TExpected, TGiven>)
>
#if defined(__cpp_variable_templates)
    core::dependency<scopes::deduce, TExpected, TGiven> bind{};
#else
    struct bind : core::dependency<scopes::deduce, TExpected, TGiven> {};
#endif

static constexpr BOOST_DI_UNUSED core::override override{};
static constexpr BOOST_DI_UNUSED scopes::deduce deduce{};
static constexpr BOOST_DI_UNUSED scopes::unique unique{};
static constexpr BOOST_DI_UNUSED scopes::shared shared{};
static constexpr BOOST_DI_UNUSED scopes::singleton singleton{};

template<class TName>
constexpr auto session(const TName&) noexcept {
    return scopes::session<TName>{};
}

template<class TName>
constexpr auto session_entry(const TName&) noexcept {
    return scopes::session_entry<TName>{};
}

template<class TName>
constexpr auto session_exit(const TName&) noexcept {
    return scopes::session_exit<TName>{};
}

}}} // boost::di::v1

#endif

