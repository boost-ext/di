//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FWD_HPP
#define BOOST_DI_FWD_HPP

#include "boost/di/fwd_ext.hpp"

namespace placeholders {
__BOOST_DI_UNUSED static const struct arg { } _{}; }

template <class, class = void>
struct named {};
struct no_name {
  constexpr auto operator()() const noexcept { return ""; }
};
template <class, class = int>
struct ctor_traits;

template <class>
struct self {};

struct ignore_policies {};

namespace core {
template <class>
struct any_type_fwd;
template <class>
struct any_type_ref_fwd;
template <class>
struct any_type_1st_fwd;
template <class>
struct any_type_1st_ref_fwd;

struct dependency_base {};
struct injector_base {};

template <class T>
struct dependency__ : T {
  using T::create;
  using T::is_referable;
  using T::try_create;
};

template <class T>
struct injector__ : T {
  using T::cfg;
  using T::create_impl;
  using T::create_successful_impl;

#if defined(__MSVC__)  // __pph__
  template <class... Ts>
  using is_creatable = typename T::template is_creatable<Ts...>;
  template <class... Ts>
  using try_create = typename T::template try_create<Ts...>;
#else   // __pph__
  using T::is_creatable;
  using T::try_create;
#endif  // __pph__
};

template <class, class...>
struct array;

struct deduced {};
struct none {};

template <class, class TExpected = deduced, class = TExpected, class = no_name, class = void, class = none>
class dependency;
}  // namespace core

namespace scopes {
class deduce;
class instance;
class singleton;
class unique;
}  // namespace scopes

#endif
