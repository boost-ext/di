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
    struct get_impl {
        template<class Q, std::enable_if_t<creatable_<TInjector, Q, TError>(), int> = 0>
        auto blah() const   {
            return injector_.template create_impl<T, no_name, std::false_type, TError>();
        }

        template<class Q, std::enable_if_t<!creatable_<TInjector, Q, TError>(), int> = 0>
        void blah() const;

        const TInjector& injector_;
    };

    template<class... Ts>
    struct get_impl<any_type<Ts...>> {
        template<class Q>
        auto blah() const  {
            return any_type<TParent, TInjector, TError>{injector_};
        }

        const TInjector& injector_;
    };

   template<class TName_, class T>
    struct get_impl<type_traits::named<TName_, T>> {
        template<class Q, std::enable_if_t<!std::is_same<Q, void>{} && creatable_<TInjector, T, TError, TName_>(), int> = 0>
        auto blah() const  {
            return injector_.template create_impl<T, TName_, std::false_type, TError>();
        }

        template<class Q, std::enable_if_t<!std::is_same<Q, void>{} && !creatable_<TInjector, T, TError, TName_>(), int> = 0>
        void blah() const;

        const TInjector& injector_;
    };

    template<class TMemory = type_traits::heap>
    auto get(const TMemory& memory = {}) const
    -> decltype(
        std::declval<TInjector>().provider().template get<TExpected, TGiven, TName, TError>(
            TInitialization{}
          , memory
          , get_impl<TArgs>{std::declval<TInjector>()}.template blah<TArgs>()...
        )
            )
    {
        return injector_.provider().template get<TExpected, TGiven, TName, TError>(
            TInitialization{}
          , memory
          , get_impl<TArgs>{injector_}.template blah<TArgs>()...
        );
    }

    const TInjector& injector_;
};

}}} // boost::di::core

#endif

