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
    auto get_impl(const aux::type<T>&) const/* -> decltype(std::declval<TInjector>().template create_impl<T>())*/ {
        return injector_.template create_impl<T, no_name, std::false_type, TError>();
    }

    template<class... Ts>
    auto get_impl(const aux::type<any_type<Ts...>>&) const -> decltype(any_type<TParent, TInjector, TError>{std::declval<TInjector>()}) {
        return any_type<TParent, TInjector, TError>{injector_};
    }

    template<class T, class TName_>
    auto get_impl(const aux::type<type_traits::named<TName_, T>>&) const /*-> decltype(std::declval<TInjector>().template create_impl<T, TName_>())*/ {
        return injector_.template create_impl<T, TName_, std::false_type, TError>();
    }

    template<class TMemory = type_traits::heap>
    auto get(const TMemory& memory = {}) const
    -> decltype(
        std::declval<TInjector>().provider().template get<TExpected, TGiven, TName, TError>(
            TInitialization{}
          , memory
          , get_impl(aux::type<TArgs>{})...
        )
            )
    {
        return injector_.provider().template get<TExpected, TGiven, TName, TError>(
            TInitialization{}
          , memory
          , get_impl(aux::type<TArgs>{})...
        );
    }

    const TInjector& injector_;
};

}}} // boost::di::core

#endif

