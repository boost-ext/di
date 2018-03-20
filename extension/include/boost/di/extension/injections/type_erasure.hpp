//
// Copyright (c) 2012-2018 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <type_traits>
#include <utility>

#include "boost/di.hpp"

BOOST_DI_NAMESPACE_BEGIN
namespace extension {

template <class T>
struct identity {
  using type = T;
};

template <class>
struct type_erasure_traits;

template <class R, class T, class... TArgs>
struct type_erasure_traits<R (T::*)(TArgs...) const> {
  using type = R (*)(void*, TArgs...);
};

template <class R, class T, class... TArgs>
struct type_erasure_traits<R (T::*)(TArgs...)> {
  using type = R (*)(void*, TArgs...);
};

template <class R, class... TArgs>
struct type_erasure_traits<R(TArgs...)> {
  using type = R (*)(void*, TArgs...);
};

template <class R, class... TArgs>
struct type_erasure_traits<R(TArgs...) const> {
  using type = R (*)(void*, TArgs...);
};

template <int n>
struct counter : counter<n - 1> {
  int _{};
};

template <>
struct counter<0> {};

template <class I, int MaxFunSize = 8>
class type_erasure {
 public:
  type_erasure() = default;

  template <class Tgt, class = std::enable_if_t<!std::is_convertible<std::decay_t<Tgt>, type_erasure>::value>>
  type_erasure(Tgt&& tgt) : type_erasure(std::forward<Tgt>(tgt), std::make_index_sequence<cs_counter<I>>{}) {}

  template <class Tgt, class T = std::decay_t<Tgt>, std::size_t... Ns>
  type_erasure(Tgt&& tgt, std::index_sequence<Ns...>) : ptr_{new T{std::forward<Tgt>(tgt)}}, vtable_{vtable<Tgt>()} {
    vtable_[0] = (void*)I::template dtor<T>;
    vtable_[1] = (void*)I::template copy_ctor<T>;
    (void)aux::swallow{0, (set<Ns, T>(identity<typename type_erasure_traits<typename decltype(
                                          I::fsign__(std::integral_constant<int, Ns>{}))::type>::type>{}),
                           0)...};
  }

  type_erasure(const type_erasure& other)
      : ptr_{(reinterpret_cast<void* (*)(void*)>(other.vtable_[1]))(other.ptr_)}, vtable_{other.vtable_} {}

  type_erasure(type_erasure&& other) : ptr_{other.ptr_}, vtable_{other.vtable_} { other.ptr_ = nullptr; }

  ~type_erasure() { (reinterpret_cast<void (*)(void*)>(vtable_[0]))(ptr_); }

 protected:
  using type__ = I;

  static counter<1> fnext__(counter<1>);

  template <class T>
  static constexpr auto cs_counter = sizeof(T::fnext__(::boost::di::extension::counter<MaxFunSize + 1>())) / sizeof(int) - 1;

  template <int Id, class TSign, class... Ts>
  decltype(auto) call__(Ts&&... args) const {
    return (*reinterpret_cast<typename type_erasure_traits<TSign>::type*>(&vtable_[2 + Id]))(ptr_, args...);
  }

  static constexpr auto CS_MAX_COUNT = MaxFunSize;

 private:
  template <class T>
  static auto& vtable() {
    static void* fs[2 + cs_counter<I>];
    return fs;
  }

  template <class T>
  static void dtor(void* self) {
    delete static_cast<T*>(self);
  }

  template <class T>
  static void* copy_ctor(void* self) {
    return new T{*static_cast<T*>(self)};
  }

  template <int Id, class T, class R, class... Ts>
  decltype(auto) set(identity<R (*)(void*, Ts...)>) {
    vtable_[2 + Id] =
        (void*)static_cast<R (*)(std::integral_constant<int, Id>, void*, Ts...)>(I::template fcall__<T, R, Ts...>);
  }

  void* ptr_{};
  void** vtable_{};
};

#define REQUIRES(name, ...) REQUIRES_IMPL(name, decltype(&type__::name), __VA_ARGS__)
#define REQUIRES_SIGN(name, sign, ...) REQUIRES_IMPL(name, sign, __VA_ARGS__)
#define REQUIRES_IMPL(name, sign, ...)                                                                                     \
  { return call__<__BOOST_DI_CAT(name, __LINE__)::value, sign>(__VA_ARGS__); }                                             \
  using __BOOST_DI_CAT(name, __LINE__) =                                                                                   \
      std::integral_constant<int, sizeof(fnext__(::boost::di::extension::counter<CS_MAX_COUNT + 1>())) / sizeof(int) - 1>; \
  static ::boost::di::extension::identity<sign> fsign__(__BOOST_DI_CAT(name, __LINE__));                                   \
  template <class T, class R, class... Ts>                                                                                 \
  static R fcall__(__BOOST_DI_CAT(name, __LINE__), void* ptr, Ts... args) {                                                \
    return static_cast<T*>(ptr)->name(args...);                                                                            \
  }                                                                                                                        \
  static auto fnext__(::boost::di::extension::counter<__BOOST_DI_CAT(name, __LINE__)::value + 2> id)->decltype(id)

}  // extension
BOOST_DI_NAMESPACE_END
