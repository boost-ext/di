//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_CREATABLE_HPP
#define BOOST_DI_CONCEPTS_CREATABLE_HPP

#include <type_traits>
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/scopes/exposed.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"

namespace boost { namespace di { namespace concepts {

template<class, class, class>
struct creatable_impl;

template<class TScope, class T, class... TArgs>
struct creatable_impl<TScope, T, aux::pair<type_traits::uniform, aux::type_list<TArgs...>>> {
    using type = aux::is_braces_constructible<T, TArgs...>;
};

template<class TScope, class T, class... TArgs>
struct creatable_impl<TScope, T, aux::pair<type_traits::direct, aux::type_list<TArgs...>>> {
    using type = std::is_constructible<T, TArgs...>;
};

template<class TScope, class T, class... TArgs>
struct creatable_impl<scopes::exposed<TScope>, T, aux::pair<type_traits::direct, aux::type_list<TArgs...>>> {
    using type = std::true_type;
};

template<class TScope, class T, class... TArgs>
struct creatable_impl<scopes::exposed<TScope>, T, aux::pair<type_traits::uniform, aux::type_list<TArgs...>>> {
    using type = std::true_type;
};

template<class TParent, class TDeps>
struct any {
    template<class T>
    using any_ = any<T, TDeps>;

    template<class T
           , class U = aux::decay_t<T>
           , class D = std::remove_reference_t<decltype(core::binder::resolve<U>((TDeps*)nullptr))>
           , class = std::enable_if_t<!(std::is_same<U, TParent>{} || std::is_base_of<TParent, U>{})>
           , class = std::enable_if_t<
                 typename creatable_impl<
                     typename D::scope
                   , typename D::given
                   , typename type_traits::ctor_traits<typename D::given, any_>::type
                 >::type{}
             >
    > operator T();
};

template<class, class, class = void>
struct creatable : std::false_type { };

template<class T, class TDeps>
struct creatable<T, TDeps, aux::void_t<decltype(any<void, core::pool<TDeps>>{}.operator T())>>
    : std::true_type
{ };

}}} // boost::di::concepts

#endif

