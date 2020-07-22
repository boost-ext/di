//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_PROVIDER_HPP
#define BOOST_DI_CORE_PROVIDER_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/concepts/creatable.hpp"
#include "boost/di/fwd.hpp"
#include "boost/di/type_traits/memory_traits.hpp"

namespace core {

#if (BOOST_DI_CFG_DIAGNOSTICS_LEVEL >= 2)  // __pph__
template <class T>
struct creating {
  creating() { type(); }
  static inline T type(_ = "creating...");
};
#endif  // __pph__

template <class, class, class>
struct try_provider;

template <class T, class TInjector, class TProvider, class TInitialization, template <class...> class TList, class... TCtor>
struct try_provider<aux::pair<T, aux::pair<TInitialization, TList<TCtor...>>>, TInjector, TProvider> {
  using injector = TInjector;
  using config = typename TInjector::config;

  template <class>
  struct is_creatable {
    static constexpr auto value =
        TProvider::template is_creatable<TInitialization, T,
                                         typename injector__<TInjector>::template try_create<TCtor>::type...>::value;
  };

  template <class TMemory = type_traits::heap>
  auto get(const TMemory& = {}) const
      -> aux::enable_if_t<is_creatable<TMemory>::value,
                          aux::conditional_t<aux::is_same<TMemory, type_traits::stack>::value, T, aux::remove_reference_t<T>*>>;
};

template <class, class, class>
struct provider;

template <class T, class TName, class TInjector, class TInitialization, template <class...> class TList, class... TCtor>
struct provider<aux::pair<T, aux::pair<TInitialization, TList<TCtor...>>>, TName, TInjector> {
  using injector = TInjector;
  using config = typename TInjector::config;

  template <class, class... TArgs>
  struct is_creatable {
    using type = decltype(aux::declval<injector__<TInjector>>().cfg().provider((TInjector*)0));
    static constexpr auto value = type::template is_creatable<TInitialization, T, TArgs...>::value;
  };

  template <class TMemory = type_traits::heap>
  auto get(const TMemory& memory = {}) const {
    return get_impl(memory, ((const injector__<TInjector>*)injector_)->create_impl(aux::type<TCtor>{})...);
  }

  template <class TMemory, class... TArgs, __BOOST_DI_REQUIRES(is_creatable<TMemory, TArgs...>::value) = 0>
  auto get_impl(const TMemory& memory, TArgs&&... args) const {
#if (BOOST_DI_CFG_DIAGNOSTICS_LEVEL >= 2)  // __pph__
    (void)aux::conditional_t<injector__<TInjector>::template is_creatable<T>::value, _, creating<T>>{};
#endif  // __pph__

    return cfg().provider(injector_).template get<T>(TInitialization{}, memory, static_cast<TArgs&&>(args)...);
  }

  template <class TMemory, class... TArgs, __BOOST_DI_REQUIRES(!is_creatable<TMemory, TArgs...>::value) = 0>
  auto get_impl(const TMemory&, TArgs&&...) const {
#if (BOOST_DI_CFG_DIAGNOSTICS_LEVEL > 0)  // __pph__
    return concepts::creatable_error<TInitialization, TName, T*, TArgs...>();
#else   // __pph__
    return nullptr;
#endif  // __pph__
  }

  auto& super() const { return *injector_; }
  auto& cfg() const { return ((injector__<TInjector>*)injector_)->cfg(); }

  const TInjector* injector_;
};

namespace successful {
template <class, class>
struct provider;

template <class T, class TInjector, class TInitialization, template <class...> class TList, class... TCtor>
struct provider<aux::pair<T, aux::pair<TInitialization, TList<TCtor...>>>, TInjector> {
  using injector = TInjector;
  using config = typename TInjector::config;

  template <class TMemory = type_traits::heap>
  auto get(const TMemory& memory = {}) const {
    return cfg().provider(injector_).template get<T>(
        TInitialization{}, memory, ((const injector__<TInjector>*)injector_)->create_successful_impl(aux::type<TCtor>{})...);
  }

  auto& super() const { return *injector_; }
  auto& cfg() const { return ((injector__<TInjector>*)injector_)->cfg(); }

  const TInjector* injector_;
};
}  // namespace successful
}  // namespace core

#endif
