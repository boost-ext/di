//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
// // Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_BINDER_HPP
#define BOOST_DI_CORE_BINDER_HPP

#include "boost/di/aux_/mpl.hpp"
#include "boost/di/type_traits/make_plain.hpp"
#include "boost/di/bindings/dependency.hpp"
#include "boost/di/scopes/deduce.hpp"

namespace boost {
namespace di {
namespace core {

template<typename T, typename _>
using resolve_impl = pair<typename _::bind::template apply<T>::type, _>;

template<typename>
struct binder;

template<typename... Ts>
struct binder<type_list<Ts...>> {
    template<
        typename T
      , typename TDefault =
            bindings::dependency<
                scopes::deduce
              , type_traits::make_plain_t<T>
              , type_traits::make_plain_t<T>
              , bindings::detail::is_required_type<type_traits::make_plain_t<T>>
            >
    >
    using resolve = at_key_t<TDefault, std::true_type, resolve_impl<T, Ts>...>::
        template rebind<typename scopes::deduce::rebind<T>::other>::other;
};

} // namespace core
} // namespace di
} // namespace boost

#endif

