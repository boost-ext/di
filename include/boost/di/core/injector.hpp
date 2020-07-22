//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_INJECTOR_HPP
#define BOOST_DI_CORE_INJECTOR_HPP

#if !defined(__BOOST_DI_CORE_INJECTOR_POLICY)

#include "boost/di/aux_/compiler.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/concepts/creatable.hpp"
#include "boost/di/config.hpp"
#include "boost/di/core/any_type.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/core/bindings.hpp"
#include "boost/di/core/policy.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/core/provider.hpp"
#include "boost/di/core/wrapper.hpp"
#include "boost/di/fwd.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"

namespace core {

struct from_injector {};
struct from_deps {};
struct init {};
struct with_error {};

template <class T, class, class>
struct referable {
  using type = T;
};

template <class T, class TConfig, class TDependency>
struct referable<T&, TConfig, TDependency> {
  using type = aux::conditional_t<TDependency::template is_referable<T&, TConfig>::value, T&, T>;
};

template <class T, class TConfig, class TDependency>
struct referable<const T&, TConfig, TDependency> {
  using type = aux::conditional_t<TDependency::template is_referable<const T&, TConfig>::value, const T&, T>;
};

#if defined(__MSVC__)  // __pph__
template <class T, class TConfig, class TDependency>
struct referable<T&&, TConfig, TDependency> {
  using type = aux::conditional_t<TDependency::template is_referable<T&&, TConfig>::value, T&&, T>;
};
#endif  // __pph__

template <class T, class TConfig, class TDependency>
using referable_t = typename referable<T, TConfig, TDependency>::type;

#if defined(__MSVC__)  // __pph__
template <class T, class TInjector>
inline auto build(TInjector&& injector) noexcept {
  return T{static_cast<TInjector&&>(injector)};
}
#endif  // __pph__

#define __BOOST_DI_CORE_INJECTOR_POLICY(...) __VA_ARGS__ __BOOST_DI_CORE_INJECTOR_POLICY_ELSE
#define __BOOST_DI_CORE_INJECTOR_POLICY_ELSE(...)
#define __BOOST_DI_INJECTOR_ITERATE

#endif

template <class TConfig __BOOST_DI_CORE_INJECTOR_POLICY(, class TPolicies = pool<>)(), class... TDeps>
class injector __BOOST_DI_CORE_INJECTOR_POLICY()(<TConfig, pool<>, TDeps...>)
    : public injector_base, public pool<bindings_t<TDeps...>> {
  using pool_t = pool<bindings_t<TDeps...>>;

 protected:
  template <class T, class TName = no_name, class TIsRoot = aux::false_type>
  struct is_creatable {
    using dependency_t = binder::resolve_t<injector, T, TName>;
    using ctor_t = typename type_traits::ctor_traits__<binder::resolve_template_t<injector, typename dependency_t::given>, T,
                                                       typename dependency_t::ctor>::type;
    using ctor_args_t = typename ctor_t::second::second;

    static constexpr auto value =
        aux::is_convertible<decltype(dependency__<dependency_t>::template try_create<T, TName>(
                                try_provider<ctor_t, injector, decltype(aux::declval<TConfig>().provider((injector*)0))>{})),
                            T>::value
            __BOOST_DI_CORE_INJECTOR_POLICY(
                &&policy::template try_call<arg_wrapper<T, TName, TIsRoot, ctor_args_t, dependency_t, pool_t>,
                                            TPolicies>::value)();
  };

  auto& cfg() { return config_; }
  const auto& cfg() const { return config_; }

 public:
  using deps = bindings_t<TDeps...>;
  using config = TConfig;

  injector(injector &&) = default;

  template <class... TArgs>
  explicit injector(const init&, TArgs... args) noexcept : injector{from_deps{}, static_cast<TArgs&&>(args)...} {}

  template <class TConfig_, class TPolicies_, class... TDeps_>
  explicit injector(injector<TConfig_, TPolicies_, TDeps_...> && other) noexcept
      : injector{from_injector{}, static_cast<injector<TConfig_, TPolicies_, TDeps_...>&&>(other), deps{}} {}

  template <class T>
  injector& operator=(T&& other) noexcept {
    static_cast<pool_t&>(*this).operator=(static_cast<T&&>(other));
    return *this;
  }

  template <class T, __BOOST_DI_REQUIRES(is_creatable<T, no_name, aux::true_type>::value) = 0>
  T create() const {
    return __BOOST_DI_TYPE_WKND(T) create_successful_impl<aux::true_type>(aux::type<T>{});
  }

  template <class T, __BOOST_DI_REQUIRES(!is_creatable<T, no_name, aux::true_type>::value) = 0>
  __BOOST_DI_CONCEPTS_CREATABLE_ERROR_MSG T
      // clang-format off
  create
      // clang-format on
      () const {
    return __BOOST_DI_TYPE_WKND(T) create_impl<aux::true_type>(aux::type<T>{});
  }

  template <template <class...> class T, class R = binder::resolve_template_t<injector, aux::identity<T<>>>,
            __BOOST_DI_REQUIRES(is_creatable<R, no_name, aux::true_type>::value) = 0>
  R
      // clang-format off
  create()
      // clang-format on
      const {
    return __BOOST_DI_TYPE_WKND(R) create_successful_impl<aux::true_type>(aux::type<R>{});
  }

  template <template <class...> class T, class R = binder::resolve_template_t<injector, aux::identity<T<>>>,
            __BOOST_DI_REQUIRES(!is_creatable<R, no_name, aux::true_type>::value) = 0>
  __BOOST_DI_CONCEPTS_CREATABLE_ERROR_MSG R
      // clang-format off
  create()
      // clang-format on
      const {
    return __BOOST_DI_TYPE_WKND(R) create_impl<aux::true_type>(aux::type<R>{});
  }

 protected:
  template <class T>
  struct try_create {
    using type = aux::conditional_t<is_creatable<T>::value, T, void>;
  };

  template <class TParent>
  struct try_create<any_type_fwd<TParent>> {
    using type = any_type<TParent, injector, with_error>;
  };

  template <class TParent>
  struct try_create<any_type_ref_fwd<TParent>> {
    using type = any_type_ref<TParent, injector, with_error>;
  };

  template <class TParent>
  struct try_create<any_type_1st_fwd<TParent>> {
    using type = any_type_1st<TParent, injector, with_error>;
  };

  template <class TParent>
  struct try_create<any_type_1st_ref_fwd<TParent>> {
    using type = any_type_1st_ref<TParent, injector, with_error>;
  };

  template <class TName, class T>
  struct try_create<BOOST_DI_NAMESPACE::named<TName, T>> {
    using type = aux::conditional_t<is_creatable<T, TName>::value, T, void>;
  };

  template <class TParent, int N, class T>
  struct try_create<core::ctor_arg<TParent, N, T&&>> {
    using type = aux::conditional_t<is_creatable<T>::value, T, void>;
  };

  template <class TParent, int N>
  struct try_create<core::ctor_arg<TParent, N, const placeholders::arg&>> {
    using type = any_type_1st_ref<TParent, injector, with_error>;
  };

  template <class T>
  struct try_create<self<T>> {
    using type = injector;
  };

  template <class TIsRoot = aux::false_type, class T>
  auto create_impl(const aux::type<T>&) const {
    return create_impl__<TIsRoot, T>();
  }

  template <class TIsRoot = aux::false_type, class TParent>
  auto create_impl(const aux::type<any_type_fwd<TParent>>&) const {
    return any_type<TParent, injector>{*this};
  }

  template <class TIsRoot = aux::false_type, class TParent>
  auto create_impl(const aux::type<any_type_ref_fwd<TParent>>&) const {
    return any_type_ref<TParent, injector, aux::false_type, aux::true_type>{*this};
  }

  template <class TIsRoot = aux::false_type, class TParent>
  auto create_impl(const aux::type<any_type_1st_fwd<TParent>>&) const {
    return any_type_1st<TParent, injector>{*this};
  }

  template <class TIsRoot = aux::false_type, class TParent>
  auto create_impl(const aux::type<any_type_1st_ref_fwd<TParent>>&) const {
    return any_type_1st_ref<TParent, injector, aux::false_type, aux::true_type>{*this};
  }

  template <class TIsRoot = aux::false_type, class T, class TName>
  auto create_impl(const aux::type<BOOST_DI_NAMESPACE::named<TName, T>>&) const {
    return create_impl__<TIsRoot, T, TName>();
  }

  template <class TIsRoot = aux::false_type, class TParent, int N, class T>
  auto create_impl(const aux::type<core::ctor_arg<TParent, N, T>>&) const {
    auto& dependency = binder::resolve<TParent>((injector*)this);
    return static_cast<core::ctor_arg<TParent, N, T>&>(dependency);
  }

  template <class TIsRoot = aux::false_type, class TParent, int N>
  auto create_impl(const aux::type<core::ctor_arg<TParent, N, const placeholders::arg&>>&) const {
    return any_type_1st_ref<TParent, injector, aux::false_type, aux::true_type>{*this};
  }

  template <class TIsRoot = aux::false_type, class T>
  auto create_successful_impl(const aux::type<T>&) const {
    return create_successful_impl__<TIsRoot, T>();
  }

  template <class TIsRoot = aux::false_type, class TParent>
  auto create_successful_impl(const aux::type<any_type_fwd<TParent>>&) const {
    return successful::any_type<TParent, injector>{*this};
  }

  template <class TIsRoot = aux::false_type, class TParent>
  auto create_successful_impl(const aux::type<any_type_ref_fwd<TParent>>&) const {
    return successful::any_type_ref<TParent, injector>{*this};
  }

  template <class TIsRoot = aux::false_type, class TParent>
  auto create_successful_impl(const aux::type<any_type_1st_fwd<TParent>>&) const {
    return successful::any_type_1st<TParent, injector>{*this};
  }

  template <class TIsRoot = aux::false_type, class TParent>
  auto create_successful_impl(const aux::type<any_type_1st_ref_fwd<TParent>>&) const {
    return successful::any_type_1st_ref<TParent, injector>{*this};
  }

  template <class TIsRoot = aux::false_type, class T, class TName>
  auto create_successful_impl(const aux::type<BOOST_DI_NAMESPACE::named<TName, T>>&) const {
    return create_successful_impl__<TIsRoot, T, TName>();
  }

  template <class TIsRoot = aux::false_type, class TParent, int N, class T>
  auto create_successful_impl(const aux::type<core::ctor_arg<TParent, N, T>>&) const {
    auto& dependency = binder::resolve<TParent>((injector*)this);
    return static_cast<core::ctor_arg<TParent, N, T>&>(dependency);
  }

  template <class TIsRoot = aux::false_type, class TParent, int N>
  auto create_successful_impl(const aux::type<core::ctor_arg<TParent, N, const placeholders::arg&>>&) const {
    return any_type_1st_ref<TParent, injector, aux::false_type, aux::true_type>{*this};
  }

  template <class TIsRoot = aux::false_type, class T>
  decltype(auto) create_successful_impl(const aux::type<self<T>>&) const {
    return *this;
  }

 private:
  explicit injector(const from_deps&) noexcept {}

  template <class... TArgs>
  explicit injector(const from_deps&, TArgs... args) noexcept
      : pool_t{bindings_t<TArgs...>{}, core::pool_t<TArgs...>{static_cast<TArgs&&>(args)...}} {}

  template <class TInjector, class... TArgs>
  injector(const from_injector&, TInjector&& injector, const aux::type_list<TArgs...>&) noexcept
#if defined(__MSVC__)  // __pph__
      : pool_t {
    bindings_t<TArgs...>{}, pool_t { build<TArgs>(static_cast<TInjector&&>(injector))... }
  }
#else   // __pph__
      : pool_t {
    bindings_t<TArgs...>{}, pool_t { TArgs{static_cast<TInjector&&>(injector)}... }
  }
#endif  // __pph__
  {}

  template <class TInjector>
  injector(const from_injector&, TInjector&&, const aux::type_list<>&) noexcept {}

  template <class TIsRoot = aux::false_type, class T, class TName = no_name>
  auto create_impl__() const {
    auto&& dependency = binder::resolve<T, TName>((injector*)this);
    using dependency_t = typename aux::remove_reference<decltype(dependency)>::type;
    using ctor_t = typename type_traits::ctor_traits__<binder::resolve_template_t<injector, typename dependency_t::given>, T,
                                                       typename dependency_t::ctor>::type;
    using provider_t = core::provider<ctor_t, TName, injector>;
    auto& creatable_dept = static_cast<dependency__<dependency_t>&>(dependency);
    using wrapper_t = decltype(creatable_dept.template create<T, TName>(provider_t{this}));
    __BOOST_DI_CORE_INJECTOR_POLICY(using ctor_args_t = typename ctor_t::second::second;
                                    policy::template call<arg_wrapper<T, TName, TIsRoot, ctor_args_t, dependency_t, pool_t>>(
                                        ((injector*)this)->cfg().policies(this));)
    () return wrapper<T, wrapper_t>{creatable_dept.template create<T, TName>(provider_t{this})};
  }

  template <class TIsRoot = aux::false_type, class T, class TName = no_name>
  auto create_successful_impl__() const {
    auto&& dependency = binder::resolve<T, TName>((injector*)this);
    using dependency_t = typename aux::remove_reference<decltype(dependency)>::type;
    using ctor_t = typename type_traits::ctor_traits__<binder::resolve_template_t<injector, typename dependency_t::given>, T,
                                                       typename dependency_t::ctor>::type;
    using provider_t = successful::provider<ctor_t, injector>;
    auto& creatable_dept = static_cast<dependency__<dependency_t>&>(dependency);
    using wrapper_t = decltype(creatable_dept.template create<T, TName>(provider_t{this}));
    using create_t = referable_t<T, config, dependency__<dependency_t>>;
    __BOOST_DI_CORE_INJECTOR_POLICY(using ctor_args_t = typename ctor_t::second::second;
                                    policy::template call<arg_wrapper<T, TName, TIsRoot, ctor_args_t, dependency_t, pool_t>>(
                                        ((injector*)this)->cfg().policies(this));)
    () return successful::wrapper<create_t, wrapper_t>{creatable_dept.template create<T, TName>(provider_t{this})};
  }

  config config_;
};

#if defined(__BOOST_DI_INJECTOR_ITERATE)
#undef BOOST_DI_CORE_INJECTOR_HPP
#undef __BOOST_DI_INJECTOR_ITERATE
#undef __BOOST_DI_CORE_INJECTOR_POLICY
#undef __BOOST_DI_CORE_INJECTOR_POLICY_ELSE
#define __BOOST_DI_CORE_INJECTOR_POLICY(...) __BOOST_DI_CORE_INJECTOR_POLICY_ELSE
#define __BOOST_DI_CORE_INJECTOR_POLICY_ELSE(...) __VA_ARGS__
#include "boost/di/core/injector.hpp"
}  // core

template <class T, class TInjector>
auto create(const TInjector& injector) -> decltype(injector.template create<T>()) {
  return injector.template create<T>();
}

template <template <class...> class T, class TInjector>
auto create(const TInjector& injector) -> decltype(injector.template create<T>()) {
  return injector.template create<T>();
}

template <class T, class TInjector>
constexpr auto is_creatable(const TInjector&) {
  return core::injector__<TInjector>::template is_creatable<T, no_name, aux::true_type>::value;
}

template <template <class...> class T, class TInjector>
constexpr auto is_creatable(const TInjector&) {
  return core::injector__<TInjector>::template is_creatable<core::binder::resolve_template_t<TInjector, aux::identity<T<>>>,
                                                            no_name, aux::true_type>::value;
}

#endif

#endif
