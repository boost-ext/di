//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef __BOOST_DI_CORE_POLICY_HPP
#define __BOOST_DI_CORE_POLICY_HPP

#include "boost/di/aux_/compiler.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/core/pool.hpp"

namespace core {

template <class...>
struct arg_wrapper;

template <class T, class TName, class TIsRoot, template <class...> class TList, class... TCtor, class TDependency, class TDeps>
struct arg_wrapper<T, TName, TIsRoot, TList<TCtor...>, TDependency, TDeps> {
  using type __BOOST_DI_UNUSED = T;
  using expected __BOOST_DI_UNUSED = typename TDependency::expected;
  using given __BOOST_DI_UNUSED = typename TDependency::given;
  using name __BOOST_DI_UNUSED = TName;
  using arity __BOOST_DI_UNUSED = aux::integral_constant<int, sizeof...(TCtor)>;
  using scope __BOOST_DI_UNUSED = typename TDependency::scope;
  using is_root __BOOST_DI_UNUSED = TIsRoot;

  template <class T_, class TName_, class TDefault_>
  using resolve = decltype(core::binder::resolve<T_, TName_, TDefault_>((TDeps*)0));
};

template <class T>
struct allow_void : T {};

template <>
struct allow_void<void> : aux::true_type {};

class policy {
  template <class TArg, class TPolicy, class TPolicies,
            __BOOST_DI_REQUIRES(!aux::is_base_of<_, aux::remove_reference_t<typename TArg::type>>::value) = 0>
  static void call_impl(const TPolicies& policies) noexcept {
    static_cast<const TPolicy&>(policies)(TArg{});
  }

  template <class TArg, class, class TPolicies,
            __BOOST_DI_REQUIRES(aux::is_base_of<_, aux::remove_reference_t<typename TArg::type>>::value) = 0>
  static void call_impl(const TPolicies&) noexcept {}

  template <class TArg, class TPolicy>
  struct try_call_impl
      : aux::conditional_t<aux::is_base_of<_, aux::remove_reference_t<typename TArg::type>>::value, aux::true_type,
                           allow_void<decltype((aux::declval<TPolicy>())(aux::declval<TArg>()))>> {};

 public:
  template <class, class>
  struct try_call;

  template <class TArg, class... TPolicies>
  struct try_call<TArg, pool_t<TPolicies...>> : aux::is_same<aux::bool_list<aux::always<TPolicies>::value...>,
                                                             aux::bool_list<try_call_impl<TArg, TPolicies>::value...>> {};

  template <class TArg, class... TPolicies>
  static void call(__BOOST_DI_UNUSED const pool_t<TPolicies...>& policies) noexcept {
    (void)aux::swallow{0, (call_impl<TArg, TPolicies>(policies), 0)...};
  }
};

}  // namespace core

#endif
