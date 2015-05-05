//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
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

namespace boost { namespace di { inline namespace v1 { namespace core {

template<class...>
struct provider;

template<
    class TExpected
  , class TGiven
  , class TName
  , class TParent
  , class TInjector
  , class TInitialization
  , class... TCtor
> struct provider<
    TExpected
  , TGiven
  , TName
  , TParent
  , aux::pair<TInitialization, aux::type_list<TCtor...>>
  , TInjector
> {
    using provider_t = decltype(std::declval<TInjector>().provider());

    template<class TMemory, class... TArgs>
    struct is_creatable {
        static constexpr auto value =
            provider_t::template is_creatable<TInitialization, TMemory, TGiven, TArgs...>::value;
    };

    template<class T>
    struct try_get_arg {
        using type = std::conditional_t<TInjector::template is_creatable<T>::value, T, void>;
    };

    template<class... TArgs>
    struct try_get_arg<any_type<TArgs...>> {
        using type = any_type<TParent, TInjector, std::true_type>;
    };

    template<class TName_, class T>
    struct try_get_arg<type_traits::named<TName_, T>> {
        using type = std::conditional_t<TInjector::template is_creatable<T, TName_>::value, T, void>;
    };

    template<class TMemory = type_traits::heap>
    auto try_get(const TMemory& memory = {}) const -> std::enable_if_t<
        is_creatable<TMemory, typename try_get_arg<TCtor>::type...>::value
      , std::conditional_t<std::is_same<TMemory, type_traits::stack>::value, TGiven, TGiven*>
    >;

    template<class TMemory = type_traits::heap>
    auto get(const TMemory& memory = {}) const {
        return get_impl(memory, get_arg(aux::type<TCtor>{})...);
    }

    template<class TMemory, class... TArgs, BOOST_DI_REQUIRES(is_creatable<TMemory, TArgs...>::value)>
    auto get_impl(const TMemory& memory, TArgs&&... args) const {
        return injector_.provider().template get<TExpected, TGiven>(
            TInitialization{}
          , memory
          , std::forward<TArgs>(args)...
        );
    }

    template<class TMemory, class... TArgs, BOOST_DI_REQUIRES(!is_creatable<TMemory, TArgs...>::value)>
    auto get_impl(const TMemory&, TArgs&&...) const {
        return concepts::creatable_error<TInitialization, TName, TExpected*, TGiven*, TArgs...>();
    }

    template<class T>
    auto get_arg(const aux::type<T>&) const {
        return injector_.template create_impl<T>();
    }

    template<class... TArgs>
    auto get_arg(const aux::type<any_type<TArgs...>>&) const {
        return any_type<TParent, TInjector>{injector_};
    }

    template<class T, class TName_>
    auto get_arg(const aux::type<type_traits::named<TName_, T>>&) const {
        return injector_.template create_impl<T, TName_>();
    }

    const TInjector& injector_;
};

}}}} // boost::di::v1::core

#endif

