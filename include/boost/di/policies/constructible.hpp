//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_CONSTRUCTIBLE_HPP
#define BOOST_DI_POLICIES_CONSTRUCTIBLE_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/fwd.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"

namespace policies {
namespace detail {

struct type_op {};

template <class T, class = int>
struct apply_impl {
  template <class>
  struct apply : T {};
};

template <template <class...> class T, class... Ts>
struct apply_impl<T<Ts...>, __BOOST_DI_REQUIRES(!aux::is_base_of<type_op, T<Ts...>>::value)> {
  template <class TOp, class>
  struct apply_placeholder_impl {
    using type = TOp;
  };

  template <class TOp>
  struct apply_placeholder_impl<_, TOp> {
    using type = TOp;
  };

  template <template <class...> class TExpr, class TOp, class... TArgs>
  struct apply_placeholder {
    using type = TExpr<typename apply_placeholder_impl<TArgs, TOp>::type...>;
  };

  template <class TArg>
  struct apply : apply_placeholder<T, typename TArg::type, Ts...>::type {};
};

template <class T>
struct apply_impl<T, __BOOST_DI_REQUIRES(aux::is_base_of<type_op, T>::value)> {
  template <class TArg>
  struct apply : T::template apply<TArg>::type {};
};

template <class T>
struct not_ : detail::type_op {
  template <class TArg>
  struct apply : aux::integral_constant<bool, !detail::apply_impl<T>::template apply<TArg>::value> {};
};

template <class... Ts>
struct and_ : detail::type_op {
  template <class TArg>
  struct apply : aux::is_same<aux::bool_list<detail::apply_impl<Ts>::template apply<TArg>::value...>,
                              aux::bool_list<aux::always<Ts>::value...>> {};
};

template <class... Ts>
struct or_ : detail::type_op {
  template <class TArg>
  struct apply : aux::integral_constant<bool,
                                        !aux::is_same<aux::bool_list<detail::apply_impl<Ts>::template apply<TArg>::value...>,
                                                      aux::bool_list<aux::never<Ts>::value...>>::value> {};
};

}  // namespace detail

template <class T>
struct type {
  template <class TPolicy>
  struct not_allowed_by {
    operator aux::false_type() const {
      using constraint_not_satisfied = not_allowed_by;
      return constraint_not_satisfied{}.error();
    }

    // clang-format off
    static inline aux::false_type
	error(_ = "type disabled by constructible policy, added by BOOST_DI_CFG or make_injector<CONFIG>!");
    // clang-format on
  };
};

template <class>
struct is_root : detail::type_op {
  template <class TArg>
  struct apply : TArg::is_root {};
};

template <class T>
struct is_bound : detail::type_op {
  struct not_resolved {};
  template <class TArg>
  struct apply
      : aux::integral_constant<
            bool,
            !aux::is_same<typename TArg::template resolve<aux::conditional_t<aux::is_same<T, _>::value, typename TArg::type, T>,
                                                          typename TArg::name, not_resolved>,
                          not_resolved>::value> {};
};

template <class T>
struct is_injected : detail::type_op {
  template <class TArg, class U = aux::decay_t<aux::conditional_t<aux::is_same<T, _>::value, typename TArg::type, T>>>
  struct apply : aux::conditional_t<aux::is_class<U>::value, typename type_traits::is_injectable<U>::type, aux::true_type> {};
};

static constexpr auto include_root = true;

namespace operators {

template <class X, class Y>
inline auto operator||(const X&, const Y&) {
  return detail::or_<X, Y>{};
}

template <class X, class Y>
inline auto operator&&(const X&, const Y&) {
  return detail::and_<X, Y>{};
}

template <class T>
inline auto operator!(const T&) {
  return detail::not_<T>{};
}

}  // namespace operators

template <bool, class T>
struct constructible_impl {
  template <class TArg, __BOOST_DI_REQUIRES(TArg::is_root::value || T::template apply<TArg>::value) = 0>
  aux::true_type operator()(const TArg&) const {
    return {};
  }

  template <class TArg, __BOOST_DI_REQUIRES(!TArg::is_root::value && !T::template apply<TArg>::value) = 0>
  aux::false_type operator()(const TArg&) const {
    return typename type<typename TArg::type>::template not_allowed_by<T>{};
  }
};

template <class T>
struct constructible_impl<true, T> {
  template <class TArg, __BOOST_DI_REQUIRES(T::template apply<TArg>::value) = 0>
  aux::true_type operator()(const TArg&) const {
    return {};
  }

  template <class TArg, __BOOST_DI_REQUIRES(!T::template apply<TArg>::value) = 0>
  aux::false_type operator()(const TArg&) const {
    return typename type<typename TArg::type>::template not_allowed_by<T>{};
  }
};

template <bool IncludeRoot = false, class T = aux::never<_>,
          __BOOST_DI_REQUIRES(aux::is_base_of<detail::type_op, T>::value) = 0>
inline auto constructible(const T& = {}) {
  return constructible_impl<IncludeRoot, T>{};
}

template <bool IncludeRoot = false, class T = aux::never<_>,
          __BOOST_DI_REQUIRES(!aux::is_base_of<detail::type_op, T>::value) = 0>
inline auto constructible(const T& = {}) {
  return constructible_impl<IncludeRoot, detail::or_<T>>{};
}

}  // namespace policies

#endif
