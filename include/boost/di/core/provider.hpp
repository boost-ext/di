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
    using provider_t = decltype(std::declval<TInjector>().provider());

    template<class TMemory, class... Ts>
    struct is_creatable {
        static constexpr auto value =
            provider_t::template is_creatable<TInitialization, TMemory, TGiven, Ts...>::value;
    };

    template<class T>
    struct get_arg_ {
        using type = std::conditional_t<concepts::creatable_<TInjector, T>(), T, void>;
    };

    template<class... Ts>
    struct get_arg_<any_type<Ts...>> {
        using type = any_type<TParent, TInjector, std::true_type>;
    };

    template<class TName_, class T>
    struct get_arg_<type_traits::named<TName_, T>> {
        using type = std::conditional_t<concepts::creatable_<TInjector, T, TName_>(), T, void>;
    };

    template<class TMemory = type_traits::heap>
    auto get_(const TMemory& memory = {}) const -> std::enable_if_t<
        is_creatable<TMemory, typename get_arg_<TArgs>::type...>::value
      , std::conditional_t<std::is_same<TMemory, type_traits::stack>{}, TGiven, TGiven*>
    >;

    template<class TMemory = type_traits::heap>
    auto get(const TMemory& memory = {}) const {
        return get_impl(memory, get_arg(aux::type<TArgs>{})...);
    }

    template<class TMemory, class... Ts, BOOST_DI_REQUIRES(is_creatable<TMemory, Ts...>::value)>
    auto get_impl(const TMemory& memory, Ts&&... args) const {
        return injector_.provider().template get<TExpected, TGiven>(
            TInitialization{}
          , memory
          , args...
        );
    }

    template<class TMemory, class... Ts, BOOST_DI_REQUIRES(!is_creatable<TMemory, Ts...>::value)>
    auto get_impl(const TMemory& memory, Ts&&... args) const {
        return concepts::creatable_error<TInitialization, TName, TGiven*, Ts...>();
    }

    template<class T>
    auto get_arg(const aux::type<T>&) const {
        return injector_.template create_impl<T>();
    }

    template<class... Ts>
    auto get_arg(const aux::type<any_type<Ts...>>&) const {
        return any_type<TParent, TInjector>{injector_};
    }

    template<class T, class TName_>
    auto get_arg(const aux::type<type_traits::named<TName_, T>>&) const {
        return injector_.template create_impl<T, TName_>();
    }

    const TInjector& injector_;
};

}}} // boost::di::core

#endif

