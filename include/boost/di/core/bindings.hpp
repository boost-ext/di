//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_BINDINGS_HPP
#define BOOST_DI_CORE_BINDINGS_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/dependency.hpp"
#include "boost/di/scopes/exposed.hpp"

namespace boost { namespace di { inline namespace v1 { namespace core {

template<
    class T
  , class = typename is_injector<T>::type
  , class = typename is_dependency<T>::type
> struct add_type_list;

template<class T, class TAny>
struct add_type_list<T, aux::true_type, TAny> {
    using type = typename T::deps;
};

template<class T>
struct add_type_list<T, aux::false_type, aux::true_type> {
    using type = aux::type_list<T>;
};

template<class T>
struct add_type_list<T, aux::false_type, aux::false_type> {
    using type = aux::type_list<dependency<scopes::exposed<>, T>>;
};

#if defined(_MSC_VER) // __pph__
    template<class... Ts>
    struct bindings : aux::join_t<typename add_type_list<Ts>::type...> { };

    template<class... Ts>
    using bindings_t = typename bindings<Ts...>::type;
#else // __pph__
    template<class... Ts>
    using bindings_t = aux::join_t<typename add_type_list<Ts>::type...>;
#endif // __pph__

}}}} // boost::di::v1::core

#endif

