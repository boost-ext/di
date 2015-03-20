//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_PROVIDER_HPP
#define BOOST_DI_CORE_PROVIDER_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/core/any_type.hpp"
#include "boost/di/type_traits/memory_traits.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"

namespace boost { namespace di { namespace core {

template<class...>
struct provider;

template<
    class TExpected
  , class TGiven
  , class TName
  , class TParent
  , class TInjector
  , class TError
  , class TInitialization
  , class... TArgs
> struct provider<
    TExpected
  , TGiven
  , TName
  , TParent
  , aux::pair<TInitialization, aux::type_list<TArgs...>>
  , TInjector
  , TError
> {
    template<class T>
    struct get_impl_ {
        template<class Q>
        static std::conditional_t<creatable_<TInjector, Q, TError>(), T, void> impl();
    };

    template<class... Ts>
    struct get_impl_<any_type<Ts...>> {
        template<class>
        static auto impl() -> any_type<TParent, TInjector, std::true_type>;
    };

    template<class TName_, class T>
    struct get_impl_<type_traits::named<TName_, T>> {
        template<class Q>
        static std::conditional_t<creatable_<TInjector, Q, TError, TName_>(), T, void> impl();
    };

    template<class TMemory = type_traits::heap>
    auto get_(const TMemory& memory = {}) const -> decltype(
        std::declval<TInjector>().provider().template get_<TExpected, TGiven, TName, TError>(
            TInitialization{}
          , memory
          , get_impl_<TArgs>::template impl<TArgs>()...
        )
    );

    template<class TMemory = type_traits::heap>
    auto get(const TMemory& memory = {}) const {
        return injector_.provider().template get<TExpected, TGiven, TName, TError>(
            TInitialization{}
          , memory
          , get_impl(aux::type<TArgs>{})...
        );
    }

    template<class T>
    auto get_impl(const aux::type<T>&) const {
        return injector_.template create_impl<T>();
    }

    template<class... Ts>
    auto get_impl(const aux::type<any_type<Ts...>>&) const {
        return any_type<TParent, TInjector>{injector_};
    }

    template<class T, class TName_>
    auto get_impl(const aux::type<type_traits::named<TName_, T>>&) const {
        return injector_.template create_impl<T, TName_>();
    }

    const TInjector& injector_;
};

}}} // boost::di::core

#endif

