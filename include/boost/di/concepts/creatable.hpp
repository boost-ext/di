//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_CREATABLE_HPP
#define BOOST_DI_CONCEPTS_CREATABLE_HPP

#include <type_traits>
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/scopes/exposed.hpp"
#include "boost/di/scopes/external.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"

namespace boost { namespace di { namespace concepts {

template<class, class, class = no_name, class = core::pool<>>
struct any;

template<class, class, class, class>
struct creatable_impl;

template<class T, class TDeps>
struct get_type {
    using type = any<void, TDeps>;
};

template<class TP, class X, class TDeps>
struct get_type<core::any_type<TP, X>, TDeps> {
    using type = any<void, TDeps>;
};

template<class TName, class T, class TDeps>
struct get_type<type_traits::named<TName, T>, TDeps> {
    using type = any<void, TDeps, TName>;
};

template<class TScope, class T, class TDeps, class... TArgs>
struct creatable_impl<TScope, T, TDeps, aux::pair<type_traits::direct, aux::type_list<TArgs...>>> {
    using type = std::is_constructible<T, typename get_type<TArgs, TDeps>::type...>;
};

template<class TScope, class T, class TDeps, class... TArgs>
struct creatable_impl<TScope, T, TDeps, aux::pair<type_traits::uniform, aux::type_list<TArgs...>>> {
    using type = aux::is_braces_constructible<T, typename get_type<TArgs, TDeps>::type...>;
};

template<class TScope, class T, class TDeps, class... TArgs>
struct creatable_impl<scopes::exposed<TScope>, T, TDeps, aux::pair<type_traits::direct, aux::type_list<TArgs...>>> {
    using type = std::true_type;
};

template<class TScope, class T, class TDeps, class... TArgs>
struct creatable_impl<scopes::exposed<TScope>, T, TDeps, aux::pair<type_traits::uniform, aux::type_list<TArgs...>>> {
    using type = std::true_type;
};

template<class T, class TDeps, class... TArgs>
struct creatable_impl<scopes::external, T, TDeps, aux::pair<type_traits::direct, aux::type_list<TArgs...>>> {
    using type = std::true_type;
};

template<class T, class TDeps, class... TArgs>
struct creatable_impl<scopes::external, T, TDeps, aux::pair<type_traits::uniform, aux::type_list<TArgs...>>> {
    using type = std::true_type;
};

template<class, class, class, class, class>
struct call_policies;

template <bool ...> struct bool_seq;

template<class>
using always = std::true_type;

template<class T, class TDependency, class TName, class TDeps, class... Ts>
struct call_policies<T, TDependency, TName, TDeps, core::pool<aux::type_list<Ts...>>> {
    struct arg {
        using type = T;
        using name = TName;
        using is_root = std::false_type;
        template<class T_, class TName_, class TDefault_>
        using resolve = decltype(core::binder::resolve<T_, TName_, TDefault_>((TDeps*)nullptr));
    };

    constexpr operator bool() const noexcept {
        return std::is_same<bool_seq<always<Ts>{}...>, bool_seq<(Ts)(arg{})...>>{};
    }
};

template<class TParent, class TDeps, class TName, class TPolicies>
struct any {
    template<
        class T
      , class U = aux::decay_t<T>
      , class D = std::remove_reference_t<decltype(core::binder::resolve<U, TName>((TDeps*)nullptr))>
      , class = std::enable_if_t<!(std::is_same<U, TParent>{} || std::is_base_of<TParent, U>{})>
      , class = std::enable_if_t<
            typename creatable_impl<
                typename D::scope
              , typename D::given
              , TDeps
              , typename type_traits::ctor_traits<typename D::given>::type
            >::type{} && call_policies<TPolicies, T, D, TName, TDeps>{}
        >
    > struct is_creatable { };

    template<class T, class = is_creatable<T>> operator T();
    template<class T, class = is_creatable<T>> operator T&() const;
};

std::false_type creatable(...);

template<class T, class TDeps, class TConfig>
auto creatable(T&&, TDeps&&, TConfig&& cfg) -> aux::is_valid_expr<
    decltype(any<void, core::pool<TDeps>, no_name, decltype(cfg.policies())>{}.operator T())
>;

}}} // boost::di::concepts

#endif

