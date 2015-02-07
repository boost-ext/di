//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_CREATABLE_HPP
#define BOOST_DI_CONCEPTS_CREATABLE_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/core/any_type.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/core/policy.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/scopes/exposed.hpp"
#include "boost/di/scopes/external.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"

namespace boost { namespace di { namespace concepts {

template<class, class, class = void, class = no_name, class = std::false_type>
struct create;

template<class, class, class, class, class>
struct is_creatable_impl;

template<class T, class TDeps, class TPolicies>
struct get_type {
    using type = std::conditional_t<
        std::is_convertible<create<TDeps, TPolicies>, T>{}
      , T
      , void
    >;
};

template<class TParent, class TNone, class TDeps, class TPolicies>
struct get_type<core::any_type<TParent, TNone>, TDeps, TPolicies> {
    using type = create<TDeps, TPolicies, TParent>;
};

template<class TName, class T, class TDeps, class TPolicies>
struct get_type<type_traits::named<TName, T>, TDeps, TPolicies> {
    using type = std::conditional_t<
        std::is_convertible<create<TDeps, TPolicies, void, TName>, T>{}
      , T
      , void
    >;
};

template<
    class TScope
  , class T
  , class TDeps
  , class TPolicies
  , class... TCtor
> struct is_creatable_impl<
    TScope
  , T
  , TDeps
  , aux::pair<type_traits::direct, aux::type_list<TCtor...>>
  , TPolicies
> : aux::identity<std::is_constructible<
        T
      , typename get_type<TCtor, TDeps, TPolicies>::type...
    >>
{ };

template<
    class TScope
  , class T
  , class TDeps
  , class TPolicies
  , class... TCtor
> struct is_creatable_impl<
    TScope
  , T
  , TDeps
  , aux::pair<type_traits::uniform, aux::type_list<TCtor...>>
  , TPolicies
> : aux::is_braces_constructible<
        T
      , typename get_type<TCtor, TDeps, TPolicies>::type...
    >
{ };

template<
    class TScope
  , class T
  , class TDeps
  , class TPolicies
  , class... TCtor
> struct is_creatable_impl<
    scopes::exposed<TScope>
  , T
  , TDeps
  , aux::pair<type_traits::direct, aux::type_list<TCtor...>>
  , TPolicies
> : std::true_type
{ };

template<
    class TScope
  , class T
  , class TDeps
  , class TPolicies
  , class... TCtor
> struct is_creatable_impl<
    scopes::exposed<TScope>
  , T
  , TDeps
  , aux::pair<type_traits::uniform, aux::type_list<TCtor...>>
  , TPolicies
> : std::true_type
{ };

template<
    class T
  , class TDeps
  , class TPolicies
  , class... TCtor
> struct is_creatable_impl<
    scopes::external
  , T
  , TDeps
  , aux::pair<type_traits::direct, aux::type_list<TCtor...>>
  , TPolicies
> : std::true_type
{ };

template<
    class T
  , class TDeps
  , class TPolicies
  , class... TCtor
> struct is_creatable_impl<
    scopes::external
  , T
  , TDeps
  , aux::pair<type_traits::uniform, aux::type_list<TCtor...>>
  , TPolicies
> : std::true_type
{ };

template<class, class, class, class, class, class>
struct call_policies;

template<
    class TScope
  , class T
  , class TDeps
  , class TCtor
  , class TPolicies
> using is_creatable_impl_t =
    typename is_creatable_impl<
        TScope
      , T
      , TDeps
      , TCtor
      , TPolicies
    >::type;

template<
    class T
  , class TDependency
  , class TName
  , class TIsRoot
  , class TDeps
  , class... Ts
> struct call_policies<
    T
  , TDependency
  , TName
  , TIsRoot
  , TDeps
  , core::pool<aux::type_list<Ts...>>
> {
    struct arg {
        using type = T;
        using name = TName;
        using is_root = TIsRoot;

        template<class T_, class TName_, class TDefault_>
        using resolve =
            decltype(core::binder::resolve<T_, TName_, TDefault_>((TDeps*)nullptr));
    };

    template<class TPolicy, class = void>
    struct call : std::true_type { };

    template<class TPolicy>
    struct call<TPolicy, std::enable_if_t<core::has_compile_time<TPolicy>{}>>
        : decltype((TPolicy{})(arg{}))
    { };

    static constexpr auto value =
        std::is_same<
            aux::bool_list<aux::always<Ts>{}...>
          , aux::bool_list<call<Ts>{}...>
        >::value;
};

template<
    class T
  , class TName
  , class TIsRoot
  , class TDependency
  , class TDeps
  , class TCtor
  , class TPolicies
> struct is_createable_impl {
    using type = std::integral_constant<bool,
        is_creatable_impl_t<
            typename TDependency::scope
          , typename TDependency::given
          , TDeps
          , TCtor
          , TPolicies
        >::value &&
        call_policies<
            T
          , TDependency
          , TName
          , TIsRoot
          , TDeps
          , TPolicies
        >::value
    >;
};

template<
    class T
  , class TName
  , class TIsRoot
  , class TDependency
  , class TDeps
  , class TCtor
> struct is_createable_impl<
    T
  , TName
  , TIsRoot
  , TDependency
  , TDeps
  , TCtor
  , core::pool<aux::type_list<>>
> {
    using type = is_creatable_impl_t<
        typename TDependency::scope
      , typename TDependency::given
      , TDeps
      , TCtor
      , core::pool<aux::type_list<>>
    >;
};

template<
    class T
  , class TName
  , class TIsRoot
  , class TDependency
  , class TDeps
  , class TCtor
  , class TPolicies
> using is_createable_impl_t =
    std::enable_if_t<
        is_createable_impl<
            T
          , TName
          , TIsRoot
          , TDependency
          , TDeps
          , TCtor
          , TPolicies
        >::type::value
    >;

template<
    class T
  , class TParent
  , class TDeps
  , class TName
  , class TIsRoot
  , class TPolicies
  , class TDependency = std::remove_reference_t<
        decltype(core::binder::resolve<T, TName>((TDeps*)nullptr))
    >
  , class TCtor = typename type_traits::ctor_traits<typename TDependency::given>::type
  , class = core::is_not_same<T, TParent>
  , class = is_createable_impl_t<T, TName, TIsRoot, TDependency, TDeps, TCtor, TPolicies>
> struct is_creatable { };

template<
    class TDeps
  , class TPolicies
  , class TParent
  , class TName
  , class TIsRoot
> struct create {
    template<class T>
    using is_creatable_t =
        is_creatable<T, TParent, TDeps, TName, TIsRoot, TPolicies>;

    template<class T, class = is_creatable_t<T>>
    operator T();

    template<class T, class = is_creatable_t<T>>
    operator T&() const;
};

std::false_type creatable_impl(...);

template<class T, class TDeps, class TPolicies>
auto creatable_impl(T&&, TDeps&&, TPolicies&&) -> aux::is_valid_expr<
    decltype(
        create<
            core::pool<TDeps>
          , TPolicies
          , void // parent
          , no_name // name
          , std::true_type // is_root
        >{}.operator T()
    )
>;

template<class TDeps, template<class> class TConfig, class... Ts>
constexpr auto creatable() {
#if defined(BOOST_DI_CFG_ENABLE_CREATABLE_CONCEPT)
    return std::is_same<
        aux::bool_list<aux::always<Ts>{}...>
      , aux::bool_list<decltype(
            creatable_impl(std::declval<Ts>()
                         , std::declval<TDeps>()
                         , std::declval<TConfig<int>>().policies())
        ){}...>
    >{};
#else
    return true;
#endif
}

}}} // boost::di::concepts

#endif

