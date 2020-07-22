//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_BINDER_HPP
#define BOOST_DI_CORE_BINDER_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/dependency.hpp"
#include "boost/di/scopes/deduce.hpp"

namespace core {

struct binder {
  template <class TDefault, class>
  static TDefault resolve_impl(...) noexcept {
    return {};
  }

  template <class, class TConcept, class TDependency>
  static decltype(auto) resolve_impl(aux::pair<TConcept, TDependency>* dep) noexcept {
    return static_cast<TDependency&>(*dep);
  }

  template <class, class TConcept, class TScope, class TExpected, class TGiven, class TName, class TCtor,
            template <class, class, class, class, class, class> class TDependency>
  static decltype(auto) resolve_impl(
      aux::pair<TConcept, TDependency<TScope, TExpected, TGiven, TName, override, TCtor>>* dep) noexcept {
    return static_cast<TDependency<TScope, TExpected, TGiven, TName, override, TCtor>&>(*dep);
  }

  template <class TDefault, class>
  static TDefault resolve_impl__(...);

  template <class, class TConcept, class TDependency>
  static TDependency resolve_impl__(aux::pair<TConcept, TDependency>*);

  template <class, class TConcept, class TScope, class TExpected, class TGiven, class TName, class TCtor,
            template <class, class, class, class, class, class> class TDependency>
  static dependency<TScope, TExpected, TGiven, TName, override, TCtor> resolve_impl__(
      aux::pair<TConcept, TDependency<TScope, TExpected, TGiven, TName, override, TCtor>>*);

  template <class TDeps, class T, class TName, class TDefault>
  struct resolve__ {
    using type = decltype(resolve_impl__<TDefault, dependency_concept<aux::decay_t<T>, TName>>((TDeps*)0));
  };

/// Wknd for https://llvm.org/bugs/show_bug.cgi?id=28844
#if (defined(__CLANG__) && __CLANG__ >= 3'9)  // __pph__
  template <class TDeps, class T>
  static T& resolve_(TDeps* deps, const aux::type<T&>&) noexcept {
    return static_cast<T&>(*deps);
  }

  template <class TDeps, class T>
  static T resolve_(TDeps*, const aux::type<T>&) noexcept {
    return {};
  }
#endif  // __pph__

  template <class, class T>
  struct resolve_template_impl {
    using type = T;
  };

  template <class TDeps, template <class...> class T, class... Ts>
  struct resolve_template_impl<TDeps, aux::identity<T<Ts...>>> {
    using type = T<typename resolve_template_impl<
        TDeps, aux::remove_qualifiers_t<typename resolve__<
                   TDeps, Ts, no_name, dependency<scopes::deduce, aux::decay_t<Ts>>>::type::given>>::type...>;
  };

  template <class T, class TName = no_name, class TDefault = dependency<scopes::deduce, aux::decay_t<T>>, class TDeps>
  static decltype(auto) resolve(TDeps* deps) noexcept {
    using dependency = dependency_concept<aux::decay_t<T>, TName>;

/// Wknd for https://llvm.org/bugs/show_bug.cgi?id=28844
#if (defined(__CLANG__) && __CLANG__ >= 3'9)  // __pph__
    return resolve_(deps, aux::type<decltype(resolve_impl<TDefault, dependency>((TDeps*)0))>{});
#else   // __pph__
    return resolve_impl<TDefault, dependency>(deps);
#endif  // __pph__
  }

  template <class TDeps, class T, class TName = no_name, class TDefault = dependency<scopes::deduce, aux::decay_t<T>>>
  using resolve_t = typename resolve__<TDeps, T, TName, TDefault>::type;

  template <class TDeps, class T>
  using resolve_template_t = typename resolve_template_impl<TDeps, aux::remove_qualifiers_t<T>>::type;
};

}  // namespace core

#endif
