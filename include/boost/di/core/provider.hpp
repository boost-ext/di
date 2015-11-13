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
#include "boost/di/type_traits/memory_traits.hpp"
#include "boost/di/concepts/creatable.hpp"
#include "boost/di/fwd.hpp"

namespace core {

#if (BOOST_DI_CFG_DIAGNOSTICS_LEVEL >= 2)  // __pph__
template <class T>
struct creating {
  creating() { type(); }
  static inline T type(_ = "creating...");
};
#endif  // __pph__

template <class, class, class, class>
struct try_provider;

template <class T, class TInjector, class TProvider, class TInitialization, class... TCtor>
struct try_provider<T, aux::pair<TInitialization, aux::type_list<TCtor...>>, TInjector, TProvider> {
  using injector_t = TInjector;

  template <class TMemory>
  struct is_creatable {
    static constexpr auto value =
        TProvider::template is_creatable<TInitialization, TMemory, T,
                                         typename injector__<TInjector>::template try_create<TCtor>::type...>::value;
  };

  template <class TMemory = type_traits::heap>
  auto get(const TMemory& = {}) const -> aux::enable_if_t<
      is_creatable<TMemory>::value,
      aux::conditional_t<aux::is_same<TMemory, type_traits::stack>::value, T, aux::remove_reference_t<T>*>>;
};

template <class, class, class, class>
struct provider;

template <class T, class TName, class TInjector, class TInitialization, class... TCtor>
struct provider<T, TName, aux::pair<TInitialization, aux::type_list<TCtor...>>, TInjector> {
  using provider_t = decltype(TInjector::config::provider(aux::declval<TInjector>()));
  using injector_t = TInjector;

  template <class TMemory, class... TArgs>
  struct is_creatable {
    static constexpr auto value = provider_t::template is_creatable<TInitialization, TMemory, T, TArgs...>::value;
  };

  template <class TMemory = type_traits::heap>
  auto get(const TMemory& memory = {}) const {
    return get_impl(memory, static_cast<const injector__<TInjector>&>(injector_).create_impl(aux::type<TCtor>{})...);
  }

  template <class TMemory, class... TArgs, BOOST_DI_REQUIRES(is_creatable<TMemory, TArgs...>::value) = 0>
  auto get_impl(const TMemory& memory, TArgs&&... args) const {
#if (BOOST_DI_CFG_DIAGNOSTICS_LEVEL >= 2)  // __pph__
    (void)aux::conditional_t<injector__<TInjector>::template is_creatable<T>::value, _, creating<T>>{};
#endif  // __pph__

    return TInjector::config::provider(injector_)
        .template get<T>(TInitialization{}, memory, static_cast<TArgs&&>(args)...);
  }

  template <class TMemory, class... TArgs, BOOST_DI_REQUIRES(!is_creatable<TMemory, TArgs...>::value) = 0>
  T* get_impl(const TMemory&, TArgs&&...) const {
#if (BOOST_DI_CFG_DIAGNOSTICS_LEVEL > 0)  // __pph__
    return concepts::creatable_error<TInitialization, TName, T*, TArgs...>();
#else   // __pph__
    return {};
#endif  // __pph__
  }

  const TInjector& injector_;
};

namespace successful {

template <class, class, class>
struct provider;

template <class T, class TInjector, class TInitialization, class... TCtor>
struct provider<T, aux::pair<TInitialization, aux::type_list<TCtor...>>, TInjector> {
  using injector_t = TInjector;

  template <class TMemory = type_traits::heap>
  auto get(const TMemory& memory = {}) const {
    return TInjector::config::provider(injector_).template get<T>(
        TInitialization{}, memory,
        static_cast<const injector__<TInjector>&>(injector_).create_successful_impl(aux::type<TCtor>{})...);
  }

  const TInjector& injector_;
};
}
}  // successful::core

#endif
