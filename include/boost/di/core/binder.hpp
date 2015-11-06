//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
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

class binder {
  template <class TDefault, class>
  static TDefault resolve_impl(...) noexcept {
    return {};
  }

  template <class, class TConcept, class TDependency>
  static decltype(auto) resolve_impl(aux::pair<TConcept, TDependency>* dep) noexcept {
    return static_cast<TDependency&>(*dep);
  }

  template <class, class TConcept, class TScope, class TExpected, class TGiven, class TName, class TBase>
  static decltype(auto) resolve_impl(
      aux::pair<TConcept, dependency<TScope, TExpected, TGiven, TName, override, TBase>>* dep) noexcept {
    return static_cast<dependency<TScope, TExpected, TGiven, TName, override, TBase>&>(*dep);
  }

  template <class TDeps, class T, class TName, class TDefault>
  struct resolve__ {
    using dependency = dependency_concept<aux::decay_t<T>, TName>;
    using type = aux::remove_reference_t<decltype(resolve_impl<TDefault, dependency>((TDeps*)0))>;
  };

 public:
  template <class T, class TName = no_name, class TDefault = dependency<scopes::deduce, aux::decay_t<T>>, class TDeps>
  static decltype(auto) resolve(TDeps* deps) noexcept {
    using dependency = dependency_concept<aux::decay_t<T>, TName>;
    return resolve_impl<TDefault, dependency>(deps);
  }

  template <class TDeps, class T, class TName = no_name, class TDefault = dependency<scopes::deduce, aux::decay_t<T>>>
  using resolve_t = typename resolve__<TDeps, T, TName, TDefault>::type;
};

}  // core

#endif
