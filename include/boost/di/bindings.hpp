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

namespace boost { namespace di { inline namespace v1 { namespace detail {

template<class...>
struct bind;

template<class TScope, class... Ts>
struct bind<int, TScope, Ts...> {
    using type = core::dependency<TScope, concepts::any_of<Ts...>>;
};

template<class TScope, class T>
struct bind<int, TScope, T> {
    using type = core::dependency<TScope, T>;
};

} // namespace detail

template<class... Ts>
#if !defined(__cpp_variable_templates) // __pph__
    struct bind :
#else // __pph__
    typename
#endif // __pph__
    detail::bind<
        BOOST_DI_REQUIRES_MSG(concepts::boundable<concepts::any_of<Ts...>>)
      , scopes::deduce
      , Ts...
    >::type
#if defined(__cpp_variable_templates) // __pph__
    bind
#endif // __pph__
{ };

static constexpr BOOST_DI_UNUSED core::override override{};
static constexpr BOOST_DI_UNUSED scopes::deduce deduce{};
static constexpr BOOST_DI_UNUSED scopes::unique unique{};
static constexpr BOOST_DI_UNUSED scopes::singleton singleton{};

template<class>
struct named { };

}}} // boost::di::v1

#endif

