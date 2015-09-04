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
#include "boost/di/scopes/singleton.hpp"
#include "boost/di/scopes/unique.hpp"
#include "boost/di/concepts/boundable.hpp"

namespace boost { namespace di { inline namespace v1 {

namespace detail {

template<class T>
struct type_ {
    using type = T;
};

template<class... Ts, BOOST_DI_REQUIRES(aux::is_unique<Ts...>::value) = 0>
aux::type_list<Ts...> any_of(Ts&&...);

template<class T>
type_<T> any_of(T&&);

template<class... Ts>
using any_of_t = typename decltype(detail::any_of(std::declval<Ts>()...))::type;

} // namespace detail

#if defined(__cpp_variable_templates) // __pph__
    template<class... Ts, BOOST_DI_REQUIRES_MSG(concepts::boundable<concepts::any_of<Ts...>>) = 0>
    core::dependency<scopes::deduce, detail::any_of_t<Ts...>> bind{};
#else // __pph__
    template<class... Ts>
    struct bind : core::dependency<scopes::deduce, detail::any_of_t<Ts...>> { };
#endif // __pph__

static constexpr BOOST_DI_UNUSED core::override override{};
static constexpr BOOST_DI_UNUSED scopes::deduce deduce{};
static constexpr BOOST_DI_UNUSED scopes::unique unique{};
static constexpr BOOST_DI_UNUSED scopes::singleton singleton{};

}}} // boost::di::v1

#endif

