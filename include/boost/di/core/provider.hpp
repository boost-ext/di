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
#include "boost/di/concepts/creatable.hpp"
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
  , class TInitialization
  , class... TArgs
> struct provider<
    TExpected
  , TGiven
  , TName
  , TParent
  , aux::pair<TInitialization, aux::type_list<TArgs...>>
  , TInjector
> {
    template<class TMemory>
    static constexpr auto is_creatable() {
        using provider_t = decltype(injector_.provider());
        return provider_t::template is_creatable<TGiven, TInitialization, TMemory, TArgs...>();
    }

    template<class T>
    struct get_impl_ {
        static std::conditional_t<concepts::creatable_<TInjector, T>(), T, void> impl();
    };

    template<class... Ts>
    struct get_impl_<any_type<Ts...>> {
        static auto impl() -> any_type<TParent, TInjector, std::true_type>;
    };

    template<class TName_, class T>
    struct get_impl_<type_traits::named<TName_, T>> {
        static std::conditional_t<concepts::creatable_<TInjector, T, TName_>(), T, void> impl();
    };

    template<class TMemory, class... Ts, REQUIRES<is_creatable<TMemory>()> = 0>
    auto get__(Ts&&...) const -> std::conditional_t<std::is_same<TMemory, type_traits::stack>{}, TGiven, TGiven*>;

    template<class TMemory = type_traits::heap>
    auto get_(const TMemory& memory = {}) const -> decltype(
        get__<TMemory>(get_impl_<TArgs>::impl()...)
    );

    template<class TMemory = type_traits::heap, REQUIRES<is_creatable<TMemory>()> = 0>
    auto get(const TMemory& memory = {}) const {
        return injector_.provider().template get<TExpected, TGiven>(
            TInitialization{}
          , memory
          , get_impl(aux::type<TArgs>{})...
        );
    }

    template<class TMemory = type_traits::heap, REQUIRES<!is_creatable<TMemory>()> = 0>
    auto get(const TMemory& memory = {}) const {
        return concepts::creatable_error<TInitialization, TName, TGiven*, TArgs...>();
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



