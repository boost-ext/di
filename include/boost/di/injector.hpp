//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_INJECTOR_HPP
#define BOOST_DI_INJECTOR_HPP

#include "boost/di/core/injector.hpp"
#include "boost/di/core/dependency.hpp"
#include "boost/di/scopes/exposed.hpp"

namespace boost { namespace di { namespace detail {

BOOST_DI_HAS_TYPE(deps);

template<class T>
using is_injector =
    std::integral_constant<bool, has_deps<T>{} || core::has_configure<T>{}>;

template<class T, class = void>
struct get_deps {
    using type = typename T::deps;
};

template<class T>
struct get_deps<T, std::enable_if_t<core::has_configure<T>{}>> {
    using type = typename aux::function_traits<
        decltype(&T::configure)
    >::result_type::deps;
};

template<
    class T
  , class = typename is_injector<T>::type
  , class = typename core::is_dependency<T>::type
> struct add_type_list;

template<class T, class TAny>
struct add_type_list<T, std::true_type, TAny> {
    using type = typename get_deps<T>::type;
};

template<class T>
struct add_type_list<T, std::false_type, std::true_type> {
    using type = aux::type_list<T>;
};

template<class T>
struct add_type_list<T, std::false_type, std::false_type> {
    using type = aux::type_list<core::dependency<scopes::exposed, T>>;
};

template<class... Ts>
using bindings_t =
    typename aux::join<typename add_type_list<Ts>::type...>::type;

template<class TConfig, class... TArgs>
using injector = core::injector<detail::bindings_t<TArgs...>, TConfig>;

} // detail

template<class... TArgs>
using injector = core::injector<detail::bindings_t<TArgs...>, config>;

}} // boost::di

#endif

