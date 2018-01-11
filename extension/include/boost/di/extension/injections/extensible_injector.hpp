//
// Copyright (c) 2012-2018 Kanstantsin Chernik
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "boost/di.hpp"

BOOST_DI_NAMESPACE_BEGIN
namespace extension {

template <class TDependency>
class dependency_proxy
    : core::dependency_base,
      public core::dependency_impl<core::dependency_concept<typename TDependency::expected, typename TDependency::name>,
                                   dependency_proxy<TDependency>> {
 public:
  using scope = typename TDependency::scope;
  using expected = typename TDependency::expected;
  using given = typename TDependency::given;
  using name = typename TDependency::name;
  using priority = typename TDependency::priority;

  dependency_proxy(TDependency& orig_dependency) noexcept : orig_dependency_(orig_dependency) {}

  dependency_proxy(dependency_proxy& other) noexcept : orig_dependency_(other.orig_dependency_) {}
  dependency_proxy(dependency_proxy&& other) noexcept : orig_dependency_(other.orig_dependency_) {}

 protected:
  using scope_t = typename scope::template scope<expected, given>;

  template <class T>
  using is_referable = typename scope_t::template is_referable<T>;

  template <class T, class TName, class TProvider>
  static decltype(scope_t::template try_create<T, TName>(aux::declval<TProvider>())) try_create(const TProvider&);

  template <class T, class TName, class TProvider>
  auto create(const TProvider& provider) {
    return static_cast<core::dependency__<TDependency>&>(orig_dependency_).template create<T, TName>(provider);
  }

 private:
  TDependency& orig_dependency_;
};

template <class T>
struct remove_any_of {
  using type = T;
};
template <class T, class... Ts>
struct remove_any_of<di::concepts::any_of<T, Ts...>> {
  using type = T;
};

template <class TDependency, class TInjector>
auto make_extensible_impl(const aux::type<TDependency>&, TInjector& injector) {
  auto& dependency =
      core::binder::resolve<typename remove_any_of<typename TDependency::expected>::type, typename TDependency::name>(
          &injector);
  return dependency_proxy<TDependency>{dependency};
}

template <class... TDeps, class TInjector>
auto make_extensible(const aux::type_list<TDeps...>&, TInjector& injector) {
  return make_injector(make_extensible_impl(aux::type<TDeps>{}, injector)...);
}

template <class TInjector>
auto make_extensible(TInjector& injector) {
  return make_extensible(typename TInjector::deps{}, injector);
}

}  // extension
BOOST_DI_NAMESPACE_END
