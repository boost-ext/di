//
// Copyright (c) 2012-2020 Kanstantsin Chernik
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "boost/di.hpp"

BOOST_DI_NAMESPACE_BEGIN
namespace extension {

template <class TScope, class TExpected, class TGiven, class TName, class TPriority, class TCtor>
class dependency_proxy : core::dependency_base,
                         public core::dependency_impl<core::dependency_concept<TExpected, TName>,
                                                      dependency_proxy<TScope, TExpected, TGiven, TName, TPriority, TCtor>> {
  using dependency_t = core::dependency<TScope, TExpected, TGiven, TName, TPriority, TCtor>;

 public:
  using scope = TScope;
  using expected = TExpected;
  using given = TGiven;
  using name = TName;
  using priority = TPriority;
  using ctor = TCtor;

  dependency_proxy(dependency_t& orig_dependency) noexcept : orig_dependency_(orig_dependency) {}
  dependency_proxy(dependency_proxy& other) noexcept : orig_dependency_(other.orig_dependency_) {}
  dependency_proxy(dependency_proxy&& other) noexcept : orig_dependency_(other.orig_dependency_) {}

  using scope_t = typename scope::template scope<expected, given>;

  template <class T, class TConfig>
  using is_referable = typename scope_t::template is_referable<T, TConfig>;

  template <class T, class Name, class TProvider>
  static decltype(scope_t::template try_create<T, Name>(aux::declval<TProvider>())) try_create(const TProvider&);

  template <class T, class Name, class TProvider>
  auto create(const TProvider& provider) {
    return static_cast<core::dependency__<dependency_t>&>(orig_dependency_).template create<T, Name>(provider);
  }

 private:
  dependency_t& orig_dependency_;
};

template <class TDependency, class TInjector>
auto make_extensible_impl(const aux::type<TDependency>&, TInjector& injector) {
  return dependency_proxy<typename TDependency::scope, typename TDependency::expected, typename TDependency::given,
                          typename TDependency::name, typename TDependency::priority, typename TDependency::ctor>{injector};
}

template <class... TDeps, class TInjector>
auto make_extensible(const aux::type_list<TDeps...>&, TInjector& injector) {
  (void)injector;
  return make_injector(make_extensible_impl(aux::type<TDeps>{}, injector)...);
}

template <class TInjector>
auto make_extensible(TInjector& injector) {
  return make_extensible(typename TInjector::deps{}, injector);
}

}  // namespace extension
BOOST_DI_NAMESPACE_END
